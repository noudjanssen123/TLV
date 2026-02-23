#include "tlv.h"

#include <stdlib.h>
#include <string.h>

TLV_Node_t *tlv_cast_node(TLV_Message_t *msg,size_t index)
{
    return (TLV_Node_t *)&msg->pBuffer[index];
}

bool tlv_message_init(TLV_Message_t *msg,uint8_t *pBuff,size_t len)
{
    if (msg == NULL || pBuff == NULL)
    {
        return false;
    }
    memset(msg,0,sizeof(TLV_Message_t));

    msg->pBuffer = pBuff;
    msg->buffer_size = len;
    return true;
}

bool tlv_read_available(TLV_Message_t *msg)
{
    if (msg == NULL)
    {
        return false;
    }

    size_t readIndex = tlv_size(msg);

    if (readIndex < msg->buffer_size - TLV_HEAD_SIZE)
    {
        TLV_Node_t *node = tlv_cast_node(msg,readIndex);
        readIndex += node->len;
        return readIndex <= msg->buffer_size;
    }
    return false;
}

TLV_Node_t *tlv_read(TLV_Message_t *msg)
{
    if (tlv_read_available(msg))
    {
        size_t readIndex = tlv_size(msg);
        TLV_Node_t *node = tlv_cast_node(msg,readIndex);
        msg->index[msg->depth] += node->len + TLV_HEAD_SIZE;
        return node;
    }
    return NULL;
}

size_t tlv_size(TLV_Message_t *msg)
{
    if (msg == NULL)
    {
        return 0;
    }
    return msg->index[msg->depth];
}

bool tlv_nest(TLV_Message_t *msg,uint8_t tag)
{
    if (msg == NULL || msg->depth + 1 >= TLV_MAX_DEPTH)
    {
        return false;
    }

    // Set tag
    msg->pBuffer[msg->index[msg->depth]] = tag;
    msg->pBuffer[msg->index[msg->depth] + 1] = 0;

    // copy the index one nest deeper
    msg->index[msg->depth + 1] = msg->index[msg->depth] + 2;
    msg->depth++;
    return true;
}

void tlv_unnest(TLV_Message_t *msg)
{
    if (msg == NULL || msg->depth == 0)
    {
        return;
    }
    // Get the size off all nest allocated nodes 
    const size_t nestSize = tlv_size(msg) - msg->index[msg->depth - 1];
    
    // Decrease the depth
    msg->depth--;
    // Update the "len" byte
    msg->pBuffer[tlv_size(msg) + 1] = nestSize;
    msg->index[msg->depth] += nestSize + TLV_HEAD_SIZE;
}

void tlv_unnest_recursive(TLV_Message_t *msg)
{
    if (msg == NULL)
    {
        return;
    }
    
    while (msg->depth != 0)
    {
        tlv_unnest(msg);
    }
}

TLV_Node_t *tlv_alloc(TLV_Message_t *msg,size_t len)
{
    if (msg == NULL)
    {
        return NULL;
    }

    size_t writeIndex = tlv_size(msg);

    // Check if their is enough room
    if (writeIndex < msg->buffer_size - TLV_HEAD_SIZE)
    {
        // Manually write the "len" byte.
        msg->pBuffer[writeIndex + 1] = len;
        TLV_Node_t *node = tlv_cast_node(msg,writeIndex);
        // Increase the write index
        msg->index[msg->depth] += TLV_HEAD_SIZE + len;
        return node;
    }
    return NULL;
}

bool tlv_push_u8(TLV_Message_t *msg,uint8_t tag, uint8_t value)
{
    if (msg == NULL)
    {
        return false;
    }
    TLV_Node_t *node = tlv_alloc(msg,sizeof(value));
    if (node == NULL)
    {
        return false;
    }
    node->tag = tag;
    node->value[0] = value;
    return true;
}
bool tlv_push_u16(TLV_Message_t *msg,uint8_t tag, uint16_t value)
{
    if (msg == NULL)
    {
        return false;
    }
    TLV_Node_t *node = tlv_alloc(msg,sizeof(value));
    if (node == NULL)
    {
        return false;
    }
    node->tag = tag;
    node->value[0] = value >> 8;
    node->value[1] = value;
    return true;
}
bool tlv_push_u24(TLV_Message_t *msg,uint8_t tag, uint32_t value)
{
    if (msg == NULL)
    {
        return false;
    }
    TLV_Node_t *node = tlv_alloc(msg,3);
    if (node == NULL)
    {
        return false;
    }
    node->tag = tag;
    node->value[0] = value >> 16;
    node->value[1] = value >> 8;
    node->value[2] = value;
    return true;
}
bool tlv_push_u32(TLV_Message_t *msg,uint8_t tag, uint32_t value)
{
    if (msg == NULL)
    {
        return false;
    }
    TLV_Node_t *node = tlv_alloc(msg,sizeof(value));
    if (node == NULL)
    {
        return false;
    }
    node->tag = tag;
    node->value[0] = value >> 24;
    node->value[1] = value >> 16;
    node->value[2] = value >> 8;
    node->value[3] = value;
    return true;
}
bool tlv_push_u64(TLV_Message_t *msg,uint8_t tag, uint64_t value)
{
    if (msg == NULL)
    {
        return false;
    }
    TLV_Node_t *node = tlv_alloc(msg,sizeof(value));
    if (node == NULL)
    {
        return false;
    }
    node->tag = tag;
    node->value[0] = value >> 56;
    node->value[1] = value >> 48;
    node->value[2] = value >> 40;
    node->value[3] = value >> 32;
    node->value[4] = value >> 24;
    node->value[5] = value >> 16;
    node->value[6] = value >> 8;
    node->value[7] = value;
    return true;
}