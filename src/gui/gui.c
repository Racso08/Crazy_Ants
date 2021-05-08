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

/*
typedef struct {
    int posX;
    int posY;
    int finalDest;  //add
    int path;       //add
    int vel;
    int dest;
    int type;
    int channel;
    int time;
    int priority;
} ant_t;*/

// GET size of canal to make the positions
int canal1[6] = {450,422,394,366,338,310};
int canal2[6] = {450,422,394,366,338,310};
int canal3[6] = {350,322,294,266,238,210};
int canal4[6] = {750,778,806,834,862,890};
int canal5[6] = {720,748,776,804,832,860};
int canal6[6] = {840,868,896,924,952,980};

SDL_Texture *initialize_texture_from_file(const char* file_name, SDL_Renderer *renderer);
void waze(ant_t *ant);

static const int width = 1200;
static const int height = 700;
char input[50] = "";

int main(int argc, char **argv)
{
    srand(time(0));
	CEthread_init();
	initializeChannels();
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
    SDL_Texture * zompopaTXT = initialize_texture_from_file("src/gui/img/zompopas.png", renderer);
    SDL_Texture * reinaTXT = initialize_texture_from_file("src/gui/img/reina.png", renderer);
    SDL_Texture * antTXT = initialize_texture_from_file("src/gui/img/antIsaac.jpeg", renderer);

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

                    case SDLK_q:


                    case SDLK_r:
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                            "Missing file",
                            "File is missing. Please reinstall the program.",
                            NULL);

                    case SDLK_c:
                        SDL_StopTextInput();
                        type = atoi(strtok(input,",")); // 0 1 1 -1
                        channel = atoi(strtok(NULL,","));
                        dest = atoi(strtok(NULL,","));   
                        extra = atoi(strtok(NULL,","));  

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

                        createAnt(posX, posY, channelLenght, dest, type, channel, channelTime, priority, -1, -1);    

                        strcpy(input,"");
                }
            }
        }

        // Clear screen
        SDL_RenderClear(renderer);

        // Draw
        SDL_RenderCopy(renderer, backgroundTxt, NULL, &backgroundRect); // BG

        //Label
        TTF_Init();
        TTF_Font *verdanaFont = TTF_OpenFont("src/gui/lazy.ttf", 50);
        SDL_Color textColor = { 0, 0, 0, 255 };
        SDL_Surface *textSurface = TTF_RenderText_Solid(verdanaFont, input, textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        int w, h;
        SDL_QueryTexture(textTexture,NULL,NULL,&w,&h);
        SDL_Rect textRect = {470,60,w,h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect); // Label
 
        //SPRITES
        if(allAnts.count > 0){
            queueNode *temp = (queueNode*) allAnts.head;
            ant_t *ant;

            while(temp != NULL){
                ant = (ant_t*)temp->item;

                // ANT
                SDL_Rect spriteNormal = {(sprite % 5)*24,0,24,38};
                SDL_Rect normalRect = {ant->posX,ant->posY,24,38}; 
                SDL_RenderCopyEx(renderer,normalTXT,&spriteNormal,&normalRect,0,NULL,0);
                temp = temp->next;
            }

        }

        // Show what was drawn
        SDL_RenderPresent(renderer);

        // Frametime configuration
        frametime ++;

        if (FPS / frametime == 4){
            frametime = 0;
            sprite++;
        }

        SDL_Delay(1000/60);
    }

    // Release resources
    //SDL_DestroyTexture(image_texture);
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

void waze(ant_t *ant){

    if(ant->dest == 1){

        if(ant->path == 0){
            if (ant->posX < 200){
                ant->posX ++;
            }else{
                ant->path = 1;
            }
        }

        else if(ant->path == 1){
            if(ant->channel == 1){
                if(ant->posY != 340){
                    ant->posY --;
                }else{
                    ant->path = 2;
                }
            }

            if(ant->channel == 2){
                ant->path = 2;
            }

            if(ant->channel == 3){
                if(ant->posY != 540){
                    ant->posY ++;
                }else{
                    ant->path = 2;
                }
            }
        }

        else if(ant->path == 2){
            if(ant->posX < ant->finalDest){
                ant->posX ++;
            }
            else if(ant->posX > ant->finalDest){
                ant->posX --;
            }
            else{
                ant->path = 3;
                //printf("stop;");
            }
        }

        else if(ant->path == 3){
            if(ant->posX < 1000){
                ant->posX ++;
            }
            else{
                ant->path = 4;
            }
        }

        else if(ant->path == 4){
            if(ant->posY < 440){
                ant->posY ++;
            }
            else if(ant->posY > 440){
                ant->posY --;
            }
            else{
                ant->path = 5;
            }
        }

        else if(ant->path == 5){
            if(ant->posX < 1100){
                ant->posX ++;
            }
            else{
                ant->path = 6;
            }
        }

        else{
            //printf("La hormiga %d finalizo su trayecto\n",ant->vel);
        } 
    }
    else if(ant->dest == 2){
    
        if(ant->path == 0){
            if (ant->posX > 900){
                ant->posX --;
            }else{
                ant->path = 1;
            }
        }

        else if(ant->path == 1){
            if(ant->channel == 1){
                if(ant->posY != 340){
                    ant->posY --;
                }else{
                    ant->path = 2;
                }
            }

            if(ant->channel == 2){
                ant->path = 2;
            }

            if(ant->channel == 3){
                if(ant->posY != 540){
                    ant->posY ++;
                }else{
                    ant->path = 2;
                }
            }
        }

        else if(ant->path == 2){
            if(ant->posX < ant->finalDest){
                ant->posX ++;
            }
            else if(ant->posX > ant->finalDest){
                ant->posX --;
            }
            else{
                ant->path = 3;
                //printf("stop");
            }
        }

        else if(ant->path == 3){
            if(ant->posX > 200){
                ant->posX --;
            }
            else{
                ant->path = 4;
            }
        }

        else if(ant->path == 4){
            if(ant->posY < 440){
                ant->posY ++;
            }
            else if(ant->posY > 440){
                ant->posY --;
            }
            else{
                ant->path = 5;
            }
        }

        else if(ant->path == 5){
            if(ant->posX > 100){
                ant->posX --;
            }
            else{
                ant->path = 6;
            }
        }

        else{
            //printf("La hormiga %d finalizo su trayecto\n",ant->vel);
        } 

    }

    else{
        printf("[ERROR] Destination not valid");
    }
}

