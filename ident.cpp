#include <stdio.h>
#include <dos.h>

#define BASE 0xD0000

#define ROMADD(X) (volatile unsigned char far *)MK_FP(0xD000, (X))

volatile unsigned char far *unlock1 = ROMADD(0x5555);
volatile unsigned char far *unlock2 = ROMADD(0x2AAA);

void main() {
        *unlock1 = 0xAA; 
        *unlock2 = 0x55;
        *unlock1 = 0x90;
        unsigned char manufacturer = *ROMADD(0x0000);
        unsigned char product = *ROMADD(0x0001);

        *unlock1 = 0xAA;
        *unlock2 = 0x55;
        *unlock1 = 0xF0;

        int chipid = manufacturer << 8 | product;

        switch (chipid) {
                case 0xbfb5:
                        printf("SST39SF010A\n");
                        break;
                case 0xbfb6:
                        printf("SST39SF020A\n");
                        break;
                case 0xbfb7:
                        printf("SST39SF040\n");
                        break;
                default:
                        printf("Unsupported chip or chip error\n");
                        break;
        }
}
