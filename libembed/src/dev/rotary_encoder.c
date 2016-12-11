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

/******************************************************************************/
uint8_t rotary_encoder_init (rotary_encoder_t *p_this, uint8_t signal_number)
{
    /* check parameters */
    if ((NULL == p_this) || (signal_number < 2) || (signal_number > 8))
    {
        return !0;
    }

    /* initialize structure members */
    p_this->sig_msb = (uint8_t) (signal_number - 1);
    p_this->sig_prior = 0xff;

    return 0;
}

/******************************************************************************/
/**
 * @brief       decoder state machine
 * @details
 * @return      ROTARY_ENCODER_*
 */
__LOCAL
uint8_t __decoder (rotary_encoder_t *p_this, uint8_t falling_bits)
{
    int8_t  state   = p_this->state;
    uint8_t sig_msb = p_this->sig_msb;
    uint8_t ret     = ROTARY_ENCODER_NONE;

    if (0 == state) {                             /* STATE: NONE */
        if (falling_bits & BIT(0)) {              /* if signal_0 on falling edge */
            p_this->state = 1;                    /* entry POSITIVE_1 */
        } else if (falling_bits & BIT(sig_msb)) { /* if signal_n on falling edge */
            p_this->state = -1;                   /* entry INVERSION_1 */
        }

    } else if (state > 0) {                       /* STATE: POSITIVE_* */
        if (falling_bits & BIT(state)) {          /* the last signal */
            if (state == sig_msb) {               /* now, the last state */
                ret = ROTARY_ENCODER_POSITIVE;    /* confirm!! */
                p_this->state = 0;                /* entry NONE */
            } else {
                p_this->state++;                  /* entry (POSITIVE_*+1) */
            }
        } else if (falling_bits & BIT(0)) {       /* error */
            p_this->state = 0;                    /* entry NONE */
        }

    } else { /* if (p_state < 0) */               /* STATE: INVERSION_* */
        if (falling_bits &                        /* the last signal */
            BIT((uint8_t)((int8_t)sig_msb + state)))
        {
            if (state == -sig_msb) {              /* now, the last state */
                ret = ROTARY_ENCODER_INVERSION;   /* confirm!! */
                p_this->state = 0;                /* entry NONE */
            } else {
                p_this->state--;                  /* entry (INVERSION_*+1) */
            }
        } else if (falling_bits & BIT(sig_msb)) { /* error */
            p_this->state = 0;                    /* entry NONE */
        }
    }

    return ret;
}

/******************************************************************************/
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
    if (0x00 == sig_trig) {
        return ROTARY_ENCODER_NONE;
    } else {
        return __decoder(p_this, sig_trig);
    }
}
