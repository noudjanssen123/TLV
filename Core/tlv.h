/*
 * MIT License
 * 
 * Copyright (c) 2026 noudjanssen123
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TLV_H_
#define TLV_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define TLV_MAX_DEPTH 10

#define TLV_TAG_SIZE sizeof(uint8_t)
#define TLV_LEN_SIZE sizeof(uint8_t)
#define TLV_HEAD_SIZE (TLV_TAG_SIZE + TLV_LEN_SIZE)

typedef struct {
    uint8_t tag;
    const uint8_t len;
    uint8_t value[];
} TLV_Node_t;

typedef struct {
    size_t index[TLV_MAX_DEPTH];
    size_t depth;
    uint8_t *pBuffer;
    size_t buffer_size;
} TLV_Message_t;

bool tlv_message_init(TLV_Message_t *msg,uint8_t *pBuff,size_t len);

/**
 * @brief Check if there is a message available to read.
 * 
 * @param msg 
 * @return true 
 * @return false 
 */
bool tlv_read_available(TLV_Message_t *msg);
/**
 * @brief Read the next available node if available.
 * 
 * @param msg 
 * @return TLV_Node_t* 
 */
TLV_Node_t *tlv_read(TLV_Message_t *msg);

size_t tlv_size(TLV_Message_t *msg);

bool tlv_nest(TLV_Message_t *msg,uint8_t tag);
void tlv_unnest(TLV_Message_t *msg);
void tlv_unnest_recursive(TLV_Message_t *msg);

TLV_Node_t *tlv_alloc(TLV_Message_t *msg,size_t len);


/* ==== Helpers ==== */
bool tlv_push_u8(TLV_Message_t *msg,uint8_t tag, uint8_t value);
bool tlv_push_u16(TLV_Message_t *msg,uint8_t tag, uint16_t value);
bool tlv_push_u24(TLV_Message_t *msg,uint8_t tag, uint32_t value);
bool tlv_push_u32(TLV_Message_t *msg,uint8_t tag, uint32_t value);
bool tlv_push_u64(TLV_Message_t *msg,uint8_t tag, uint64_t value);


#endif // TLV_H_