#include <stdio.h>
#include <stdlib.h>

#include "_endian.c"
#include "basic.c"

int main(int ac, char**av)
{
    char *ptr=NULL;
    char *bsource=NULL;
    char *fdest=NULL;

    char *dest=NULL;
    int len=0;
    FILE* fp;

    for(int i=1; i<ac; ++i) {
        ptr = av[i];
        if(*ptr == '-') {
            ptr += 2;
            switch(ptr[-1]) {
                case 'o':
                    fdest=*ptr ? ptr : av[++i];
                    break;
                default:
                    fprintf(stderr, "Invalid option: %c\n", (int)ptr[-1]);
                    return -1;
            }
        } else
            bsource = ptr;
    }

    if(basic_read(bsource, &dest, &len) == 0) {        
        
        printf("Writing basic bin \"%s\" len=%d bytes\n", fdest, len);

        if((fp = fopen(fdest, "wb"))) {
            fwrite(dest, 1, len, fp);
            fclose(fp);
        }
    }

    return 0;
}
