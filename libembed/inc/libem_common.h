/**
 * @file
 * @brief   Libembed Common File
 * @internal
 * @par Modification history
 * - 1.00 2016-10-17  noodlefighter, first implementation
 * @endinternal
 */

#ifndef __LIBEM_COMMON_H__
#define __LIBEM_COMMON_H__

#include <stdint.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>
#include "_libem_config.h"   /** from user dir */

/**
 * @name keyword redefine for unity test and widely compiler support
 * @{
 */

#define __STATIC_INLINE static inline  /**< @brief static inline func */
#define __INLINE        inline         /**< @brief inline func */

#define __LOCAL         static         /**< @brief local func and var */

#define __ROM           const          /**< @brief data stored in ROM */
#define __ROM_BYTE(n)   (n)            /**< @brief read byte stored in ROM */
#define __ROM_PTR(n)    (n)            /**< @brief read pointer stored in ROM */

/** @} */


/**
 * @name Bit Operation
 * @{
 */
#define __BITS_PER_BYTE             (8)
#define __BITS_PER_WORD             (LIBEMBED_SRCCFG_WORD_LENGTH)

/** @brief mask of a bit */
#define BIT(bit)                    (1 << (bit))

/** @brief determine whether or not to set */
#define BIT_ISSET(data, bit)        ((data) & BIT(bit))

/** @brief get value of bit */
#define BIT_GET(data, bit)          (BIT_ISSET(data, bit) ? 1 : 0)

/** @brief set bit */
#define BIT_SET(data, bit)          ((data) |= BIT(bit))

/** @brief clear bit */
#define BIT_CLR(data, bit)          ((data) &= ~BIT(bit))

/** @brief toggle bit */
#define BIT_TOGGLE(data, bit)       ((data) ^= BIT(bit))

/** @brief set bits with mask */
#define BIT_SET_MASK(data, mask)    ((data) |= (mask))

/** @brief clear bits with mask */
#define BIT_CLR_MASK(data, mask)    ((data) &= ~(mask))

/** @brief clear bits with mask */
#define BIT_TOGGLE_MASK(data, mask) ((data) ^= ~(mask))

/** @brief value bitwise left shift */
#define SBF(value, field)           ((value) << (field))

/** @brief mark of bits */
#define BITS_MASK(n)                (~((~0u) << (n)))

/** @brief get value of bits*/
#define BITS_GET(data, start, len)  (((data) >> (start)) & BITS_MASK(len))

/** @brief set value of bits */
#define BITS_SET(data, start, len, value)                   \
    ((data) = (((data) & ~SBF(BITS_MASK(len), (start))) |   \
        SBF((value) & (BITS_MASK(len)), (start))))

/** @} */

//todo:...

/**
 * @brief 取2-byte整数的高位byte
 *
 * @par 示例
 * @code
 *  uint16_t a = 0x1234;
 *  uint16_t b;
 *
 *  b = MSB(a);  //b=0x12
 * @endcode
 */
#define MSB(x)       (((x) >> 8) & 0xff)

/**
 * @brief 取2-byte整数的低位byte
 *
 * @par 示例
 * @code
 *  uint16_t a = 0x1234;
 *  uint16_t b;
 *
 *  b = LSB(a);  //b=0x34
 * @endcode
 */
#define LSB(x)       ((x) & 0xff)

/**
 * @brief 取2-word整数的高位word
 *
 * @par 示例
 * @code
 *  uint32_t a = 0x12345678;
 *  uint32_t b;
 *
 *  b = MSW(a);  //b=0x1234
 * @endcode
 */
#define MSW(x)       (((x) >> 16) & 0xffff)

/**
 * @brief 取2-word整数的低位word
 *
 * @par 示例
 * @code
 *  uint32_t a = 0x12345678;
 *  uint32_t b;
 *
 *  b = LSW(a);  //b=0x5678
 * @endcode
 */
#define LSW(x)       ((x) & 0xffff)

/**
 * @brief 交换32-bit整数的高位word和低位word
 *
 * @par 示例
 * @code
 *  uint32_t a = 0x12345678;
 *  uint32_t b;
 *
 *  b = WORDSWAP(a); //b=0x56781234
 * @endcode
 */
#define WORDSWAP(x)  (MSW(x) | (LSW(x) << 16))

/**
 * @brief 交换32-bit整数的字节顺序
 *
 * @par 示例
 * @code
 *  uint32_t a = 0x12345678;
 *  uint32_t b;
 *
 *  b = LONGSWAP(a); //b=0x78563412
 * @endcode
 */
#define LONGSWAP(x)  ((LLSB(x) << 24) | \
                        (LNLSB(x) << 16) | \
                        (LNMSB(x) << 8)  | \
                        (LMSB(x)))

#define LLSB(x)      ((x) & 0xff)            /**< @brief 取32bit整数第1个字节 */
#define LNLSB(x)     (((x) >> 8) & 0xff)     /**< @brief 取32bit整数第2个字节 */
#define LNMSB(x)     (((x) >> 16) & 0xff)    /**< @brief 取32bit整数第3个字节 */
#define LMSB(x)      (((x) >> 24) & 0xff)    /**< @brief 取32bit整数第4个字节 */


#define POINTER_TO_UINT(x) ((uint32_t) (x))
#define UINT_TO_POINTER(x) ((void *) (x))
#define POINTER_TO_INT(x)  ((int32_t) (x))
#define INT_TO_POINTER(x)  ((void *) (x))

#define ARRAY_SIZE(array) ((unsigned long)(sizeof(array) / sizeof((array)[0])))
#define CONTAINER_OF(ptr, type, field) \
    ((type *)(((char *)(ptr)) - offsetof(type, field)))

/* round "x" up/down to next multiple of "align" (which must be a power of 2) */
#define ROUND_UP(x, align)                                   \
    (((unsigned long)(x) + ((unsigned long)align - 1)) & \
     ~((unsigned long)align - 1))
#define ROUND_DOWN(x, align) ((unsigned long)(x) & ~((unsigned long)align - 1))


#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define IS_POW_OF_TWO(x)     ((x != 0) && !(x & (x - 1)))

#ifndef KB
#define KB(x) ((x) << 10)
#endif
#ifndef MB
#define MB(x) (KB(x) << 10)
#endif
#ifndef GB
#define GB(x) (MB(x) << 10)
#endif

#ifndef KHZ
#define KHZ(x) ((x) * 1000)
#endif
#ifndef MHZ
#define MHZ(x) (KHZ(x) * 1000)
#endif

#ifndef INTEGER_LOG2
#ifdef __GNUC__
#define INTEGER_LOG2(x) ((uint32_t) (31u - __builtin_clz((uint32_t)(x))))
#else
#define INTEGER_LOG2(x) (__libem_integer_log2(x))
__STATIC_INLINE
uint32_t __libem_integer_log2 (uint32_t x)
{
    uint32_t p = 0;
    if (x & 0xFFFF0000) { p += 16;  n >>= 16;   }
    if (x & 0xFF00)     { p += 8;   n >>= 8;    }
    if (x & 0xF0)       { p += 4;   n >>= 4;    }
    if (x & 0xC)        { p += 2;   n >>= 2;    }
    if (x & 0x2)        { p++;                  }
    return p;
}
#endif  /* __GNUC__ */
#endif  /* INTEGER_LOG2 */

#endif /* __LIBEM_COMMON_H__ */
