//
// Single-producer/single-consumer queue for flow deltas.
//

#include "delta_queue.h"

#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "ogs-core.h"

#define FLOW_DELTA_QUEUE_CAPACITY 4096

typedef struct {
    _Atomic(size_t) head;
    _Atomic(size_t) tail;
    size_t capacity;
    flow_delta_entry **entries;
} flow_delta_queue_t;

static flow_delta_queue_t g_queue = {
    .head = ATOMIC_VAR_INIT(0),
    .tail = ATOMIC_VAR_INIT(0),
    .capacity = 0,
    .entries = NULL,
};

static pthread_once_t g_queue_once = PTHREAD_ONCE_INIT;

static void flow_delta_queue_init_once(void) {
    g_queue.capacity = FLOW_DELTA_QUEUE_CAPACITY;
    g_queue.entries = calloc(g_queue.capacity, sizeof(flow_delta_entry *));
    if (!g_queue.entries) {
        ogs_fatal("flow_delta_queue: failed to allocate entries (%d)", errno);
        g_queue.capacity = 0;
        return;
    }
    atomic_store_explicit(&g_queue.head, 0, memory_order_relaxed);
    atomic_store_explicit(&g_queue.tail, 0, memory_order_relaxed);
}

static inline void flow_delta_queue_ensure(void) {
    pthread_once(&g_queue_once, flow_delta_queue_init_once);
}

bool flow_delta_enqueue(flow_delta_entry *entry) {
    if (!entry) return false;
    flow_delta_queue_ensure();
    if (!g_queue.entries || !g_queue.capacity) {
        return false;
    }

    size_t head = atomic_load_explicit(&g_queue.head, memory_order_relaxed);
    size_t tail = atomic_load_explicit(&g_queue.tail, memory_order_acquire);
    if (head - tail >= g_queue.capacity) {
        ogs_warn("flow_delta_queue: drop delta (queue full)");
        return false;
    }
    g_queue.entries[head % g_queue.capacity] = entry;
    atomic_store_explicit(&g_queue.head, head + 1, memory_order_release);
    return true;
}

flow_delta_entry *flow_delta_try_dequeue(void) {
    flow_delta_queue_ensure();
    if (!g_queue.entries || !g_queue.capacity) {
        return NULL;
    }

    size_t tail = atomic_load_explicit(&g_queue.tail, memory_order_relaxed);
    size_t head = atomic_load_explicit(&g_queue.head, memory_order_acquire);
    if (tail == head) {
        return NULL;
    }
    size_t idx = tail % g_queue.capacity;
    flow_delta_entry *entry = g_queue.entries[idx];
    g_queue.entries[idx] = NULL;
    atomic_store_explicit(&g_queue.tail, tail + 1, memory_order_release);
    return entry;
}

void flow_delta_queue_shutdown(void) {
    flow_delta_queue_ensure();
    if (!g_queue.entries) return;
    flow_delta_entry *entry;
    while ((entry = flow_delta_try_dequeue()) != NULL) {
        flow_delta_entry_free(entry);
    }
    free(g_queue.entries);
    g_queue.entries = NULL;
    g_queue.capacity = 0;
}

void flow_delta_entry_free(flow_delta_entry *entry) {
    if (!entry) return;
    free(entry->str_key);
    free(entry->usubid);
    free(entry->ue_ip);
    free(entry->dst_ip);
    free(entry);
}
