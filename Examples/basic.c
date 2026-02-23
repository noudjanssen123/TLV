
#include "tlv.h"
#include <stdio.h>


size_t create_message(uint8_t *pBuff,size_t len)
{
    TLV_Message_t msg;
    tlv_message_init(&msg,pBuff,len);

    tlv_push_u8(&msg,0xA0,0x12);
    tlv_push_u32(&msg,0xA1,0x12345678);

    return tlv_size(&msg);
}

void parse_message(uint8_t *pBuff,size_t len)
{
    TLV_Message_t msg;
    tlv_message_init(&msg,pBuff,len);

    // Read as long as their are bytes available
    while (tlv_read_available(&msg))
    {
        // Read a node, and print it out
        TLV_Node_t *node = tlv_read(&msg);
        printf("0x%02X -> %hhi Bytes\n",node->tag,node->len);
    }

}

int main(int argc, char const *argv[])
{
    // Create a message
    uint8_t buff[10];

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
