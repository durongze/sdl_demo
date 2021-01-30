#include "SDL.h"
#include "SDL_main.h"
#include <stdio.h>

#define PrintCtx(fmt, ...)          \
    do {                            \
        printf("[%s:%d]" fmt,         \
             __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0);

#define TO_STR(k) #k

const char* KeyToStr(int key)
{
    const char* keyStr = "undefine";
    switch (key) {
    case SDLK_0:
        keyStr = TO_STR(SDLK_0);
        break;
    case SDLK_1:
        keyStr = TO_STR(SDLK_1);
        break;
    case SDLK_2:
        keyStr = TO_STR(SDLK_2);
        break;
    case SDLK_3:
        keyStr = TO_STR(SDLK_3);
        break;
    case SDLK_4:
        keyStr = TO_STR(SDLK_4);
        break;
    case SDLK_5:
        keyStr = TO_STR(SDLK_5);
        break;
    case SDLK_LEFT:
        keyStr = TO_STR(SDLK_LEFT);
        break;
    case SDLK_RIGHT:
        keyStr = TO_STR(SDLK_RIGHT);
        break;
    case SDLK_DOWN:
        keyStr = TO_STR(SDLK_DOWN);
        break;
    case SDLK_UP:
        keyStr = TO_STR(SDLK_UP);
        break;
    default:
        break;
    }
    return keyStr;
}

int BlitSurface(SDL_Surface* surface, SDL_Surface *pic)
{
    SDL_BlitSurface(pic, NULL, surface, NULL);
    return 0;
}

int ShowRectInfo(SDL_Rect* rect)
{
    PrintCtx("rect x:%d,y:%d,w:%d,h:%d\n", rect->x, rect->y, rect->w, rect->h);
    return 0;
}

int MotionToRect(SDL_MouseMotionEvent* motion, SDL_Rect* rect)
{
    rect->x = motion->x;
    rect->y = motion->y;
    rect->w = 10;
    rect->h = 10;
    return 0;
}

int GetWinPos(SDL_Window* window, SDL_Rect *rect)
{
    SDL_GetWindowPosition(window, &rect->x, &rect->y);
    SDL_GetWindowSize(window, &rect->w, &rect->h);
    return 0;
}

int ProcMouse(SDL_Event event, SDL_Surface *surface, SDL_Rect *rect)
{
    MotionToRect(&event.motion, rect);
    switch (event.button.button) {
    case SDL_BUTTON_LEFT:
        break;
    case SDL_BUTTON_RIGHT:
        break;
    default:
        break;
    }
    ShowRectInfo(rect);
    return 0;
}

int ProcKeyBoard(SDL_Event event, SDL_Surface* surface, SDL_Surface* pic)
{
    switch (event.key.keysym.sym) {
    case SDLK_LEFT:
        break;
    case SDLK_RIGHT:
        break; 
    case SDLK_DOWN:
        break;
    case SDLK_UP:
        break;
    default:    
        break;
    }
    return 0;
}

int ProcMouseMotion(SDL_Event event, SDL_Surface* surface, SDL_Rect* rect)
{
    MotionToRect(&event.motion, rect);

    ShowRectInfo(rect);  // crash

    return 0;
}

int ProcConsole(SDL_Rect *rect)
{
    ShowRectInfo(rect);
    return 0;
}

int ProcWindow(SDL_Event event, SDL_Surface* surface, SDL_Rect* rect)
{
    switch (event.window.event) {
    case SDL_WINDOWEVENT_LEAVE:
        break;
    case SDL_WINDOWEVENT_ENTER:
        ShowRectInfo(rect);
        break;
    default:
        break;
    }

    
    return 0;
}

/* This is where execution begins [console apps, unicode] */
int main(int argc, char **argv)
{
    SDL_Surface* pic1 = NULL, * pic2 = NULL, * cur = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    
    // if (argc == 2)
    {
        pic1 = SDL_LoadBMP("1.bmp");
        // pic2 = SDL_LoadBMP("2.bmp");
        cur = pic1;
    }
    SDL_Window* window = SDL_CreateWindow("myWinApp", 100, 100, cur->w, cur->h, SDL_WINDOW_SHOWN);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    
    SDL_Event event = {0};
    SDL_Rect mouseRect = {0}, winRect = {0}, motionRect = {0};
    while (1) {
        SDL_Delay(300);
        if (NULL == window || NULL == surface) {
            continue;
        }
        SDL_FillRect(surface, NULL, 0);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                PrintCtx("\n");
                break;
            case SDL_MOUSEBUTTONDOWN:
                ProcMouse(event, surface, &mouseRect);
                break;
            case SDL_MOUSEMOTION:
                // ProcMouseMotion(event, surface, &motionRect);
                break;
            case SDL_KEYDOWN:
                ProcKeyBoard(event, surface, pic2);
                break;
            case SDL_WINDOWEVENT:
                GetWinPos(window, &winRect);
                ProcWindow(event, surface, &winRect);
                break;
            default:
                break;
            }
        }
        SDL_BlitSurface(cur, NULL, surface, NULL);
        SDL_FillRect(surface, &mouseRect, 0);
        SDL_UpdateWindowSurface(window);

    }
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

