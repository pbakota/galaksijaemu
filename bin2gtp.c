#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

typedef Uint8 byte;
typedef Uint16 word;

#include "gtp.c"

int main(int ac, char **av)
{
    char *ptr=NULL;
    char *bsource=NULL;
    char *fdest=NULL;
    byte MEMORY[65536];

    int len=0;
    FILE* fp;
    word start = 0, end;

    for(int i=1; i<ac; ++i) {
        ptr = av[i];
        if(*ptr == '-') {
            ptr += 2;
            switch(ptr[-1]) {
                case 'o':
                    fdest=*ptr ? ptr : av[++i];
                    break;
                case 's':
                    start = strtol(ptr ? ptr : av[++i], NULL, 16);
                    break;
                default:
                    fprintf(stderr, "Invalid option: %c\n", (int)ptr[-1]);
                    return -1;
            }
        } else
            bsource = ptr;
    }

    memset((void*)MEMORY, 0, sizeof(MEMORY));
    if((fp = fopen(bsource, "rb"))) {
        len = (int)fread(&MEMORY[0x2c3a], 1, sizeof(MEMORY), fp);
        fclose(fp);
    } else {
        printf("Can't open file '%s'\n", bsource);
        return -1;
    }

    if(!start) {
        end = 0x2c3a + len;
        MEMORY[0x2c36] = 0x3a;
        MEMORY[0x2c37] = 0x2c;
        MEMORY[0x2c38] = end & 0xff;
        MEMORY[0x2c39] = (end >> 8) & 0xff;

        start = 0x2c36;
    } else {
        end = start + len;
    }

    printf("Writing '%s', start=0x%04x, end=0x%04x\n", fdest, (int)start, (int)end);

    if(save_gtp_file(fdest, MEMORY, start, end) != 0) {
        printf("Unable to write gtp file '%s'\n", fdest);
        return -1;
    }

    return 0;
}