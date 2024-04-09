Well, just looking at 

```C
struct exp
    {
    uint8_t ReadA[2];
    uint8_t ReadB[2];
    GPIO_TypeDef* port;
    uint16_t pinmask;
    uint8_t Arec[1];
    uint8_t Brec[1];
    } exps[2];
}
```

you see two `}` and just one `{` so it is no surprise the compiler does not understand this as valid code. What would be the type of `exps[]`? Or the members of `exp`?

If your `struct` is just what it seems to be, there is a 4-byte header for each one of the structs in the array.

## See this example: ##

```C
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
```

### output ###
```none
    A 64-bit value: 0x0807060504030201

Using 0x01020304 to test    Buffer:
       32-bit dword = [ 0x01020304 ]
    As 16-bit words = [ 0x0304, 0x0102 ]
    As 8-bit bytes =  [ 0x04, 0x03, 0x02, 0x01 ]

Changing values for header in op[19]    Buffer:
       32-bit dword = [ 0x41134112 ]
    As 16-bit words = [ 0x4112, 0x4113 ]
    As 8-bit bytes =  [ 0x12, 0x41, 0x13, 0x41 ]
```

Sure, as told in the comments, you can not assign values to whole arrays at runtime, but we are talking here of a mere 32-bit entity and you can assign values to that, for sure.

See the example above, that uses a union to remap the area as 32, 2x16 or 4x8 bytes, in order to be able to assign values at any internal byte. 



