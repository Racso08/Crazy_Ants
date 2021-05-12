// Compile command: gcc gui.c -o gui `sdl2-config --libs --cflags` -lSDL2_image
// Run Command: ./gui

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../logic/ant.h"
#include "../logic/channel.h"
#include "../logic/antScheduler.h"
#include "../logic/antFlow.h"

SDL_Texture *initialize_texture_from_file(const char* file_name, SDL_Renderer *renderer);
void waze(ant_t *ant);

static const int width = 1200;
static const int height = 700;
char input[50] = "";

int main(int argc, char **argv)
{
    srand(time(0));
	initializeChannels();
    initializePositions();
	clock_gettime(CLOCK_REALTIME, &sign1Begin);
	clock_gettime(CLOCK_REALTIME, &sign2Begin);
	clock_gettime(CLOCK_REALTIME, &sign3Begin);
    
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a SDL window
    SDL_Window *window = SDL_CreateWindow("Crazy Ants", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

    // Create a renderer (accelerated and in sync with the display refresh rate)
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Initialize support for loading PNG and JPEG images
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    // Textures
    SDL_Texture * backgroundTxt = initialize_texture_from_file("src/gui/img/background.png", renderer);
    SDL_Texture * normalTXT = initialize_texture_from_file("src/gui/img/normal.png", renderer);
    SDL_Texture * normalTXTIzq = initialize_texture_from_file("src/gui/img/normal_izq.png", renderer);
    SDL_Texture * zompopaTXT = initialize_texture_from_file("src/gui/img/zompopas.png", renderer);
    SDL_Texture * zompopaTXTIzq = initialize_texture_from_file("src/gui/img/zompopas_izq.png", renderer);
    SDL_Texture * reinaTXT = initialize_texture_from_file("src/gui/img/reina.png", renderer);
    SDL_Texture * reinaTXTIzq = initialize_texture_from_file("src/gui/img/reina_izq.png", renderer);

    // Define where on the "screen" we want to draw the texture
    SDL_Rect backgroundRect = {0,0,1200,800};

    // GRAPHICS VARIABLES
    int FPS = 60;
    int frametime = 0;
    int sprite = 0;
    bool running = true;
    SDL_Event event;

    // ANT Variables    
    int posX;
    int posY;
    int channelLenght;
    int dest;
    int type;
    int channel;
    int channelTime;
    int priority;
    int extra;

    // Counters for canalization
    int canal1Counter = 0;
    int canal2Counter = 0;
    int canal3Counter = 0;
    int canal4Counter = 0;
    int canal5Counter = 0;
    int canal6Counter = 0;

    CEthread_init();

    int inicio = 0;

    while(running) {
        // Process events
         SDL_StartTextInput(); 

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
            
             else if(event.type == SDL_TEXTINPUT) {
                strcat(input,event.text.text);
            }

            else if(event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        running = false;
                        break;

                    case SDLK_j:
                        channel = 1;
                        printf("Se selecciono el canal 1\n");
                        break;

                    case SDLK_k:
                        channel = 2;
                        printf("Se selecciono el canal 2\n");
                        break;

                    case SDLK_l:
                        channel = 3;
                        printf("Se selecciono el canal 3\n");
                        break;

                    case SDLK_z:
                        type = 0;
                        printf("Se selecciono una hormiga normal\n");
                        break;
                    
                    case SDLK_x:
                        type = 1;
                        printf("Se selecciono una hormiga zompopa\n");
                        break;

                    case SDLK_c:
                        type = 2;
                        printf("Se selecciono una hormiga reina\n");
                        break;

                    case SDLK_n:
                        dest = 0;
                        printf("Se selecciono como destino el hormiguero izquierdo\n");
                        break;

                    case SDLK_m:
                        dest = 1;
                        printf("Se selecciono como destino el hormiguero derecho\n");
                        break;

                    case SDLK_e:
                        extra = atoi(input);
                        printf("Se ingreso el parámetro extra\n");
                        printf("%s\n",input);
                        break;

                    case SDLK_BACKSPACE:
                        input[strlen(input)-1] = '\0';
                        break;
                        
                    case SDLK_r:
                        SDL_StopTextInput();

                        if(dest == 1){
                            posX = 80;
                            posY= 440;
                        }else if (dest == 0){
                            posX = 1100;
                            posY = 440;
                        }

                        if(channel == 1){
                            channelLenght = channel1->lenght;   
                            
                            if(channel1->scheduler == 1){
                                priority = extra;
                                channelTime = -1;
                            }else if(channel1->scheduler == 2 || channel1->scheduler == 4){
                                channelTime = extra;
                                priority = -1;
                            }else {
                                channelTime = -1;
                                priority = -1;
                            }

                        }else if(channel == 2){
                            channelLenght = channel2->lenght; 

                            if(channel2->scheduler == 1){
                                priority = extra;
                                channelTime = -1;
                            }else if(channel2->scheduler == 2 || channel2->scheduler == 4){
                                channelTime = extra;
                                priority = -1;
                            }else {
                                channelTime = -1;
                                priority = -1;
                            }

                        }else if(channel == 3){
                            channelLenght = channel3->lenght; 

                            if(channel3->scheduler == 1){
                                priority = extra;
                                channelTime = -1;
                            }else if(channel3->scheduler == 2 || channel3->scheduler == 4){
                                channelTime = extra;
                                priority = -1;
                            }else {
                                channelTime = -1;
                                priority = -1;
                            } 
                        }
                        
                        if (type == 2) {
                            channelTime = extra;
                        }

                        createAnt(posX, posY, channelLenght, dest, type, channel, channelTime, priority);

                        strcpy(input,"");
                        break;
                }
            }
        }

        // Clear screen
        SDL_RenderClear(renderer);

        // Draw
        SDL_RenderCopy(renderer, backgroundTxt, NULL, &backgroundRect); // BG

        //Label
        /*
        TTF_Init();
        TTF_Font *verdanaFont = TTF_OpenFont("src/gui/lazy.ttf", 50);
        SDL_Color textColor = { 0, 0, 0, 255 };
        SDL_Surface *textSurface = TTF_RenderText_Solid(verdanaFont, input, textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        int w, h;
        SDL_QueryTexture(textTexture,NULL,NULL,&w,&h);
        SDL_Rect textRect = {470,60,w,h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect); // Label
        TTF_Quit();
        TTF_CloseFont(verdanaFont);
        */

        //SPRITES
        if(allAnts.count > 0){
            queueNode *temp = (queueNode*) allAnts.head;
            ant_t *ant;

            while(temp != NULL){
                ant = (ant_t*)temp->item;

                // ANT 

                if(ant->dest == 1){
                    if(ant->type == 0){
                        SDL_Rect spriteNormal = {(sprite % 5)*24,0,24,38};
                        SDL_Rect normalRect = {ant->posX,ant->posY,24,38}; 
                        SDL_RenderCopyEx(renderer,normalTXTIzq,&spriteNormal,&normalRect,0,NULL,0);
                    }

                    if(ant->type == 1){
                        SDL_Rect spriteNormal = {(sprite % 3)*32,0,32,39};
                        SDL_Rect normalRect = {ant->posX,ant->posY,32,39}; 
                        SDL_RenderCopyEx(renderer,zompopaTXTIzq,&spriteNormal,&normalRect,0,NULL,0);
                    }

                    if(ant->type == 2){
                        SDL_Rect spriteNormal = {(sprite % 4)*14,0,14,38};
                        SDL_Rect normalRect = {ant->posX,ant->posY,14,38};
                        SDL_RenderCopyEx(renderer,reinaTXTIzq,&spriteNormal,&normalRect,0,NULL,0);
                    }
                }
                else{
                    if(ant->type == 0){
                        SDL_Rect spriteNormal = {(sprite % 5)*24,0,24,38};
                        SDL_Rect normalRect = {ant->posX,ant->posY,24,38};
                        SDL_RenderCopyEx(renderer,normalTXT,&spriteNormal,&normalRect,0,NULL,0);
                    }

                    if(ant->type == 1){
                        SDL_Rect spriteNormal = {(sprite % 3)*32,0,32,39};
                        SDL_Rect normalRect = {ant->posX,ant->posY,32,39}; 
                        SDL_RenderCopyEx(renderer,zompopaTXT,&spriteNormal,&normalRect,0,NULL,0);
                    }

                    if(ant->type == 2){
                        SDL_Rect spriteNormal = {(sprite % 4)*14,0,14,38};
                        SDL_Rect normalRect = {ant->posX,ant->posY,14,38}; 
                        SDL_RenderCopyEx(renderer,reinaTXT,&spriteNormal,&normalRect,0,NULL,0);
                    }
                }
                
                temp = temp->next;
            }

        }

        // Show what was drawn
        SDL_RenderPresent(renderer);

        if(inicio == 0){
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
            "Missing file",
            "File is missing. Please reinstall the program.",
            NULL);
            inicio = 1;
        }

        // Frametime configuration
        frametime ++;

        if (FPS / frametime == 4){
            frametime = 0;
            sprite++;
        }

        SDL_Delay(1000/60);
    }

    // Release resources
    SDL_DestroyTexture(backgroundTxt);
    SDL_DestroyTexture(normalTXT);
    SDL_DestroyTexture(normalTXTIzq);
    SDL_DestroyTexture(zompopaTXT);
    SDL_DestroyTexture(zompopaTXTIzq);
    SDL_DestroyTexture(reinaTXT);
    SDL_DestroyTexture(reinaTXTIzq);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

SDL_Texture *initialize_texture_from_file(const char* file_name, SDL_Renderer *renderer) {
    SDL_Surface *image = IMG_Load(file_name);
    SDL_Texture * image_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return image_texture;
}