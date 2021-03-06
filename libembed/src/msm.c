/**
 * @file
 * @brief 
 *     
 *     details...
 *
 * @internal
 * @par Modification history
 * - 1.00 2016年10月28日  noodlefighter, first implementation
 * @endinternal
 */

#include "msm.h"

__LOCAL msm_state_t __msm_initial_dummy_state;

void __msm_exec_tran (msm_t *p_this, const msm_state_t *p_target);

int msm_init (msm_t *p_this, msm_handler_t *p_action_init)
{
    if ((NULL == p_this) || (NULL == p_action_init)) {
        return -1;
    }

    p_this->temp.p_hdler = p_action_init;
    p_this->p_state      = &__msm_initial_dummy_state;
    return 0;
}

int msm_start (msm_t *p_this)
{
    msm_hdler_ret_t ret;

    if ((NULL == p_this) || (NULL == p_this->temp.p_hdler)) {
        return -1;
    }

    ret = p_this->temp.p_hdler(p_this);

    do {
        ret = __msm_exec_tran(p_this, p_this->temp.p_state);
    } while(ret == MSM_RET_TRAN);

    return 0;
}

msm_hdler_ret_t __msm_exec_tran (msm_t *p_this, const msm_state_t *p_target)
{
    uint8_t i, len_s, len_t, len_min;

    msm_hdler_ret_t ret = MSM_RET_HANDLED;
    const msm_state_t *p_stmp;
    const msm_state_t *(p_s[LIBEMBED_CFG_MSM_DEPTH]);
    const msm_state_t *(p_t[LIBEMBED_CFG_MSM_DEPTH]);

    /*
     * Step1. calculate the number of times do exit&entry action.
     */
    if ((p_this->p_state != p_target) &&
        (p_this->p_state->p_sup_state != p_target->p_sup_state))
    {
        /* calulate "chain to top" */
        p_stmp = p_this->p_state;
        for (len_s = 0; NULL != p_stmp; (len_s++, p_stmp = p_stmp->p_sup_state)) {
            p_s[len_s] = p_stmp;
        }

    }



}

void msm_evt_process_ (msm_t *p_this)
{
    msm_hdler_ret_t ret;
    /*
     *  execute handler, until event be handled
     */
    do {
        if (NULL != p_this->p_state->p_hdler) {
            ret = p_this->p_state->p_hdler();
        } else {
            ret = MSM_RET_UNHANDLED;
        }

        if ()

            //todo: writing!!!!!!!!!!!!!!!!!!!!!!!!!!here! so tire !!sleep!!!
    } while();

    do {
        ret = __msm_exec_tran(p_this, p_this->temp.p_state);
    } while(ret == MSM_RET_TRAN);
}

