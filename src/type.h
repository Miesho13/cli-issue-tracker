#ifndef __TYPE__
#define __TYPE__

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef double f64;
typedef float  f32;

typedef enum {
    STATUS_OK  = 0,
    STATUS_ERR = 0xff,
} status_t;



#endif // __TYPE__
