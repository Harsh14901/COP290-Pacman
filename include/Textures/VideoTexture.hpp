#pragma once

#include "SDL2/SDL.h"
#include "iostream"


class VideoTexture
{

public:
    // Initializes variables
    VideoTexture(SDL_Renderer *renderer);
    VideoTexture();
    void setRenderer(SDL_Renderer *renderer);

    // Loads image at specified path
    bool loadFromFile(std::string path);

    // Deallocates texture
    void free();

    // Renders texture at given point
    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0,
                SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    // Override the image dimensions
    void set_video_dimenstions(int width, int height);

    // Gets image dimensions
    int getWidth();
    int getHeight();

private:
    // The actual hardware texture
    SDL_Texture *mTexture;
    SDL_Renderer *_gRenderer;


    int x;
    int y;
    // Image dimensions
    int mWidth;
    int mHeight;

    const static int bpp = 12;

    int screen_w = 500, screen_h = 500;
    const static int pixel_w = 320, pixel_h = 180;

    unsigned char buffer[pixel_w * pixel_h * bpp / 8];

    int thread_exit = 0;

    void stop();
    int startPlaying();
};