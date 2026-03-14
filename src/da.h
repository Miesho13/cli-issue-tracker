#ifndef __DA__
#define __DA__

#include <string.h>
#include <assert.h>

#define DECLTYPE_CAST(T)
#define DA_INIT_CAP (255)

#define da_reserve(da, expected_capacity)                                                                        \
    do {                                                                                                         \
        if ((expected_capacity) > (da)->capacity) {                                                              \
            if ((da)->capacity == 0) {                                                                           \
                (da)->capacity = DA_INIT_CAP;                                                                    \
            }                                                                                                    \
            while ((expected_capacity) > (da)->capacity) {                                                       \
                (da)->capacity *= 2;                                                                             \
            }                                                                                                    \
            (da)->items = DECLTYPE_CAST((da)->items)realloc((da)->items, (da)->capacity * sizeof(*(da)->items)); \
            assert((da)->items != NULL && "Memory cant be alloc");                                               \
        }                                                                                                        \
    } while (0)

#define da_append(da, item)                                                             \
    do {                                                                                \
        if ((da)->count >= (da)->capacity) {                                            \
            (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity*2;      \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items));    \
            assert((da)->items != NULL && "Memory cant be alloc");                      \
        }                                                                               \
        (da)->items[(da)->count++] = (item);                                            \
    } while (0)

#define da_append_many(da, new_items, new_items_count)                                           \
    do {                                                                                         \
        da_reserve((da), (da)->count + (new_items_count));                                       \
        memcpy((da)->items + (da)->count, (new_items), (new_items_count)*sizeof(*(da)->items));  \
        (da)->count += (new_items_count);                                                        \
    } while (0)

#define da_append(da, item)                                                             \
    do {                                                                                \
        if ((da)->count >= (da)->capacity) {                                            \
            (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity*2;      \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items));    \
            assert((da)->items != NULL && "Memory cant be alloc");                      \
        }                                                                               \
        (da)->items[(da)->count++] = (item);                                            \
    } while (0)

#endif // __DA__
