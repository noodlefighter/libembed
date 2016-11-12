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

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief rotary encoder structure, initialze it before using */
typedef struct {
    uint8_t sig_msb;    /**< @brief Most Significant Bit of signal input */
    uint8_t sig_prior;  /**< @brief prior signals */
    int8_t  state;
} rotary_encoder_t;

#define ROTARY_ENCODER_NONE         (0)
#define ROTARY_ENCODER_POSITIVE     (1)
#define ROTARY_ENCODER_INVERSION    (2)

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
uint8_t rotary_encoder_init (rotary_encoder_t *p_this, uint8_t signal_number);

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
