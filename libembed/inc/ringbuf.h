/**
 * @file
 * @brief
 *
 *     details...
 *
 * @internal
 * @par Modification history
 * - 1.00 2016-11-20  noodlefighter
 * @endinternal
 */

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include "libem_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A structure to represent a ring buffer
 */
typedef struct {
    uint32_t  item_size;       /**< size of item */
    uint32_t  size;            /**< size of buffer */
    uint8_t  *p_buf;           /**< pointer to buffer */

    uint32_t  head;            /**< index of head item in p_buf */
    uint32_t  tail;            /**< index of tail item in p_buf */
//    uint32_t  count;           /**< counter of item in buffer */
    uint32_t  mask;            /**< mask modulo mask if size is a power of 2 */
} ringbuf_t;

/**
 * @brief Initialize a ring buffer
 *
 * For optimal performance, use buffer size(item_size*length) that are a power
 * of 2 as they don't require determine operations when maintaining the buffer.
 *
 * @param[in,out] p_ringbuf     Ring buffer to initialize
 * @param[in]     item_size     Size of item
 * @param[in]     buffer_size   Size of buffer (byte)
 * @param[in]     p_buffer      pointer to buffer
 */
error_t ringbuf_init (ringbuf_t *p_ringbuf,
                      uint32_t   item_size,
                      void      *p_buffer,
                      uint32_t   buffer_size);

/**
 * @brief Determine if a ring buffer is empty
 */
__STATIC_INLINE
int ringbuf_is_empty (ringbuf_t *p_ringbuf)
{
	return (p_ringbuf->head == p_ringbuf->tail);
}

/**
 * @brief Obtain available space in a ring buffer
 */
__STATIC_INLINE
uint32_t ringbuf_space_get (ringbuf_t *p_ringbuf)
{
	if (ringbuf_is_empty(p_ringbuf)) {
		return p_ringbuf->size;
	}

	if (p_ringbuf->tail < p_ringbuf->head) {
		return p_ringbuf->head - p_ringbuf->tail;
	}

	/* p_ringbuf->tail > p_ringbuf->head */
	return (p_ringbuf->size - p_ringbuf->tail) + p_ringbuf->head;
}

/**
 * @brief Obtain used space in a ring buffer
 */
__STATIC_INLINE
uint32_t ringbuf_used_get (ringbuf_t *p_ringbuf)
{
    if (ringbuf_is_empty(p_ringbuf)) {
        return 0;
    }

    if (p_ringbuf->tail < p_ringbuf->head) {
        return (p_ringbuf->size - p_ringbuf->head) + p_ringbuf->tail;
    }

    /* p_ringbuf->tail > p_ringbuf->head */
    return p_ringbuf->tail - p_ringbuf->head;
}

/**
 * @brief Insert a item into ring buffer.
 *
 * @param[in,out] p_ringbuf     Ring buffer to insert data to
 * @param[in]     p_data        pointer to item
 *
 * @return 0 on success, -EMSGSIZE if there isn't sufficient space
 */
error_t ringbuf_insert (ringbuf_t  *p_ringbuf,
                        void       *p_data);

/**
 * @brief Insert mulit items into ring buffer.
 *
 * @param[in,out] p_ringbuf     Ring buffer to insert data to
 * @param[in]     p_data        pointer to items
 * @param[in]     length        number of items
 *
 * @return 0 on success, -EMSGSIZE if there isn't sufficient space
 */
error_t ringbuf_insert_multi (ringbuf_t  *p_ringbuf,
                              void       *p_data,
                              uint32_t    length);

/**
 * @brief Pop a item into ring buffer.
 *
 * @param[in,out] p_ringbuf     Ring buffer to insert data to
 * @param[in]     p_buf         pointer to pop buffer
 *
 * @return 0 on success, -EAGAIN if ring buffer is empty
 */
error_t ringbuf_pop (ringbuf_t  *p_ringbuf,
                     void       *p_buf);

/**
 * @brief Pop mulit items into ring buffer.
 *
 * @param[in,out] p_ringbuf     Ring buffer to insert data to
 * @param[in]     p_buf         pointer to pop buffer
 * @param[in]     length        number of items
 *
 * @return 0 on success, -EMSGSIZE if there have no enough items to pop
 */
error_t ringbuf_pop_multi (ringbuf_t  *p_ringbuf,
                           void       *p_buf,
                           uint32_t    length);

#ifdef __cplusplus
}
#endif

#endif /* __RING_BUFFER_H__ */
