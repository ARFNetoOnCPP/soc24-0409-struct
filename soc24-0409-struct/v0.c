#include <stdint.h>
#include <stdio.h>

#define Op4112 0x4112
#define Op4113 0x4113

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

int dump_value(const Op*, const char*);

int main(void)
{
    uint64_t Iam64 = 0x0102030405060708;
    uint8_t* byte  = (uint8_t*)&Iam64;
    printf("    A 64-bit value: 0x");
    for (size_t i = 0; i < 8; i += 1)
        printf("%02x", (int)byte[i]);
    printf("\n");

    Set my_set;
    my_set.op[0].Header.dword = 0x01020304;
    dump_value(
        &my_set.op[0], "\nUsing 0x01020304 to test    ");
    // set values for op[19] 1st word as 16-bit values
    my_set.op[19].Header.word[0] = Op4112;
    // set values for op[19] 2nd word as 8-bit values
    my_set.op[19].Header.byte[2] = 0x41;
    my_set.op[19].Header.byte[3] = 0x13;
    
    dump_value(
        &my_set.op[19],
        "\nChanging values for header in op[19]    ");
    return 0;
}

int dump_value(const Op* op, const char* msg)
{
    if (msg != NULL) printf("%s", msg);
    printf(
        "Buffer:\n\
       32-bit dword = [ 0x%08x ]\n\
    As 16-bit words = [ 0x%04X, 0x%04X ]\n\
    As 8-bit bytes =  [ 0x%02X, 0x%02X, 0x%02X, 0x%02X ]\n ",
        op->Header.dword, op->Header.word[0],
        op->Header.word[1], op->Header.byte[0],
        op->Header.byte[1], op->Header.byte[2],
        op->Header.byte[3]);
    return 0;
}
