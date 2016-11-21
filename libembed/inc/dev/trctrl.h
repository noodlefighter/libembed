/**
 * @file
 * @brief   Transmit&Receive Control
 *
 * @internal
 * @par Modification history
 * - 1.00 2016-11-21  noodlefighter
 * @endinternal
 */

#ifndef __TRCTRL_H__
#define __TRCTRL_H__

#include "libem_common.h"
#include "ringbuf.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    ringbuf_t rx_ringbuf;
    ringbuf_t tx_ringbuf;
} trctrl_t;

/**
 * @brief   Initialize a Transmit&Receive Control
 */
__STATIC_INLINE
error_t trctrl_init (trctrl_t *p_this,
                     uint32_t  rx_buf_length,
                     uint8_t  *p_rx_buf,
                     uint32_t  tx_buf_length,
                     uint8_t  *p_tx_buf)
{
    ringbuf_init(p_this->rx_ringbuf, 1, rx_buf_length, p_rx_buf);
    ringbuf_init(p_this->tx_ringbuf, 1, tx_buf_length, p_tx_buf);
}



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TRCTRL_H__ */
