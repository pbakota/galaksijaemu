// I/O

#define MAX_INPUT_SIZE 256

void draw_cursor(int pos)
{
    SDL_Rect r = { pos*SIRINA, VISINA-1, SIRINA, 1 };
    SDL_FillRect(screen, &r, SDL_MapRGBA(screen->format, 128,0,0,255));
}

void clear_events()
{
    SDL_Event event;
    while(SDL_PollEvent(&event));
}

int input_text(const char* prompt, char* input_buffer)
{
    int input_finished = 0;
    int prompt_len = strlen(prompt), pos=strlen(input_buffer);

    clear_events();

    while(input_finished == 0)
    {
        SDL_FillRect(screen, NULL, crna_color);
        SDL_LockSurface( screen );

        text_at(prompt, 0, 0);
        text_at(input_buffer, prompt_len, 0);

        SDL_UnlockSurface( screen );

        draw_cursor(prompt_len + pos);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch(event.type) 
            {
                case SDL_QUIT:
                    ExitLoop = 1;
                    input_finished = 2;
                break;
                case SDL_KEYPRESSED:
                {
                    int c;
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_a: c = 'A'; break;
                        case SDLK_b: c = 'B'; break;
                        case SDLK_c: c = 'C'; break;
                        case SDLK_d: c = 'D'; break;
                        case SDLK_e: c = 'E'; break;
                        case SDLK_f: c = 'F'; break;
                        case SDLK_g: c = 'G'; break;
                        case SDLK_h: c = 'H'; break;
                        case SDLK_i: c = 'I'; break;
                        case SDLK_j: c = 'J'; break;
                        case SDLK_k: c = 'K'; break;
                        case SDLK_l: c = 'L'; break;
                        case SDLK_m: c = 'M'; break;
                        case SDLK_n: c = 'N'; break;
                        case SDLK_o: c = 'O'; break;
                        case SDLK_p: c = 'P'; break;
                        case SDLK_q: c = 'Q'; break;
                        case SDLK_r: c = 'R'; break;
                        case SDLK_s: c = 'S'; break;
                        case SDLK_t: c = 'T'; break;
                        case SDLK_u: c = 'U'; break;
                        case SDLK_v: c = 'V'; break;
                        case SDLK_w: c = 'W'; break;
                        case SDLK_x: c = 'X'; break;
                        case SDLK_y: c = 'Y'; break;
                        case SDLK_z: c = 'Z'; break;
                        //
                        case SDLK_0: c = '0'; break;
                        case SDLK_1: c = '1'; break;
                        case SDLK_2: c = '2'; break;
                        case SDLK_3: c = '3'; break;
                        case SDLK_4: c = '4'; break;
                        case SDLK_5: c = '5'; break;
                        case SDLK_6: c = '6'; break;
                        case SDLK_7: c = '7'; break;
                        case SDLK_8: c = '8'; break;
                        case SDLK_9: c = '9'; break;
                        case SDLK_SPACE: c = ' '; break;
                        case SDLK_ESCAPE: c = 0x1b; break;
                        case SDLK_RETURN: c = 0x0a; break;
                        case SDLK_DELETE: c = 0x7f; break;
                        case SDLK_BACKSPACE: c = 0x08; break;
                        case SDLK_LEFT:   c = 1; break;
                        case SDLK_RIGHT:  c = 2; break;
                        //
                        default: c = 0; break; // unsupported key
                    }

                    switch(c)
                    {
                        case 0x1b: // ESCAPE
                            input_finished = 2;
                        break;
                        case 0x0a: // RETURN
                            input_finished = 1;
                        break;
                        case 0x7f: // DELETE
                            if(pos >= 0 && pos < strlen(input_buffer))
                            {
                                memcpy((void*)input_buffer+pos, (void*)input_buffer+pos+1, strlen(input_buffer)-pos);
                            }
                        break;
                        case 0x08: // BACKSPACE
                        break;
                        case 1:
                            if(pos>0) pos --;
                        break;
                        case 2:
                            if(pos < strlen(input_buffer)) pos ++;
                        break;
                        default:
                            if(c)
                            {
                                if(pos < 32 - prompt_len)
                                {
                                    input_buffer[pos++] = c;
                                }
                            }
                        break;
                    }
                }                
                break;
            }
        }   

        /* Select the color for drawing. */
        SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255);

        // Clear the window
        SDL_RenderClear( renderer );
        SDL_Texture* tex = SDL_CreateTextureFromSurface( renderer, screen );

        int xOff = (int)MEMORY[0x2BA8]*8;
        SDL_Rect src = { 0, 0, SCREEN_W, SCREEN_H },
                 dst = { xOff + (WINDOW_W-SCREEN_W*2)/2 - 88, (WINDOW_H-SCREEN_H*2)/2, 2*SCREEN_W, 2*SCREEN_H };

        SDL_RenderCopy( renderer, tex, &src, &dst);

        // Render the changes above ( which up until now had just happened behind the scenes )
        SDL_RenderPresent( renderer);
        SDL_DestroyTexture(tex);

        SDL_Delay(1000/50);
    }

    clear_events();

    return input_finished - 1;
}

int input_number(const char* prompt, word* number)
{
    char number_buf[MAX_INPUT_SIZE];
    snprintf(number_buf, sizeof(number_buf)-1, "%04X", *number);

    int result = input_text(prompt, &number_buf[0]);

    if(result == 0)
    {
        *number = (word)strtol(&number_buf[0], NULL, 16);
    }

    return result;
}

void save_memory()
{
    word start = 0x2b3c;
    input_number("START ADDR:", &start);
    
    //save_gtp_file(file, MEMORY);
}

void load_basic()
{
    
}

void save_basic()
{

}

void load_gtp()
{

}
