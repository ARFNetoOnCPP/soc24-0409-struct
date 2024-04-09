#include <stdint.h>
#include <stdio.h>

typedef uint32_t GPIO_TypeDef;  // any

typedef struct
{
    union
    {
        uint8_t  byte[4];
        uint16_t word[2];
        uint32_t dword;
    } Header;
    GPIO_TypeDef* port;
    uint16_t      pinmask;
    uint8_t       Arec[1];
    uint8_t       Brec[1];
} Op;

typedef struct
{
    Op op[20];
} Set;

int main(void)
{
    Set my_set;
    my_set.op[0].Header.dword = 0x01020304;
    printf(
        "\n\
    As 32-bit dword = [ 0x%08x ]\n\
    As 16-bit words = [ 0x%04X, 0x%04X ]\n\
    As 8-bit bytes =  [ 0x%02X, 0x%02X, 0x%02X, 0x%02X ]\n ",
        my_set.op[0].Header.dword,
        my_set.op[0].Header.word[0],
        my_set.op[0].Header.word[1],
        my_set.op[0].Header.byte[0],
        my_set.op[0].Header.byte[1],
        my_set.op[0].Header.byte[2],
        my_set.op[0].Header.byte[3]);
    return 0;
}
