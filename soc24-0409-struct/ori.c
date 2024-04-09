#include <stdint.h>

typedef uint32_t GPIO_TypeDef; // any

struct exp
{
    uint8_t       ReadA[2];
    uint8_t       ReadB[2];
    GPIO_TypeDef* port;
    uint16_t      pinmask;
    uint8_t       Arec[1];
    uint8_t       Brec[1];
} exps[2];
}

