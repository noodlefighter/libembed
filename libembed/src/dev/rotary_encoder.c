/**
 * @file
 * @brief 
 *     
 *     details...
 *
 * @internal
 * @par Modification history
 * - 1.00 2016年11月9日  noodlefighter, first implementation
 * @endinternal
 */

#include "dev/rotary_encoder.h"
#include <stddef.h>

#define __TRIG_CONFIRM_COUNT_MAX   (5u)
#define __FREE_CONFIRM_COUNT_MAX   (5u)


#define __EVT_A_FALLING        (0u)
#define __EVT_A_RASING         (1u)
#define __EVT_B_FALLING        (2u)
#define __EVT_B_RASING         (3u)
#define __EVT_TIMER_OVERFLOW   (4u)

enum {
    __SIG_STATE_ENTRY = 200,
    __SIG_STATE_EXIT,
};

/* state machine definition macros */
#define __STATE(name)  __LOCAL void __STATE_##name (rotary_encoder_t *p_this, \
                                                    uint8_t           event)

/* state machine transit macros, execute exit&entry action */
#define __TRAN(p_this, next_state)                                  do      \
        {                                                                   \
            p_this->pfn_state(p_this, __SIG_STATE_EXIT);                    \
            p_this->pfn_state = __STATE_##next_state;                       \
            p_this->pfn_state(p_this, __SIG_STATE_ENTRY);                   \
        }                                                           while(0)


__STATE (FREE)
{
    switch (event) {
    case ROTARY_ENCODER_a_level_FALLING:
        if (p_this->b_level) {
            __TRAN(p_this, A_PRE_TRIGGER);
        }
        break;

    case ROTARY_ENCODER_b_level_FALLING:
        if (p_this->a_level) {
            __TRAN(p_this, B_PRE_TRIGGER);
        }
        break;

    default:
        break;
    }
}

__STATE (A_PRE_TRIGGER)
{
    switch (event) {
    case __EVT_TIMER_OVERFLOW:
        if (p_this->a_level) {
            p_this->count++;
            if (p_this->count >= __TRIG_CONFIRM_COUNT_MAX) {
                __TRAN(p_this, A_TRIGGERED);
            }
        } else {
            __TRAN(p_this, FREE);
        }
        break;

    case __SIG_STATE_ENTRY:
        p_this->count = 0;
        p_this->pfn_timer_start();
        break;

    case __SIG_STATE_EXIT:
        p_this->pfn_timer_stop();
        break;

    default:
        break;
    }
}
__STATE (A_TRIGGERED)
{
    switch (event) {
    case __SIG_STATE_ENTRY:
        p_this->pfn_cb_a_trigger();
        break;

    case ROTARY_ENCODER_a_level_RASING:
        __TRAN(p_this, A_PRE_FREE);
        break;

    default:
        break;
    }
}

__STATE (A_PRE_FREE)
{
    switch (event) {
    case __EVT_TIMER_OVERFLOW:
        if (p_this->b_level) {
            p_this->count++;
            if (p_this->count >= __FREE_CONFIRM_COUNT_MAX) {
                __TRAN(p_this, FREE);
            }
        } else {
            p_this->count = 0;
        }
        break;

    case __SIG_STATE_ENTRY:
        p_this->count = 0;
        p_this->pfn_timer_start();
        break;

    case __SIG_STATE_EXIT:
        p_this->pfn_timer_stop();
        break;

    default:
        break;
    }
}

__STATE (B_PRE_TRIGGER)
{
    switch (event) {
    case __EVT_TIMER_OVERFLOW:
        if (p_this->b_level) {
            p_this->count++;
            if (p_this->count >= __TRIG_CONFIRM_COUNT_MAX) {
                __TRAN(p_this, B_TRIGGERED);
            }
        } else {
            __TRAN(p_this, FREE);
        }
        break;

    case __SIG_STATE_ENTRY:
        p_this->count = 0;
        p_this->pfn_timer_start();
        break;

    case __SIG_STATE_EXIT:
        p_this->pfn_timer_stop();
        break;

    default:
        break;
    }
}
__STATE (B_TRIGGERED)
{
    switch (event) {
    case __SIG_STATE_ENTRY:
        p_this->pfn_cb_b_trigger();
        break;

    case ROTARY_ENCODER_a_level_RASING:
        __TRAN(p_this, B_PRE_FREE);
        break;

    default:
        break;
    }
}

__STATE (B_PRE_FREE)
{
    switch (event) {
    case __EVT_TIMER_OVERFLOW:
        if (p_this->a_level) {
            p_this->count++;
            if (p_this->count >= __FREE_CONFIRM_COUNT_MAX) {
                __TRAN(p_this, FREE);
            }
        } else {
            p_this->count = 0;
        }
        break;

    case __SIG_STATE_ENTRY:
        p_this->count = 0;
        p_this->pfn_timer_start();
        break;

    case __SIG_STATE_EXIT:
        p_this->pfn_timer_stop();
        break;

    default:
        break;
    }
}

/******************************************************************************/
uint8_t rotary_encoder_init (rotary_encoder_t *p_this,
                             void             (*pfn_timer_start) (void),
                             void             (*pfn_timer_stop)  (void),
                             void             (*pfn_cb_trigger)  (int8_t))
{

    p_this->pfn_state = __STATE_FREE;

    return 0;
}

uint8_t rotary_encoder_signals_import (rotary_encoder_t  *p_this,
                                       uint8_t            signals)
{
    uint8_t sig_differ, sig_trig;

#if (LIBEMBED_SRCCFG_CHKPARAM)
    if (NULL == p_this) return ROTARY_ENCODER_NONE;
#endif

    /* difference signals calc */
    sig_differ = p_this->sig_prior ^ signals;

    /*
     * calculate trigger bits, the setting bits are on the falling edge.
     *     e.g. bit0 on falling edge, bit1 on rasing edge
     *       signals               1110
     *       sig_prior             1101
     *       sig_differ            0011
     *       ---------------------------
     *       ~signals & sig_differ 0001
     */
    sig_trig = (uint8_t) ((~signals) & sig_differ);

    p_this->sig_prior = signals;

    //todo: 500us一个采样 可以利用这个时间做软件定时器

    if (0x00 == sig_trig) {
        return ROTARY_ENCODER_NONE;
    } else {

        uint8_t event;
        uint8_t rc = ROTARY_ENCODER_NONE;

        p_this->a_level = signals & BIT(0);
        p_this->b_level = signals & BIT(1);

        if (sig_trig & BIT(0)) {
            if (p_this->a_level) {
                event = __EVT_A_RASING;
            } else {
                event = __EVT_A_FALLING;
            }
            rc |= p_this->pfn_state(p_this, event);
        }

        if (sig_trig & BIT(1)) {
            if (p_this->b_level) {
                event = __EVT_B_RASING;
            } else {
                event = __EVT_B_FALLING;
            }
            rc |= p_this->pfn_state(p_this, event);
        }
        return __decoder(p_this, sig_trig);
    }
}


