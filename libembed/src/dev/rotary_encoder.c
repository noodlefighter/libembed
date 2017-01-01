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

#define __TRIG_CONFIRM_COUNT_MAX   (2u)
#define __FREE_CONFIRM_COUNT_MAX   (1u)

/* software timer */
__LOCAL
void __softimer_set (rotary_encoder_t *p_this, uint8_t enable) {
    if (enable) {
        p_this->is_softimer_on = 1;
    } else {
        p_this->is_softimer_on = 0;
    }
}

enum {
    __EVT_STATE_ENTRY = 200,
    __EVT_STATE_EXIT,
};


/* state machine definition macros */
#define __STATE(name)                                               \
    __LOCAL void __STATE_##name (rotary_encoder_t *p_this,          \
                                 uint8_t           event)

/* state machine transit macros, execute exit&entry action */
#define __TRAN(p_this, next_state)                                  do      \
        {                                                                   \
            p_this->pfn_state(p_this, __EVT_STATE_EXIT);                    \
            p_this->pfn_state = __STATE_##next_state;                       \
            p_this->pfn_state(p_this, __EVT_STATE_ENTRY);                   \
        }                                                           while(0)

#if (LIBEMBED_SRCCFG_DEBUG)
#define __EVT_IN(p_this, event)                                     do      \
        {                                                                   \
            dbginfoln("rotary_encoder: " #event);                           \
            p_this->pfn_state(p_this, event);                               \
        }                                                           while(0)

#else
#define __EVT_IN(p_this, event)                                     do      \
        {                                                                   \
            if (NULL != p_this->pfn_state) {                                \
                (p_this->pfn_state(p_this, event));                         \
            }                                                               \
        }                                                           while(0)

#endif

__STATE (FREE);
__STATE (A_PRE_TRIGGER);
__STATE (A_TRIGGERED);
__STATE (A_PRE_FREE);
__STATE (B_PRE_TRIGGER);
__STATE (B_TRIGGERED);
__STATE (B_PRE_FREE);

__STATE (FREE)
{
    switch (event) {
    case ROTARY_ENCODER_EVT_A_FALLING:
        if (p_this->b_level) {
            __TRAN(p_this, A_PRE_TRIGGER);
        }
        break;

    case ROTARY_ENCODER_EVT_B_FALLING:
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
    case ROTARY_ENCODER_EVT_TIMER_OVERFLOW:
        if (!p_this->a_level) {
            p_this->count++;
            if (p_this->count >= __TRIG_CONFIRM_COUNT_MAX) {
                __TRAN(p_this, A_TRIGGERED);
            }
        } else {
            __TRAN(p_this, FREE);
        }
        break;

    case __EVT_STATE_ENTRY:
        p_this->count = 0;
        p_this->pfn_timer_set(p_this, 1);
        break;

    case __EVT_STATE_EXIT:
        p_this->pfn_timer_set(p_this, 0);
        break;

    default:
        break;
    }
}

__STATE (A_TRIGGERED)
{
    switch (event) {
    case __EVT_STATE_ENTRY:
        p_this->pfn_trigger(p_this, ROTARY_ENCODER_TRIGGER_INVERSION);
        break;

    case ROTARY_ENCODER_EVT_A_RASING:
        __TRAN(p_this, A_PRE_FREE);
        break;

    default:
        break;
    }
}

__STATE (A_PRE_FREE)
{
    switch (event) {
    case ROTARY_ENCODER_EVT_TIMER_OVERFLOW:
        if (p_this->b_level) {
            p_this->count++;
            if (p_this->count >= __FREE_CONFIRM_COUNT_MAX) {
                __TRAN(p_this, FREE);
            }
        } else {
            p_this->count = 0;
        }
        break;

    case __EVT_STATE_ENTRY:
        p_this->count = 0;
        p_this->pfn_timer_set(p_this, 1);
        break;

    case __EVT_STATE_EXIT:
        p_this->pfn_timer_set(p_this, 0);
        break;

    default:
        break;
    }
}

__STATE (B_PRE_TRIGGER)
{
    switch (event) {
    case ROTARY_ENCODER_EVT_TIMER_OVERFLOW:
        if (!p_this->b_level) {
            p_this->count++;
            if (p_this->count >= __TRIG_CONFIRM_COUNT_MAX) {
                __TRAN(p_this, B_TRIGGERED);
            }
        } else {
            __TRAN(p_this, FREE);
        }
        break;

    case __EVT_STATE_ENTRY:
        p_this->count = 0;
        p_this->pfn_timer_set(p_this, 1);
        break;

    case __EVT_STATE_EXIT:
        p_this->pfn_timer_set(p_this, 0);
        break;

    default:
        break;
    }
}

__STATE (B_TRIGGERED)
{
    switch (event) {
    case __EVT_STATE_ENTRY:
        p_this->pfn_trigger(p_this, ROTARY_ENCODER_TRIGGER_POSITIVE);
        break;

    case ROTARY_ENCODER_EVT_A_RASING:
        __TRAN(p_this, B_PRE_FREE);
        break;

    default:
        break;
    }
}

__STATE (B_PRE_FREE)
{
    switch (event) {
    case ROTARY_ENCODER_EVT_TIMER_OVERFLOW:
        if (p_this->a_level) {
            p_this->count++;
            if (p_this->count >= __FREE_CONFIRM_COUNT_MAX) {
                __TRAN(p_this, FREE);
            }
        } else {
            p_this->count = 0;
        }
        break;

    case __EVT_STATE_ENTRY:
        p_this->count = 0;
        p_this->pfn_timer_set(p_this, 1);
        break;

    case __EVT_STATE_EXIT:
        p_this->pfn_timer_set(p_this, 0);
        break;

    default:
        break;
    }
}

/******************************************************************************/
int  rotary_encoder_init (rotary_encoder_t             *p_this,
                          rotary_encoder_trigger_cb_t   pfn_trigger,
                          rotary_encoder_timer_set_cb_t pfn_timer_set)
{

    if (NULL == pfn_trigger) {
        return -1;
    }

    p_this->pfn_state       = __STATE_FREE;
    p_this->pfn_trigger     = pfn_trigger;
    p_this->pfn_timer_set   = pfn_timer_set;
    p_this->sig_prior       = 0xff;
    p_this->is_softimer_on  = 0;

    if (NULL == p_this->pfn_timer_set) {
        p_this->pfn_timer_set = __softimer_set;
    }

    return 0;
}

void rotary_encoder_event_import (rotary_encoder_t       *p_this,
                                  rotary_encoder_event_t  event,
                                  uint8_t                 signals)
{
    p_this->a_level = signals & BIT(0);
    p_this->b_level = signals & BIT(1);
    __EVT_IN(p_this, event);
}

void rotary_encoder_signals_import (rotary_encoder_t  *p_this,
                                    uint8_t            signals)
{
    uint8_t sig_trig;

#if (LIBEMBED_SRCCFG_CHKPARAM)
    if (NULL == p_this) return;
#endif

    /* falling edge&rasing edge trigger calc */
    sig_trig = p_this->sig_prior ^ signals;

    /* record sig */
    p_this->sig_prior = signals;

    /* 500us 软件定时器 */
    if (p_this->is_softimer_on) {
        __EVT_IN(p_this, ROTARY_ENCODER_EVT_TIMER_OVERFLOW);
    }
    if (0x00 == sig_trig) {
        return;
    } else {
        p_this->a_level = signals & BIT(0);
        p_this->b_level = signals & BIT(1);

        if (sig_trig & BIT(0)) {
            if (p_this->a_level) {
                __EVT_IN(p_this, ROTARY_ENCODER_EVT_A_RASING);
            } else {
                __EVT_IN(p_this, ROTARY_ENCODER_EVT_A_FALLING);
            }

        }

        if (sig_trig & BIT(1)) {
            if (p_this->b_level) {
                __EVT_IN(p_this, ROTARY_ENCODER_EVT_B_RASING);
            } else {
                __EVT_IN(p_this, ROTARY_ENCODER_EVT_B_FALLING);
            }

        }
    }
}


