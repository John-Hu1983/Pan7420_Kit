#ifndef _SPIN_DRV_H_
#define _SPIN_DRV_H_

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t osvar_t;

typedef struct
{
    u8 r;
    u8 g;
    u8 b;
} rgb_t;

typedef enum
{
    op_success = 0,
    op_para_error,
    op_time_out,
    op_busy,
    op_failed,
    op_max,
    op_empty,
} op_res;

typedef struct
{
    uint8_t step;
    uint16_t period;
} event_t;

typedef enum
{
    BTN_IDLE,
    BTN_CLICK,
    BTN_HOLD,
    BTN_END
} btnval_t;

typedef struct
{
    uint16_t tick;
    uint16_t range;
} debounce_t;

typedef struct
{
    uint16_t sum;
    uint16_t average;
} filter_t;

#endif