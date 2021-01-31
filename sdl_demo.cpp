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

int ProcMouse(SDL_Event event, SDL_Rect *rect)
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

int ProcKeyBoard(SDL_Event event)
{
    PrintCtx("key:%s\n", KeyToStr(event.key.keysym.sym));
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

int ProcMouseMotion(SDL_Event event, SDL_Rect* rect)
{
    // MotionToRect(&event.motion, rect);
    // PrintCtx("rect x:%d\n", rect->x);
    // PrintCtx("rect y:%d\n", rect->y);

    return 0;
}

int ShowWindowInfo(HWND hwd, PWCHAR title, RECT rect)
{
    PrintCtx("hwd:%p,title:%s\n", hwd, title);
    PrintCtx("hwdRect l:%d,t:%d,r:%d,b:%d\n", rect.left, rect.top, rect.right, rect.bottom);
    return 0;
}

int ProcConsole(SDL_Rect *rect)
{
    WCHAR title[256];
    RECT hwdRect = { 0 };
    HWND hwd = GetConsoleWindow();
    GetConsoleTitle(title, sizeof(title));
    GetWindowRect(hwd, &hwdRect);
    ShowWindowInfo(hwd, title, hwdRect);
    MoveWindow(hwd, rect->x + rect->w, rect->y, 600, rect->h, 1);
    return 0;
}

int ProcWindow(SDL_Event event, SDL_Rect* rect)
{
    
    WCHAR title[256] = {0};

    switch (event.window.event) {
    case SDL_WINDOWEVENT_LEAVE:
        break;
    case SDL_WINDOWEVENT_ENTER:
        break;
    default:
        break;
    }
    ProcConsole(rect);
    
    return 0;
}

typedef unsigned char byte;

typedef struct {
    byte* y;
    byte* u;
    byte* v;
    int w;
    int h;
}Yuv;

int yuv_fill_line(byte* b, size_t bsz, size_t cross, size_t crossIdx, byte val, size_t specCol)
{
    size_t idxCol;
    size_t width = bsz;

    cross = (cross == 0) ? 1 : cross;
    specCol = specCol > (width / cross) ? 0 : specCol;

    for (idxCol = 0; idxCol < width; ++idxCol) {
        if (idxCol % cross == crossIdx) {
            if (idxCol / cross == specCol) {
                b[idxCol] = val;
            }
            else {
                b[idxCol] = idxCol / cross;
            }
        }
    }
    return 0;
}

int yuv_fill(byte* b, size_t bsz, size_t width, size_t height,
    size_t cross, size_t crossIdx, int val, size_t specCol)
{
    size_t idxRow;
    for (idxRow = 0; idxRow < height; ++idxRow) {
        yuv_fill_line(b + idxRow * width, width, cross, crossIdx, val, specCol);
    }
    return 0;
}

int FillYuv(Yuv* yuv)
{
    static size_t specCol = 0;
    specCol++;
    size_t yuvSize = yuv->w * yuv->h;
    yuv_fill(yuv->y, yuvSize, yuv->w, yuv->h,     1, 0, 11, specCol);
    yuv_fill(yuv->u, yuvSize, yuv->w, yuv->h / 2, 2, 0, 22, specCol);
    yuv_fill(yuv->v, yuvSize, yuv->w, yuv->h / 2, 2, 1, 33, specCol);
    return 0;
}

Yuv *GenYuv(int w, int h)
{
    Yuv* yuv = (Yuv*)malloc(sizeof(Yuv));
    if (yuv == NULL) {
        return NULL;
    }
    yuv->w = w;
    yuv->h = h;
    size_t yuvSize = yuv->w * yuv->h;
    yuv->y = (byte*)malloc(yuvSize * sizeof(byte) * 3);
    yuv->u = yuv->y + yuvSize; // (byte*)malloc(yuvSize * sizeof(byte));
    yuv->v = yuv->u + yuvSize; // (byte*)malloc(yuvSize * sizeof(byte));
    FillYuv(yuv);
    return yuv;
}

#define SURFACE

/* This is where execution begins [console apps, unicode] */
int
console_wmain(int argc, wchar_t *wargv[], wchar_t *wenvp)
{
    SDL_Surface* pic1 = NULL, * pic2 = NULL, * cur = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    
    // if (argc == 2)
    {
        pic1 = SDL_LoadBMP("1.bmp");
        pic2 = SDL_LoadBMP("2.bmp");
        cur = pic2;
    }


#ifdef SURFACE
    SDL_Window* window = SDL_CreateWindow("win", 100, 100, cur->w, cur->h, SDL_WINDOW_SHOWN);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
#else
    Yuv* yuv = GenYuv(480, 320);
    SDL_Window* window = SDL_CreateWindow("win", 100, 100, yuv->w, yuv->h, SDL_WINDOW_SHOWN);
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_NV12, SDL_TEXTUREACCESS_STREAMING, yuv->w, yuv->h);
#endif
    SDL_Event event;
    SDL_Rect mouseRect, winRect;
    while (1) {
        SDL_Delay(300);
#ifdef SURFACE
        SDL_FillRect(surface, NULL, 0);
#else
        FillYuv(yuv);
        SDL_RenderClear(render);
#endif
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                PrintCtx("\n");
                break;
            case SDL_MOUSEBUTTONDOWN:
                ProcMouse(event, &mouseRect);
                break;
            case SDL_MOUSEMOTION:
                ProcMouseMotion(event, &mouseRect);
                break;
            case SDL_KEYDOWN:
                ProcKeyBoard(event);
                break;
            case SDL_WINDOWEVENT:
                GetWinPos(window, &winRect);
                ProcWindow(event, &winRect);
                break;
            default:
                break;
            }
        }
#ifdef SURFACE
        SDL_BlitSurface(cur, NULL, surface, NULL);
        SDL_FillRect(surface, &mouseRect, 0);
        SDL_UpdateWindowSurface(window);
#else
        SDL_UpdateTexture(texture, NULL, 
            // yuv->y, yuv->w,
            // yuv->u, yuv->w / 2,
            yuv->y, yuv->w
        );
        SDL_RenderCopy(render, texture, NULL, NULL);
        SDL_RenderPresent(render);
#endif        
    }
#ifdef SURFACE
    SDL_FreeSurface(surface);
#else
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(render);
#endif
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

