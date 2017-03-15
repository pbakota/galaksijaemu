//================================================================
//                                                              //
// GALAKSIJA emulator                                           //
// DOS Verzija: Copyright (C) by Miodrag Jevremovic 1997.       //
// Win32 Verzija: copyright (C) by Tomaž Šolc 2002 (?).         //
// Linux (SDL2) Verzija: Copyright (C) by Peter Bakota 2017.    //
//                                                              //
//================================================================

#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <unistd.h>

#include "Z80/Z80.h"

typedef uint32_t u32;

SDL_Renderer *renderer;
SDL_Window *window;
SDL_Surface *screen;
u32 crna_color, pixel_color;
char workdir[256] = "";

void text_at(const char *str, word x, byte y);

//=========================
//                       //
// GENERALNE PROMENLJIVE //
//                       //
//=========================

// Dakle 6k RAM-a
// Max 64k memorije, prvih 8k je ROM
#define WORK_SPACE 0x4000
#define KRAJ_RAMA 0x4000

// CPU_SPEED is the speed of the CPU in Hz. It is used together with
// FRAMES_PER_SECOND to calculate how much CPU cycles must pass between
// interrupts.
#define CPU_SPEED_NORMAL 3072000 // 6.144/2 Mhz
#define CPU_SPEED_FAST 25000000  // 25 Mhz?
#define FRAMES_PER_SECOND 50

#define VISINA 13
#define SIRINA 8
#define BROJ_ZNAKOVA 128

#define SCREEN_W 256
#define SCREEN_H 208

#define WINDOW_W 640
#define WINDOW_H 480

Z80 R;
byte MEMORY[65536]; // max 64k of MEMORY
byte Fassst;
byte ExitLoop;
u32 FONT[BROJ_ZNAKOVA * VISINA * SIRINA];
u32 last_time = 0;
float sekunda = 0;
float render_time = 0;
byte active_help = 0;
char *prg_to_load = NULL;
word prg_to_jump = 0;
int cpu_speed = CPU_SPEED_NORMAL;

u32 Kmap[54] =
    {
        0,
        SDL_SCANCODE_A, SDL_SCANCODE_B, SDL_SCANCODE_C, SDL_SCANCODE_D, SDL_SCANCODE_E, SDL_SCANCODE_F,
        SDL_SCANCODE_G, SDL_SCANCODE_H, SDL_SCANCODE_I, SDL_SCANCODE_J, SDL_SCANCODE_K, SDL_SCANCODE_L,
        SDL_SCANCODE_M, SDL_SCANCODE_N, SDL_SCANCODE_O, SDL_SCANCODE_P, SDL_SCANCODE_Q, SDL_SCANCODE_R,
        SDL_SCANCODE_S, SDL_SCANCODE_T, SDL_SCANCODE_U, SDL_SCANCODE_V, SDL_SCANCODE_W, SDL_SCANCODE_X,
        SDL_SCANCODE_Y, SDL_SCANCODE_Z,
        SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
        SDL_SCANCODE_SPACE,
        SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
        SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7, SDL_SCANCODE_8, SDL_SCANCODE_9,
        SDL_SCANCODE_SEMICOLON, SDL_SCANCODE_APOSTROPHE, SDL_SCANCODE_COMMA, SDL_SCANCODE_EQUALS,
        SDL_SCANCODE_PERIOD, SDL_SCANCODE_SLASH, SDL_SCANCODE_RETURN, SDL_SCANCODE_TAB, SDL_SCANCODE_LCTRL,
        SDL_SCANCODE_DELETE, SDL_SCANCODE_SCROLLLOCK, SDL_SCANCODE_LSHIFT};

#include "gtp.c"
#include "io.c"

// Z80 stuffs
inline byte RdZ80(word addr)
{
    return MEMORY[addr & 0xffff];
}

inline void WrZ80(word addr, byte val)
{
    if (addr >= 0x2800 && addr < KRAJ_RAMA)
    {
        MEMORY[addr] = val;
    }
}

inline byte InZ80(word port)
{
    // UNUSED
    return 0xff;
}

inline void OutZ80(word port, byte val)
{
    // UNUSED
}

inline void PatchZ80(Z80 *R)
{
    // UNUSED
}

//=============
//           //
// TASTATURA //
//           //
//=============

// Skanira tastaturu, i GALAKSIJI servira ono sto ona ocekuje.

void read_keyboard(void)
{
    byte A;

    const byte *key = SDL_GetKeyboardState(NULL);

    for (A = 1; A < 54; A++)
    {
        // Par modifikacija, radi udobnosti.
        // Shift je Shift, pa bio on levi ili desni. I tome slicno.

        switch (Kmap[A])
        {
        case SDL_SCANCODE_LEFT:
            MEMORY[0x2000 + A] = (key[Kmap[A]] || key[SDL_SCANCODE_BACKSPACE]) ? 0xFE : 0xFF;
            break;

        case SDL_SCANCODE_LSHIFT:
            MEMORY[0x2000 + A] = (key[Kmap[A]] || key[SDL_SCANCODE_RSHIFT]) ? 0xFE : 0xFF;
            break;

        default:
            MEMORY[0x2000 + A] = key[Kmap[A]] ? 0xFE : 0xFF;
        }
    }
}

void draw_char(byte karakter, word pozX, byte pozY)
{
    byte tmpX;
    byte n;
    word pozK;

    //   0 -  63 => karakter-0
    //  64 -  95 => karakter-64
    //  96 - 127 => karakter-0
    // 128 - ?
    if (karakter < 64)
    {
        pozK = karakter * VISINA * SIRINA;
    }
    else if (karakter < 96)
    {
        pozK = (karakter - 64) * VISINA * SIRINA;
    }
    else if (karakter < 128)
    {
        pozK = karakter * VISINA * SIRINA;
    }
    else if (karakter < 192)
    {
        pozK = (karakter - 128 + 64) * VISINA * SIRINA;
    }
    else
    {
        pozK = (karakter - 192 + 64) * VISINA * SIRINA;
    }

    for (n = 0; n < VISINA; n++, pozY++)
    {
        byte *ptr = screen->pixels + pozY * screen->pitch + pozX * screen->format->BytesPerPixel;
        for (tmpX = 0; tmpX < SIRINA; tmpX++)
        {
            *(u32 *)ptr = FONT[pozK++];
            ptr += screen->format->BytesPerPixel;
        }
    }
}

void text_at(const char *str, word x, byte y)
{
    while (*str)
    {
        if (x > 31)
        {
            x -= 32;
            y++;
        }

        draw_char(*str++, x++ * SIRINA, y * VISINA);
    }
}

void show_help()
{
    SDL_FillRect(screen, NULL, crna_color);
    SDL_LockSurface(screen);

    text_at("GALAKSIJA EMULATOR (C)2017 V0.2 ", 0, 0);
    text_at("--------------------------------", 0, 1);
    text_at("F1          - TOGGLE HELP       ", 0, 2);
    text_at("ESC         - QUIT EMULATOR     ", 0, 3);
    text_at("F12 + SHIFT - NORMAL RESET      ", 0, 4);
    text_at("F12         - NMI RESET         ", 0, 5);
    text_at("F8          - TOGGLE CPU SPEED  ", 0, 6);
    text_at("F2 + SHIFT  - SAVE MEMORY  (GTP)", 0, 7);
    text_at("F2          - LOAD MEMORY  (GTP)", 0, 8);

    SDL_UnlockSurface(screen);
}

void refresh_screen(void)
{
    byte x, y;
    word adresa;

    // 0x2800 - 0x2a00 (size=512) VIDEO MEMORIJA
    adresa = 0x2800;

    SDL_FillRect(screen, NULL, crna_color);
    SDL_LockSurface(screen);

    for (y = 0; y < 16; y++)
    {
        for (x = 0; x < 32; x++)
        {
            draw_char(MEMORY[adresa++], x * SIRINA, y * VISINA);
        }
    }

    SDL_UnlockSurface(screen);
}

void load_prg(char *prg)
{
    if (load_gtp_file(prg, MEMORY, 0) == 0)
    {
        R.PC.W = prg_to_jump ? prg_to_jump : 0x0317; // 0x0317 = BASIC WARM START
    }
}

word LoopZ80(Z80 *R)
{
    u32 current_time = SDL_GetTicks();
    float dt = (float)(current_time - last_time) / 1000.0;
    last_time = current_time;
    //    int frame_ready=0;

    if (sekunda >= 1.0)
    {
        char buffer[256];
        sprintf(buffer, "FPS: %d, SPEED=%s", (int)(1.0 / dt), (cpu_speed == CPU_SPEED_NORMAL ? "NORMAL" : "FAST"));

        SDL_SetWindowTitle(window, buffer);

        sekunda -= 1.0;
        //frame_ready = 1;
    }

    sekunda += dt;

    if (dt < 1000.0 / 60.0)
    {
        SDL_Delay((int)((1000.0 / 60.0) - dt));
    }

    if (active_help)
    {
        show_help();
    }
    else //if(frame_ready)
    {
        if (!(R->IFF & IFF_2))
        {
            Fassst++;
        }
        else
        {
            Fassst = 0;
        }

        // Ako je EI osvezavaj i ekran i tastaturu, kao i kod prave masine.
        if (!Fassst)
        {
            read_keyboard();
            refresh_screen();
        }
        else
        {
            switch (Fassst)
            {
            // Because screen is made under IRQ, there is no more further screen updates.
            case 1:
                // Need to clear it ?
                SDL_FillRect(screen, NULL, crna_color);
                break;

            // Stay where You are.
            case 2:
                Fassst--;
                break;
            }
        }
    }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            ExitLoop = 1;
            break;
        case SDL_KEYDOWN:
        {
            u32 shift = event.key.keysym.mod & KMOD_SHIFT;
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                ExitLoop = 1;
                break;
            case SDLK_F8: // TOGGLE SPEED
                cpu_speed = (cpu_speed == CPU_SPEED_NORMAL) ? CPU_SPEED_FAST : CPU_SPEED_NORMAL;
                R->IPeriod = cpu_speed / FRAMES_PER_SECOND;
                break;
            case SDLK_F1: // TOGGLE HELP SCREEN
                active_help = !active_help;
                break;
            case SDLK_F2: // SAVE/LOAD MEMORY
                if (shift)
                    save_memory();
                else
                    load_memory();
                break;
            case SDLK_F12: // HARD/NMI RESET
                if (shift)
                {
                    ResetZ80(R);
                    return INT_NONE;
                }
                else
                    return INT_NMI;
                break;
            }
        }
        break;
        }
    }

    /* Select the color for drawing. It is set to black here. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Clear the window
    SDL_RenderClear(renderer);

    // 0x2bb0 = BROJAC ZA POMERANJE SLIKE
    int yOff = (int)MEMORY[0x2BB0] * 3;

    // 0x2ba8 = HORIZONTALNA POZICIJA TEKSTA
    int xOff = (int)MEMORY[0x2BA8] * 8;

    if (active_help)
    {
        yOff = 0;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, screen);

    if (yOff)
    {
        // yOff = 9..6..3..0
        int o = 9;
        SDL_Rect src = {0, o - yOff, SCREEN_W, SCREEN_H - yOff},
                 dst = {xOff + (WINDOW_W - SCREEN_W * 2) / 2 - 88, o + (WINDOW_H - SCREEN_H * 2) / 2, 2 * SCREEN_W, 2 * (SCREEN_H - yOff)};

        SDL_RenderCopy(renderer, tex, &src, &dst);
    }
    else
    {
        SDL_Rect src = {0, 0, SCREEN_W, SCREEN_H},
                 dst = {xOff + (WINDOW_W - SCREEN_W * 2) / 2 - 88, (WINDOW_H - SCREEN_H * 2) / 2, 2 * SCREEN_W, 2 * SCREEN_H};

        SDL_RenderCopy(renderer, tex, &src, &dst);
    }

    // Render the changes above ( which up until now had just happened behind the scenes )
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(tex);

    if (prg_to_load)
    {
        // XXX: Is the machine in idle loop?
        if (R->PC.W == 0x0ceb)
        {
            printf("Loading prg \"%s\" ...\n", prg_to_load);
            load_prg(prg_to_load);

            prg_to_load = NULL;
        }
    }

    if (ExitLoop)
    {
        return INT_QUIT;
    }

    if (active_help)
    {
        return INT_NONE;
    }

    return INT_IRQ; //(frame_ready) ? INT_IRQ : INT_NONE;
}

byte DebugZ80(Z80 *R)
{
    //printf("%04x\n", (unsigned int)R->PC.W);
    return 1;
}

word start_machine()
{
#if defined(DEBUG)
    R.Trap = 0xFFFF;
    R.Trace = 0;
#endif
    last_time = SDL_GetTicks();

    R.IPeriod = cpu_speed / FRAMES_PER_SECOND;

    ResetZ80(&R);
    return RunZ80(&R);
}

void init_memory()
{
    FILE *fp;
    int n, a, b, x, karakter;
    word adresa;
    byte buffer[2048];

    // Clear memory
    memset((void *)MEMORY, 0, sizeof(MEMORY));

    if (!(fp = fopen("CHRGEN.BIN", "r")))
    {
        fprintf(stderr, "Can't open CHRGEN.BIN\n");
        exit(-1);
    }
    fread(buffer, 1, 2048, fp);
    fclose(fp);

    u32 *ptr = FONT;
    for (karakter = 0; karakter < BROJ_ZNAKOVA; karakter++)
    {
        for (n = 0; n < VISINA; n++)
        {
            adresa = n << 7; // 128*VISINA + karakter
            adresa |= karakter;

            a = 1;
            b = buffer[adresa];

            for (x = 0; x < SIRINA; x++)
            {
                *ptr++ = (!(b & a)) ? pixel_color : crna_color;
                a <<= 1;
            }
        }
    }

    if (!(fp = fopen("ROM1.BIN", "r")))
    {
        fprintf(stderr, "Can't open ROM1.BIN\n");
        exit(-1);
    }
    fread(&MEMORY[0], 1, 4096, fp);
    fclose(fp);

    if (!(fp = fopen("ROM2.BIN", "r")))
    {
        fprintf(stderr, "Can't open ROM2.BIN\n");
        exit(-1);
    }
    fread(&MEMORY[0x1000], 1, 4096, fp);
    fclose(fp);

    // Tastatura
    for (n = 0x2000; n < 0x2800; n++)
    {
        MEMORY[n] = 0xFF;
    }

    // Ekran
    for (n = 0x2800; n < 0x2A00; n++)
    {
        MEMORY[n] = ' ' + n;
    }

    // 6116-ice
    for (n = 0x2A00; n < KRAJ_RAMA; n++)
    {
        MEMORY[n] = 0;
    }

    Fassst = 0;
    ExitLoop = 0;
}

void usage(char *arg0)
{
    fprintf(stderr, "\nUsage: %s [<GTP file to load> [-j<address>]] [-c<font color>] [-w<work dir>]\n\n", arg0);
}

int main(int argc, char **argv)
{
    int sizeX = WINDOW_W;
    int sizeY = WINDOW_H;
    int i;
    char *ptr;
    int font_color = 0;
    char dirname[256];

    if (getcwd(dirname, sizeof(dirname)) != NULL)
    {
        memcpy((void *)workdir, (void *)dirname, strlen(dirname) + 1);
    }

    for (i = 1; i < argc; ++i)
    {
        ptr = argv[i];

        if (*ptr == '-')
        {
            ptr += 2;
            switch (ptr[-1])
            {
            case 'h':
                usage(argv[0]);
                return 0;
            case 'j':
                prg_to_jump = (word)strtol(ptr, NULL, 16);
                break;
                break;
            case 'c':
                // color
                font_color = atoi(ptr);
                break;
            case 'w':
                if (strlen(ptr) < sizeof(workdir))
                {
                    memcpy((void *)workdir, ptr, strlen(ptr) + 1);
                }
                break;
            default:
                fprintf(stderr, "Unrecognized option %c", (int)ptr[-1]);
                return -1;
            }
        }
        else
        {
            prg_to_load = ptr;
        }
    }

    fprintf(stderr, "Workdir: %s\n", workdir);

    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS) == -1)
    {
        fprintf(stderr, "Failed to initialize SDL : %s\n", SDL_GetError());
        return -1;
    }
    atexit(SDL_Quit);

    window = SDL_CreateWindow("Window Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sizeX, sizeY, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to create window : %s\n", SDL_GetError());
        return -1;
    }

    screen = SDL_CreateRGBSurface(0, SCREEN_W, SCREEN_H, 32, 0, 0, 0, 0);
    if (screen == NULL)
    {
        fprintf(stderr, "Failed to create screen : %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        fprintf(stderr, "Failed to create renderer : %s\n", SDL_GetError());
        return -1;
    }

    crna_color = SDL_MapRGB(screen->format, 0, 0, 0);
    switch (font_color)
    {
    default:
    case 0: // gray
        pixel_color = SDL_MapRGB(screen->format, 128, 128, 128);
        break;
    case 1: // green
        pixel_color = SDL_MapRGB(screen->format, 0, 128, 0);
        break;
    case 2: // ember
        pixel_color = SDL_MapRGB(screen->format, 147, 128, 106);
        break;
    }

    init_memory();

    word pc = start_machine();
    fprintf(stderr, "Leaving emulator PC=$%04x\n", pc);

    SDL_FreeSurface(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}
