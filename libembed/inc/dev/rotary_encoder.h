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

#ifndef ROTARY_ENCODER_H_
#define ROTARY_ENCODER_H_

#include <stdint.h>     /* C99 integer typedef */
#include "libem_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief rotary encoder structure, initialze it before using */
typedef struct {
    uint8_t  sig_prior;

    uint8_t  a_level;
    uint8_t  b_level;

    uint8_t  count;
    void   (*pfn_state) (rotary_encoder_t *p_this, uint8_t sig);

} rotary_encoder_t;

#define ROTARY_ENCODER_NONE         (0u)
#define ROTARY_ENCODER_POSITIVE     (1u)
#define ROTARY_ENCODER_INVERSION    (2u)

/**
 * @brief       initialize the rotary encoder structure
 *
 * @details
 *
 * @param[in]   p_this          rotary encoder instance
 * @param[in]   signal_number   number of signal, 2 <= signal_number <=8
 *
 * @return      0           succeed
 * @return      !0          fail
 */
uint8_t rotary_encoder_init (rotary_encoder_t *p_this,
                             void             (*pfn_timer_start) (void),
                             void             (*pfn_timer_stop)  (void),
                             void             (*pfn_cb_trigger)  (int8_t));

/**
 * @brief       signals import
 *
 * @details     import values of signals, when all signals be triggered,
 *              return the rotary encoder's rolling direction.
 *
 * @param[in]   p_this      rotary encoder instance
 * @param[in]   signals     rotary encoder signals import
 *                          bit0 is for Signal A, bit1 is for Signal B...
 *                          by parity of reasoning
 *
 * @retval      ROTARY_ENCODER_NONE         none
 * @retval      ROTARY_ENCODER_POSITIVE     positive
 * @retval      ROTARY_ENCODER_INVERSION    inversion
 */
uint8_t rotary_encoder_signals_import (rotary_encoder_t  *p_this,
                                       uint8_t            signals);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ROTARY_ENCODER_H_ */
