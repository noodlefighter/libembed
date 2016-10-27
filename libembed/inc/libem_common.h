/**
 * @file
 * @brief   Libembed Common File
 * @internal
 * @par Modification history
 * - 1.00 2016-10-17  noodlefighter, first implementation
 * @endinternal
 */

#ifndef __LIBEM_COMMON_H
#define __LIBEM_COMMON_H

#include <stdint.h>

#include "_libem_config.h"   /** from user dir */

/**
 * @name keyword redefine for unity test and widely compiler support
 * @{
 */

#define __STATIC_INLINE static inline  /**< @brief static inline func */
#define __INLINE        inline         /**< @brief inline func */

#define __LOCAL         static         /**< @brief local func */

#define __ROM           const          /**< @brief data stored in ROM */
#define __ROM_BYTE(n)   (n)            /**< @brief read byte stored in ROM */
#define __ROM_PTR(n)    (n)            /**< @brief read pointer stored in ROM */

/** @} */


#endif /* __LIBEM_COMMON_H */
