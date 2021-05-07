// Compile command: gcc gui.c -o gui `sdl2-config --libs --cflags` -lSDL2_image
// Run Command: ./gui

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
} ant_t;

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

int main(int argc, char **argv)
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a SDL window
    SDL_Window *window = SDL_CreateWindow("Crazy Ants", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

    // Create a renderer (accelerated and in sync with the display refresh rate)
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Initialize support for loading PNG and JPEG images
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    // Textures
    SDL_Texture * backgroundTxt = initialize_texture_from_file("img/background.png", renderer);
    SDL_Texture * normalTXT = initialize_texture_from_file("img/normal.png", renderer);
    SDL_Texture * zompopaTXT = initialize_texture_from_file("img/zompopas.png", renderer);
    SDL_Texture * reinaTXT = initialize_texture_from_file("img/reina.png", renderer);
    SDL_Texture * antTXT = initialize_texture_from_file("img/antIsaac.jpeg", renderer);

    // Define where on the "screen" we want to draw the texture
    SDL_Rect backgroundRect = {0,0,1200,800};

    // GRAPHICS VARIABLES
    int FPS = 60;
    int frametime = 0;
    int sprite = 0;
    bool running = true;
    SDL_Event event;

    // ANTS
    ant_t* ant = (ant_t*) malloc(sizeof(ant_t));
    ant->posX = 80;
    ant->posY = 440;
    ant->vel = 0;
    ant->dest = 1;
    ant->finalDest = -1;
    ant->path = 0;
    ant->type = 1;
    ant->channel = 1;
    ant->time = 10;
    ant->priority = 2;

    ant_t* ant2 = (ant_t*) malloc(sizeof(ant_t));
    ant2->posX = 80;
    ant2->posY = 440;
    ant2->vel = 0;
    ant2->dest = 1;
    ant2->finalDest = -1;
    ant2->path = 0;
    ant2->type = 1;
    ant2->channel = 2;
    ant2->time = 10;
    ant2->priority = 2;

    ant_t* ant3 = (ant_t*) malloc(sizeof(ant_t));
    ant3->posX = 80;
    ant3->posY = 440;
    ant3->vel = 0;
    ant3->dest = 1;
    ant3->finalDest = -1;
    ant3->path = 0;
    ant3->type = 1;
    ant3->channel = 3;
    ant3->time = 10;
    ant3->priority = 2;

    ant_t* ant4 = (ant_t*) malloc(sizeof(ant_t));
    ant4->posX = 1100;
    ant4->posY = 440;
    ant4->vel = 0;
    ant4->dest = 2;
    ant4->finalDest = -1;
    ant4->path = 0;
    ant4->type = 1;
    ant4->channel = 1;
    ant4->time = 10;
    ant4->priority = 2;

    ant_t* ant5 = (ant_t*) malloc(sizeof(ant_t));
    ant5->posX = 1100;
    ant5->posY = 440;
    ant5->vel = 0;
    ant5->dest = 2;
    ant5->finalDest = -1;
    ant5->path = 0;
    ant5->type = 1;
    ant5->channel = 2;
    ant5->time = 10;
    ant5->priority = 2;

    ant_t* ant6 = (ant_t*) malloc(sizeof(ant_t));
    ant6->posX = 1100;
    ant6->posY = 440;
    ant6->vel = 0;
    ant6->dest = 2;
    ant6->finalDest = -1;
    ant6->path = 0;
    ant6->type = 1;
    ant6->channel = 3;
    ant6->time = 10;
    ant6->priority = 2;

    ant_t *antList[6] = {ant,ant2,ant3,ant4,ant5,ant6};

    int canalID = 0;
    int canal1Counter = 0;
    int canal2Counter = 0;
    int canal3Counter = 0;
    int canal4Counter = 0;
    int canal5Counter = 0;
    int canal6Counter = 0;
    
    while(running)
    {
        // Process events
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Clear screen
        SDL_RenderClear(renderer);

        // Draw
        SDL_RenderCopy(renderer, backgroundTxt, NULL, &backgroundRect); // BG

        //SPRITES
        //SDL_Rect spriteNormal = {(sprite % 5)*24,0,24,38};

        for(int i=0; i<6; i++){
            SDL_Rect spriteNormal = {(sprite % 5)*24,0,24,38};
            SDL_Rect normalRect = {antList[i]->posX,antList[i]->posY,24,38}; //SPRITE
            SDL_RenderCopyEx(renderer,normalTXT,&spriteNormal,&normalRect,0,NULL,0);

            if(antList[i]->finalDest == -1){
                if(antList[i]->dest == 1){
                    if(antList[i]->channel == 1){
                        antList[i]->finalDest = canal1[canal1Counter];
                        canal1Counter ++;
                    }
                    if(antList[i]->channel == 2){
                        antList[i]->finalDest = canal2[canal2Counter];
                        canal2Counter ++;
                    }
                    if(antList[i]->channel == 3){
                        antList[i]->finalDest = canal3[canal3Counter];
                        canal3Counter ++;
                    }
                }

                else if(antList[i]->dest == 2){
                    if(antList[i]->channel == 1){
                        antList[i]->finalDest = canal4[canal4Counter];
                        canal4Counter ++;
                    }
                    if(antList[i]->channel == 2){
                        antList[i]->finalDest = canal5[canal5Counter];
                        canal5Counter ++;
                    }
                    if(antList[i]->channel == 3){
                        antList[i]->finalDest = canal6[canal6Counter];
                        canal6Counter ++;
                    }
                }
                else{
                    printf("[ERROR] Ant con destination no available");
                }

            }
            else{
                waze(antList[i]);
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