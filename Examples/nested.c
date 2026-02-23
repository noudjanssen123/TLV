
#include "tlv.h"
#include <stdio.h>

size_t create_message(uint8_t *pBuff,size_t len)
{
    static uint16_t sequence_number = 0;
    
    TLV_Message_t msg;
    tlv_message_init(&msg,pBuff,len);

    if (tlv_nest(&msg,0xAA))
    {
        tlv_push_u16(&msg,0x01,sequence_number++);

        uint32_t ip = 0;
        ip |= 127 << 24;
        ip |= 0 << 16;
        ip |= 0 << 8;
        ip |= 1 << 0;

        tlv_push_u32(&msg,0x02,ip);

        if (tlv_nest(&msg,0xB0)) {
            
            tlv_push_u24(&msg,0x03,0xF0F1F2);
            
            tlv_unnest(&msg);
        }

        tlv_unnest(&msg);
    }

    tlv_unnest_recursive(&msg);
    return tlv_size(&msg);
}

void parse_message(uint8_t *pBuff,size_t len)
{
    static int depth = 0;
    TLV_Message_t msg;
    tlv_message_init(&msg,pBuff,len);

    while (tlv_read_available(&msg))
    {
        TLV_Node_t *node = tlv_read(&msg);

        for (size_t i = 0; i < depth; ++i)
        {
            printf("\t");
        }
        printf("Node: 0x%02X (%hhi bytes)\n",node->tag,node->len);

        // All nodes with a tag larger then 0xA0 is expected to have nested nodes
        if (node->tag >= 0xA0)
        {
            depth++;
            parse_message(node->value,node->len);
            depth--;
        }
    }

}

int main(int argc, char const *argv[])
{
    // Create a message
    uint8_t buff[255];

    size_t msgLen = create_message(buff,sizeof(buff));

    printf("Created message (%li bytes):\n",msgLen);
    for (size_t i = 0; i < msgLen; ++i)
    {
        printf("[%li] = 0x%02X\t",i,buff[i]);
    }
    printf("\n");

    parse_message(buff,msgLen);
    return 0;
}
