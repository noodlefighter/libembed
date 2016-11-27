/**
 * @file
 * @brief   Transmit&Receive Flow Control
 *          流控:
 *              接收: 接收buffer将满 开启满信号; ....
 *              发送: 收到对方的将满信号 停止发; 收到对方将空信号 开始发
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
    //todo:
} trctrl_t;

/**
 * @brief   Initialize a Transmit&Receive Control
 * @details
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

/**
 * @brief    TRCTRL Receive Interrupt Service Routines
 * @details  Call this function when recieved completed interrupt occurred,
 *           try to save recieved data to RX buffer.
 * @retval   0              Succeed to saving data to RX buffer
 * @retval   -EMSGSIZE      RX buffer isn't sufficient space
 */
__STATIC_INLINE
error_t trctrl_rx_isr (trctrl_t *p_this, char data)
{
    return ringbuf_insert(&p_this->rx_ringbuf, &data);
}

/**
 * @brief    TRCTRL Transmit Interrupt Service Routines
 * @details  Call this function when transmit completed interrupt occurred,
 *           if TX buffer is not empty
 * @retval   0          Succeed in getting data from TX buffer
 * @retval   -EAGAIN    No more data in TX buffer
 */
__STATIC_INLINE
error_t trctrl_tx_isr (trctrl_t *p_this, char *p_data)
{
    return ringbuf_pop(&p_this->tx_ringbuf, p_data);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TRCTRL_H__ */
