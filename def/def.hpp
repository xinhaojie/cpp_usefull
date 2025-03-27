#pragma once

#define __ASM               __asm
#define STATIC              static
#define __INLINE            __inline
#define __NOINLINE          __attribute__((noinline))
#define __IRQ               __attribute__ ((interrupt ("IRQ")))

#define  ARRAY_SIZE(x)          (sizeof(x) / sizeof(x[0]))
#define  FALSE                  0
#define  TRUE                   1

#define NEWLINE                 "\n"

#ifndef NULL
#define  NULL                   (void *)0
#endif

#define BIT_31                  ((UINT32)(1 << 31))
#define BIT_30                  ((UINT32)(1 << 30))
#define BIT_29                  ((UINT32)(1 << 29))
#define BIT_28                  ((UINT32)(1 << 28))
#define BIT_27                  ((UINT32)(1 << 27))
#define BIT_26                  ((UINT32)(1 << 26))
#define BIT_25                  ((UINT32)(1 << 25))
#define BIT_24                  ((UINT32)(1 << 24))
#define BIT_23                  ((UINT32)(1 << 23))
#define BIT_22                  ((UINT32)(1 << 22))
#define BIT_21                  ((UINT32)(1 << 21))
#define BIT_20                  ((UINT32)(1 << 20))
#define BIT_19                  ((UINT32)(1 << 19))
#define BIT_18                  ((UINT32)(1 << 18))
#define BIT_17                  ((UINT32)(1 << 17))
#define BIT_16                  ((UINT32)(1 << 16))
#define BIT_15                  ((UINT32)(1 << 15))
#define BIT_14                  ((UINT32)(1 << 14))
#define BIT_13                  ((UINT32)(1 << 13))
#define BIT_12                  ((UINT32)(1 << 12))
#define BIT_11                  ((UINT32)(1 << 11))
#define BIT_10                  ((UINT32)(1 << 10))
#define BIT_9                   ((UINT32)(1 << 9))
#define BIT_8                   ((UINT32)(1 << 8))
#define BIT_7                   ((UINT32)(1 << 7))
#define BIT_6                   ((UINT32)(1 << 6))
#define BIT_5                   ((UINT32)(1 << 5))
#define BIT_4                   ((UINT32)(1 << 4))
#define BIT_3                   ((UINT32)(1 << 3))
#define BIT_2                   ((UINT32)(1 << 2))
#define BIT_1                   ((UINT32)(1 << 1))
#define BIT_0                   ((UINT32)(1 << 0))

#define BIT_N(n)                ((UINT32)(1 << n))
#define CLEAR_BIT_N(n)          (~((UINT32)(1 << n)))


#define WEAK __attribute__((weak))
#define USED  __attribute__((used))
#define UNUSED  __attribute__((unused))
#define NORETURN __attribute__((noreturn))
#define ALIGNED(x)  __attribute__((aligned(x)))
#define ALIGNED1    __attribute__((packed))
#define PRINTF_CHECK(fmt, arg)  __attribute__ ((format (printf, fmt, arg))
#define SCANF_CHECK(fmt, arg)  __attribute__ ((format (scanf, fmt, arg))
#define DEPRECATED __attribute__((deprecated("已废弃")))
#define INLINE  __attribute__((always_inline)) inline
#define NOUNLL_ALL  __attribute__((nonnull))
#define NOUNLL_ONE(index)  __attribute__((nonnull(index)))
#define NOUNLL(start, end)  __attribute__((nonnull(start, end)))
#define RETURNS_NONULL  __attribute__((returns_nonnull))
#define RUN_HOT __attribute__((hot))
#define RUN_COLD __attribute__((cold))

#define BUILD_CLZ(x) __builtin_clz(x)	    //计算x前导0的个数。x=0时结果未定义
#define BUILD_CTZ(x) __builtin_ctz(x)	    //计算x末尾0的个数。x=0时结果未定义
#define BUILD_FFS(x) __builtin_ffs(x)	    //返回x中最后一个为1的位是从后向前的第几位，如__builtin_ffs(0x789)=1, __builtin_ffs(0x78c)=3
#define BUILD_POPCOUNT(x) __builtin_popcount(x)	        //计算x中1的个数
#define BUILD_PARITY(x) __builtin_parity(x)	            //计算x中1个数的奇偶性
#define BUILD_NEARBYINT(x) __builtin_nearbyint(x)	    //计算参数x经过四舍五入后的值
#define BUILD_FLOOR(x) __builtin_floor(x)	            //向下取整，既取不大于x的最大整数
#define BUILD_CEIL(x) __builtin_ceil(x)	                //向上取整，既返回不小于x的最小整数
#define BUILD_TRUNC(x) __builtin_trunc(x)	            //将数字x的小数部分直接去掉，返回整数
#define BUILD_SQRT(x) __builtin_sqrt(x)	                //求x的开平方根，返回结果