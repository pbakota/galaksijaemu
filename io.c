// I/O

#define MAX_INPUT_SIZE 256

void draw_cursor(int pos)
{
    SDL_Rect r = {pos * SIRINA, VISINA - 1, SIRINA, 1};
    SDL_FillRect(screen, &r, SDL_MapRGBA(screen->format, 128, 0, 0, 255));
}

int input_text(const char *prompt, char *input_buffer)
{
    int input_finished = 0;
    int prompt_len = strlen(prompt), pos = strlen(input_buffer), max_len = 32 - prompt_len - 1;

    SDL_Rect input_rect = {0, 0, max_len * SIRINA, VISINA};

    SDL_StartTextInput();
    SDL_SetTextInputRect(&input_rect);

    while (input_finished == 0)
    {
        SDL_FillRect(screen, NULL, crna_color);
        SDL_LockSurface(screen);

        text_at(prompt, 0, 0);
        text_at(input_buffer, prompt_len, 0);

        SDL_UnlockSurface(screen);

        draw_cursor(prompt_len + pos);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                ExitLoop = 1;
                input_finished = 2;
                break;
            case SDL_TEXTINPUT:
                if (strlen(input_buffer) < max_len)
                {
                    int len = strlen(input_buffer);
                    for (int i = len - 1; i >= pos; --i)
                    {
                        input_buffer[i + 1] = input_buffer[i];
                    }
                    input_buffer[pos++] = toupper(event.text.text[0]);
                    input_buffer[len + 1] = 0;
                }
                break;
            case SDL_KEYDOWN:
            {
                int c;
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    c = 0x1b;
                    break;
                case SDLK_RETURN:
                    c = 0x0a;
                    break;
                case SDLK_DELETE:
                    c = 0x7f;
                    break;
                case SDLK_BACKSPACE:
                    c = 0x08;
                    break;
                case SDLK_LEFT:
                    c = 1;
                    break;
                case SDLK_RIGHT:
                    c = 2;
                    break;
                //
                default:
                    c = 0;
                    break; // unsupported key
                }

                switch (c)
                {
                case 0x1b: // ESCAPE
                    input_finished = 2;
                    break;
                case 0x0a: // RETURN
                    input_finished = 1;
                    break;
                case 0x7f: // DELETE
                    if (pos >= 0 && pos < strlen(input_buffer))
                    {
                        memcpy((void *)input_buffer + pos, (void *)input_buffer + pos + 1, strlen(input_buffer) - pos);
                    }
                    break;
                case 0x08: // BACKSPACE
                    if (pos > 0)
                    {
                        memcpy((void *)input_buffer + pos - 1, (void *)input_buffer + pos, strlen(input_buffer) - pos + 1);
                        pos--;
                    }
                    break;
                case 1:
                    if (pos > 0)
                        pos--;
                    break;
                case 2:
                    if (pos < strlen(input_buffer))
                        pos++;
                    break;
                }
            }
            break;
            }
        }

        /* Select the color for drawing. */
        SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255);

        // Clear the window
        SDL_RenderClear(renderer);
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, screen);

        int xOff = (int)MEMORY[0x2BA8] * 8;
        SDL_Rect src = {0, 0, SCREEN_W, SCREEN_H},
                 dst = {xOff + (WINDOW_W - SCREEN_W * 2) / 2 - 88, (WINDOW_H - SCREEN_H * 2) / 2, 2 * SCREEN_W, 2 * SCREEN_H};

        SDL_RenderCopy(renderer, tex, &src, &dst);

        // Render the changes above ( which up until now had just happened behind the scenes )
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(tex);

        SDL_Delay(1000 / 50);
    }

    SDL_StopTextInput();

    return input_finished - 1;
}

int input_number(const char *prompt, word *number)
{
    char number_buf[MAX_INPUT_SIZE];
    snprintf(number_buf, sizeof(number_buf) - 1, "%04X", *number);

    int result = input_text(prompt, &number_buf[0]);

    if (result == 0)
    {
        *number = (word)strtol(&number_buf[0], NULL, 16);
    }

    return result;
}

const char *build_path(const char *dirname, const char *filename)
{
    static char path_buf[MAX_INPUT_SIZE * 2] = {'\0'};

    memcpy((void *)path_buf, (void *)dirname, strlen(dirname) + 1);

#if defined(__WIN32__)
    strcat(path_buf, "\\");
#else
    strcat(path_buf, "/");
#endif
    strcat(path_buf, filename);

    return (const char *)&path_buf[0];
}

void save_memory()
{
    // By default save BASIC program
    word start = 0x2c36;
    word end = MEMORY[0x2c38] + MEMORY[0x2c39] * 256;

    char filename[MAX_INPUT_SIZE / 2];

    if (input_number("START ADDR:", &start) != 0)
        return;
    if (input_number("END ADDR:", &end) != 0)
        return;

    if (input_text("SAVE FILE:", filename) != 0 || strlen(filename) == 0)
        return;

    const char *path = build_path(workdir, filename);
    printf("Saving memory to \"%s\", start=&%04X, end=&%04X\n", path, start, end);

    if (save_gtp_file(path, MEMORY, start, end) != 0)
    {
        fprintf(stderr, "Save error!\n");
    }
    else
    {
        fprintf(stderr, "Memory saved.\n");
    }
}

void load_memory()
{
    char filename[MAX_INPUT_SIZE];
    char yn[2] = {'Y', '\0'};
    int warmstart = 0;

    if (input_text("LOAD FILE:", filename) != 0 || strlen(filename) == 0)
        return;
    if (input_text("BASIC WARM START (Y/N)?", yn) != 0)
        return;

    warmstart = (yn[0] == 'Y');

    const char *path = build_path(workdir, filename);
    if (load_gtp_file(path, MEMORY, 0) == 0)
    {
        if (warmstart)
        {
            printf("Jump to basic warmstart ...\n");
            R.PC.W = 0x0317; // BASIC WARM START
        }
    }
}
