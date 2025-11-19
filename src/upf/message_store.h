//
// Created by Fatemeh Shafiei Ardestani on 2025-11-09.
//

#ifndef OPEN5GS_MESSAGE_STORE_H
#define OPEN5GS_MESSAGE_STORE_H
#pragma once
#include <stdatomic.h>
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef void (*ms_destructor_fn)(void *p);
typedef struct {
    _Atomic(void*) slot;
    ms_destructor_fn dtor;
} message_store_t;

static inline void message_store_init(message_store_t *ms, ms_destructor_fn dtor) {
  atomic_store_explicit(&m->slot, NULL, memory_order_relaxed);
  m->dtor = dtor;
}
static inline void message_store_publish(message_store_t *m, void *new_data) {
  void *old = atomic_exchange_explicit(&m->slot, new_data, memory_order_acq_rel);
  if (old) {
    if (m->dtor) m->dtor(old);
  }
}
static inline void *message_store_try_read(message_store_t *m) {
  return atomic_exchange_explicit(&m->slot, NULL, memory_order_acq_rel);
}
static inline void message_store_clear(message_store_t *m) {
  void *p = atomic_exchange_explicit(&m->slot, NULL, memory_order_acq_rel);
  if (p && m->dtor) m->dtor(p);
}
#ifdef __cplusplus
}
#endif
#endif //OPEN5GS_MESSAGE_STORE_H
