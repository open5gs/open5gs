#ifndef __CORE_QUEUE_H__
#define __CORE_QUEUE_H__

#include "core.h"

#define que_declare(__name, __type, __size) \
    struct { \
        int head, tail, size, used; \
        __type pool[(__size)]; \
    } __name

#define que_init(__pname, __size) \
    ((__pname)->head = (__pname)->tail = 0, \
     (__pname)->used = 0, \
     (__pname)->size = __size)

#define que_push(__pname, __ptr_node) \
    ((__pname)->used == ((__pname)->size) ? -1 : \
        (((__pname)->pool[(__pname)->head] = *(__ptr_node), \
          (__pname)->head = ((__pname)->head + 1) % ((__pname)->size), \
          ++(__pname)->used)))


#define que_pop(__pname, __ptr_node) \
    ((__pname)->used == 0 ? -1 : \
        ((*(__ptr_node) = (__pname)->pool[(__pname)->tail], \
          (__pname)->tail = ((__pname)->tail + 1) % ((__pname)->size), \
          --(__pname)->used)))

#define que_is_empty(__pname) ((__pname)->used == 0)

#define que_is_full(__pname) ((__pname)->used == (__pname)->size)

#define que_size(__pname) ((__pname)->size)

#define que_used(__pname) ((__pname)->used)

#define que_avail(__pname) (que_size(__pname) - que_used(__pname))

#endif /* ! __CORE_QUEUE_H__ */
