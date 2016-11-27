/**
 * @file
 * @brief       Libembed Config Template
 * @details
 *         "libem_config.h" must be existent if you include any header file
 *     from Libembed. You may want a default config, just create
 *     "libem_config.h" in your directory and add a include seacher path,
 *     and include this file, just like it:
 *          #include "_libem_config.h"
 *
 *
 *     LIBEMBED_SRCCFG_*
 *
 * @internal
 * @par Modification history
 * - 1.00 2016-10-17  noodlefighter, first implementation
 * @endinternal
 */

#ifndef __LIBEMBED_CONFIG_H
#define __LIBEMBED_CONFIG_H


#define LIBEMBED_CFG_USE_TINY_PRINTF               1

#define LIBEMBED_CFG_DEBUG_INFO_ENABLE             1


/******************************************************************************/
/**
 *
 * todo: doxygen comment format
 * @name        Libembed Config
 * @{
 */
#define LIBEMBED_SRCCFG_INLINE_ENABLE              1
#define LIBEMBED_SRCCFG_LOCAL_FUNC_ENABLE          1

#define LIBEMBED_SRCCFG_WORD_LENGTH                32

/** @} */

/**
 *
 * todo: doxygen comment format
 * @name        MSM Config
 * @{
 */

/**
 * @brief   size of msm_event_t
 * @details 2: { uint8_t  event_number, uint8_t  arg }
 *          4: { uint16_t event_number, uint16_t arg }
 *          8: { uint32_t event_number, uint32_t arg }
 */
#ifndef LIBEMBED_SRCCFG_MSM_EVT_SIZE
#define LIBEMBED_SRCCFG_MSM_EVT_SIZE               2
#endif

#ifndef LIBEMBED_SRCCFG_MSM_DEPTH
#define LIBEMBED_SRCCFG_MSM_DEPTH                  5
#endif
/** @} */



#endif /* __LIBEMBED_CONFIG_H */
