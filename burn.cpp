#include <stdio.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>

#define BASE 0xD0000

#define ROMADD(X) (volatile unsigned char far *)MK_FP(0xD000, (X))

volatile unsigned char far *unlock1 = ROMADD(0x5555);
volatile unsigned char far *unlock2 = ROMADD(0x2AAA);

#define COMMAND(X) \
        *unlock1 = 0xAA; \
        *unlock2 = 0x55; \
        *unlock1 = (X);

void waitok() {
        unsigned char v1 = *ROMADD(0);
        unsigned char v2 = *ROMADD(0);
        while ((v1 & 0x40) != (v2 & 0x40)) {
                v1 = v2;
                v2 = *ROMADD(0);
        }
}

int main(int argc, char **argv) {

        if (argc != 2) {
                printf("Usage: burn <filename>\n");
                return 10;
        }

        COMMAND(0x90);
        unsigned char manufacturer = *ROMADD(0x0000);
        unsigned char product = *ROMADD(0x0001);
        COMMAND(0xF0);

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
                        return 10;
        }

        int fd = open(argv[1], O_BINARY | O_RDONLY);
        if (!fd) {
                printf("Error opening %s\n", argv[1]);
                return 10;
        }
        
        printf("Erasing now...");
        COMMAND(0x80);
        COMMAND(0x10);

        waitok();
        printf("done\n");

        printf("Programming...");
        unsigned long i = 0;
        char c;
        while ((i < 65536) && (read(fd, &c, 1) > 0)) {
                COMMAND(0xA0)
                *ROMADD(i) = c;
                waitok();
                i++;
        }
        close(fd);
        printf("done\n");
        return 0;
}
