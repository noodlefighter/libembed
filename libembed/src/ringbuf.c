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

#include "ringbuf.h"
#include "string.h"

/******************************************************************************/
error_t ringbuf_init (ringbuf_t *p_ringbuf,
                      uint32_t   item_size,
                      void      *p_buffer,
                      uint32_t   buffer_size)
{
    if (buffer_size % item_size != 0) {
        return -EINVAL;
    }

    p_ringbuf->item_size = item_size;
    p_ringbuf->size      = buffer_size;
    p_ringbuf->p_buf     = p_buffer;

    p_ringbuf->head      = 0;
    p_ringbuf->tail      = 0;
//    p_ringbuf->count     = 0;

    if (IS_POW_OF_TWO(buffer_size)) {
        p_ringbuf->mask = buffer_size - 1;
    } else {
        p_ringbuf->mask = 0;
    }

    return 0;
}

/******************************************************************************/
error_t ringbuf_insert (ringbuf_t  *p_ringbuf,
                        void       *p_data)
{
    error_t ret = 0;

    if (ringbuf_space_get(p_ringbuf) >= p_ringbuf->item_size) {
        /* copy */
        memcpy(&p_ringbuf->p_buf[p_ringbuf->tail], p_data, p_ringbuf->item_size);

        /* modify tail */
        if (p_ringbuf->mask) {
            p_ringbuf->tail = p_ringbuf->mask & (p_ringbuf->tail + p_ringbuf->item_size);
        } else {
            p_ringbuf->tail += p_ringbuf->item_size;
            if (p_ringbuf->tail > p_ringbuf->size) {
                p_ringbuf->tail -= p_ringbuf->size;
            }
        }
    } else {
        ret = -EMSGSIZE;
    }

    return ret;
}

/******************************************************************************/
error_t ringbuf_insert_multi (ringbuf_t  *p_ringbuf,
                              void       *p_data,
                              uint32_t    length)
{
    uint32_t remain_space, need_space;
    error_t  ret = 0;

    need_space   = length * p_ringbuf->item_size;
    remain_space = ringbuf_space_get(p_ringbuf);

    if (remain_space >= need_space) {
        /*
         * In this situation, if (size-tail < need_space),
         * need to use memcpy() twice. (Unlikely)
         *
         * 0   head    tail    size
         * |     ||||||||       |
         *
         */
        if (!((p_ringbuf->head < p_ringbuf->tail) &&
              (p_ringbuf->size - p_ringbuf->tail < need_space)))
        {
            /* copy once */
            memcpy(&p_ringbuf->p_buf[p_ringbuf->tail], p_data, need_space);
        } else {
            /* copy twice */
            uint32_t right_remain_space = p_ringbuf->size - p_ringbuf->tail;
            memcpy(&p_ringbuf->p_buf[p_ringbuf->tail], p_data, right_remain_space);
            memcpy(&p_ringbuf->p_buf[0], p_data, need_space - right_remain_space);
        }

        /* modify tail */
        if (p_ringbuf->mask) {
            p_ringbuf->tail = p_ringbuf->mask & (p_ringbuf->tail + need_space);
        } else {
            p_ringbuf->tail += need_space;
            if (p_ringbuf->tail > p_ringbuf->size) {
                p_ringbuf->tail -= p_ringbuf->size;
            }
        }
    } else {
        ret = -EMSGSIZE;
    }

    return ret;
}

/******************************************************************************/
error_t ringbuf_pop (ringbuf_t  *p_ringbuf,
                     void       *p_buf)
{
    error_t  ret = 0;

    if (ringbuf_used_get(p_ringbuf) >= p_ringbuf->item_size) {
        /* cpoy */
        memcpy(p_buf, &p_ringbuf->p_buf[p_ringbuf->head], p_ringbuf->item_size);

        /* modify head */
        if (p_ringbuf->mask) {
            p_ringbuf->head = p_ringbuf->mask & (p_ringbuf->head + p_ringbuf->item_size);
        } else {
            p_ringbuf->head += p_ringbuf->item_size;
            if (p_ringbuf->head > p_ringbuf->size) {
                p_ringbuf->head -= p_ringbuf->size;
            }
        }
    } else {
        ret = -EAGAIN;
    }

    return ret;

}

/******************************************************************************/
error_t ringbuf_pop_multi (ringbuf_t  *p_ringbuf,
                           void       *p_buf,
                           uint32_t    length)
{
    uint32_t pop_space;
    error_t  ret = 0;

    pop_space = p_ringbuf->item_size * length;
    if (ringbuf_used_get(p_ringbuf) >= pop_space) {
        /*
         * In this situation, if (size-head < pop_space),
         * need to use memcpy() twice. (Unlikely)
         *
         * 0   tail    head    size
         * ||||||       |||||||||
         *
         */
        if (!((p_ringbuf->tail < p_ringbuf->head) &&
              (p_ringbuf->size - p_ringbuf->head < pop_space)))
        {
            /* copy once */
            memcpy(p_buf, &p_ringbuf->p_buf[p_ringbuf->head], pop_space);
        } else {
            /* copy twice */
            uint32_t right_used_space = p_ringbuf->size - p_ringbuf->head;
            memcpy(p_buf, &p_ringbuf->p_buf[p_ringbuf->head], right_used_space);
            memcpy(p_buf, &p_ringbuf->p_buf[0], pop_space - right_used_space);
        }

        /* modify head */
        if (p_ringbuf->mask) {
            p_ringbuf->head = p_ringbuf->mask & (p_ringbuf->head + pop_space);
        } else {
            p_ringbuf->head += pop_space;
            if (p_ringbuf->head > p_ringbuf->size) {
                p_ringbuf->head -= p_ringbuf->size;
            }
        }
    } else {
        ret = -EMSGSIZE;
    }

    return ret;
}
