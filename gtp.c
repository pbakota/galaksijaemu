
typedef struct {
    Uint8   type;
    Uint32  len;
} GTP_header;

typedef struct {
    Uint8   magic; // 0xa5
    Uint16  start;
    Uint16  end;
    Uint8   data[0];
    // crc.w at the end
} GTP_datablock;

int load_gtp_file(const char *gtp_file, void* memory, int block)
{
    Uint16 bcnt = 0;
    SDL_RWops *file = SDL_RWFromFile(gtp_file, "rb");

    if(file == NULL)
    {
        fprintf(stderr, "Unable to open GTP file \"%s\"\n", gtp_file);
        return -1;
    }

    Sint64 total_fsize = SDL_RWsize(file);
    //fprintf(stderr, "Load size=0x%08lx\n", total_fsize);

    while(SDL_RWtell(file) < total_fsize)
    {
        GTP_header hdr;
        hdr.type = SDL_ReadU8(file);
        hdr.len = SDL_ReadLE32(file);

        Sint64 block_start_offset = SDL_RWtell(file);

        fprintf(stderr, "%d. Block type: 0x%02x, OFFSET=0x%08lx LEN=0x%04x\n", (int)bcnt, (int)hdr.type, block_start_offset, (int)hdr.len);

        switch(hdr.type) 
        {
            case 0: // Normal block
            case 1: // Turbo block
            {
                GTP_datablock blk;
                blk.magic = SDL_ReadU8(file);
                blk.start = SDL_ReadLE16(file);
                blk.end = SDL_ReadLE16(file);

                if(blk.magic != 0xa5)
                {
                    fprintf(stderr, "   ERROR: Sync 0xa5 was not found!\n");
                    break;
                }

                SDL_RWread(file, memory+blk.start, (size_t)(blk.end-blk.start), 1);
                byte crc = SDL_ReadU8(file); SDL_ReadU8(file); // checksum + 1 garbage byte

                byte chk = 0xa5 + ((blk.start>>8)&0xff) + ((blk.start)&0xff) + ((blk.end>>8)&0xff) + ((blk.end)&0xff);
                for(int i=blk.start;i<blk.end; ++i) {
                    chk += *((byte*)memory+i);
                }

                fprintf(stderr, "   START=0x%04x END=0x%04x CRC=0x%02x POS=0x%08lx\n", 
                    (int)blk.start, (int)blk.end, (int)crc, SDL_RWtell(file));

                if(((chk + crc)&0xff) != 0xff)
                {
                    fprintf(stderr, "   LOAD ERROR: 0x%02x + 0x%02x != 0xff\n", (int)crc, (int)chk);
                }
            }
            break;
            case 16: // Name block
            {
                char buffer[256];
                SDL_RWread(file, buffer, 1, hdr.len);
                fprintf(stderr, "   FILE=\"%s\" POS=0x%08lx\n", buffer, SDL_RWtell(file));
            }
            break;
            default:
                fprintf(stderr, "WARNING: Unknown block type 0x%02x\n", (int)hdr.type);
                break;
        }

        // Skip to next block
        SDL_RWseek(file, block_start_offset + hdr.len, RW_SEEK_SET);
        ++bcnt;
    }

    SDL_RWclose(file);
    return 0;
}


int save_gtp_file(const char* gtp_file, void *memory, word start, word end)
{
    SDL_RWops *file = SDL_RWFromFile(gtp_file, "wb");

    if(file == NULL)
    {
        fprintf(stderr, "Unable to open GTP file \"%s\"\n", gtp_file);
        return -1;
    }

    GTP_header hdr = { 0, end-start + sizeof(GTP_datablock)+3 };
    SDL_WriteU8(file, hdr.type);
    SDL_WriteLE32(file, hdr.len);

    byte crc = 0xa5;
    crc += (start&0xff);
    crc += ((start>>8)&0xff);
    crc += (end&0xff);
    crc += ((end>>8)&0xff);

    for(int i=start; i<end; ++i)
    {
        crc += *((byte*)memory+i);
    }

    SDL_WriteU8(file, 0xa5); // magic
    SDL_WriteLE16(file, start);
    SDL_WriteLE16(file, end);

    SDL_RWwrite(file, memory+start, end-start, 1);
    SDL_WriteU8(file, (byte)(0xff-crc)); // crc
    SDL_WriteU8(file, 0xff); // garbage

    SDL_RWclose(file);
    return 0;
}
