/**
 * @file
 * @brief 
 *     
 *     details...
 *
 * @internal
 * @par Modification history
 * - 1.00 2016年10月26日  noodlefighter, first implementation
 * @endinternal
 */

#ifndef __MSM_H
#define __MSM_H

#include <stdint.h>

#include "_libem_config.h"  //todo: developing

typedef enum {

    MSM_RET_UNHANDLED,

    MSM_RET_HANDLED,

    MSM_RET_TRAN

} msm_handler_ret_t;

typedef msm_handler_ret_t (*msm_handler_t)(msm_state_t *p_this);

/** @brief MSM state type definition */
typedef struct msm_state {
    struct msm_state  *p_sup_state;  /**< @brief super state */
    msm_handler_t      p_hdler;      /**< @brief general event handler */
    msm_handler_t      p_init;       /**< @brief initial event handler */
    msm_handler_t      p_entry;      /**< @brief entry   event handler */
    msm_handler_t      p_exit;       /**< @brief exit    event handler */
} msm_state_t;

#if   (LIBEMBED_CFG_MSM_EVT_SIZE == 2)
typedef uint8_t msm_evt_num_t;
typedef uint8_t msm_evt_arg_t;
#elif (LIBEMBED_CFG_MSM_EVT_SIZE == 4)
typedef uint16_t msm_evt_num_t;
typedef uint16_t msm_evt_arg_t;
#elif (LIBEMBED_CFG_MSM_EVT_SIZE == 8)
typedef uint32_t msm_evt_num_t;
typedef uint32_t msm_evt_arg_t;
#else
#error LIBEMBED_CFG_MSM_EVT_SIZE value error!
#endif

typedef struct {
    msm_evt_num_t evt_num;
    msm_evt_arg_t arg;
} msm_evt_t;

typedef struct {
    msm_state_t       *p_state;       /**< @brief current state */

    /** @brief temprory variable, for reducing copy operation */
    union {
        msm_state_t   *p_state;
        msm_handler_t  p_hdler;
    } temp;

    msm_evt_t

} msm_t;

#ifdef __cplusplus
extern "C"
{
#endif




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MSM_H_ */
