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

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "_libem_config.h"  //todo: developing
#include "libem_common.h"


#define MSM_RET_UNHANDLED   0u      /**< @brief  */
#define MSM_RET_HANDLED     1u      /**< @brief  */
#define MSM_RET_TRAN        2u      /**< @brief  */

typedef uint8_t msm_hdler_ret_t;

typedef msm_hdler_ret_t (*msm_handler_t)(msm_state_t *p_this);

/** @brief MSM state type definition */
typedef struct msm_state {
    struct msm_state        *p_sup_state;  /**< @brief super state */
    const msm_handler_t      p_hdler;      /**< @brief general event handler */
    const msm_handler_t      p_init;       /**< @brief initial event handler */
    const msm_handler_t      p_entry;      /**< @brief entry   event handler */
    const msm_handler_t      p_exit;       /**< @brief exit    event handler */
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

/** @brief metal  */
typedef struct {
    const msm_state_t *p_state;     /**< @brief current state */

    /** @brief temprory variable for importing some variable */
    union {
        msm_state_t   *p_state;
        msm_handler_t  p_hdler;
    } temp;

    msm_evt_t          cur_evt;     /** @brief current event */
} msm_t;

/**
 * @brief   process current event
 * @details current event import from p_this->cur_evt
 * @note    internal function, please use msm_evt_in instead
 */
void msm_evt_process_ (msm_t *p_this);

/**
 * @brief   import event into msm
 */
__STATIC_INLINE
void msm_evt_in (msm_t *p_this, msm_evt_num_t evt_num, msm_evt_arg_t arg)
{
    p_this->cur_evt.evt_num = evt_num;
    p_this->cur_evt.arg     = arg;
    msm_evt_process_(p_this);
}

#define MSM_EVT_ENTRY       ((msm_evt_num_t) 0)
#define MSM_EVT_EXIT        ((msm_evt_num_t) 1)
#define MSM_EVT_INITIAL     ((msm_evt_num_t) 2)
#define MSM_EVT_USER        ((msm_evt_num_t) 3)

/** @brief state transition macros used in event handler */
#define MSM_TRAN(p_this, p_state) \
            ((p_this)->tmp.p_state = (p_state), MSM_RET_TRAN)

int msm_init (msm_t *p_this, msm_handler_t *p_action_init);

int msm_start (msm_t *p_this);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MSM_H_ */
