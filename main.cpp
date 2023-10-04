/*This source code copyrighted by Lazy Foo' Productions 2004-2023
and may not be redistributed without written permission.*/

#pragma once

// Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cstdio>

#include "src/module/card.h"
#include "src/module/user.h"

using namespace std;

// Screen dimension constants
const int SCREEN_WIDTH = 1250;
const int SCREEN_HEIGHT = 700;

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia(string path);

// Frees media and shuts down SDL
void close();

// Loads individual image as texture
SDL_Texture *loadTexture(string path);

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Current displayed texture
SDL_Texture *gTexture = NULL;

bool init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        // Create window
        gWindow = SDL_CreateWindow("Card game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia(string path)
{
    // Loading success flag
    bool success = true;

    // Load PNG texture

    gTexture = loadTexture(path);
    if (gTexture == NULL)
    {
        printf("Failed to load texture image!\n");
        success = false;
    }

    return success;
}

void close()
{
    // Free loaded image
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *loadTexture(string path)
{
    // The final texture
    SDL_Texture *newTexture = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

int main(int argc, char *args[])
{
    // set a seed
    srand(time(0));

    // init deck
    PlayingCards plCards;

    // shuffle the deck
    plCards.shufflePlayingCards();

    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        // Main loop flag
        bool quit = false;

        // Event handler
        SDL_Event e;

        SDL_SetRenderDrawColor(gRenderer, 34, 139, 34, 255);

        // Clear the entire screen to our selected color.
        SDL_RenderClear(gRenderer);

        // Up until now everything was drawn behind the scenes.
        // This will show the new, red contents of the window.
        SDL_RenderPresent(gRenderer);

        User player;
        player.setUserCards(plCards);
        player.sortCard();

        // Render init cards
        for (int i = 0; i < player.getCardCount(); i++)
        {
            Card currCard = player.getUserCards()[i];

            if (!loadMedia(currCard.getUrl()))
            {
                cout << currCard.getUrl() << endl;
                continue;
            }

            // Render texture to screen
            SDL_Rect destinationRect = {i * 80 + 50, 500, 120, 174};
            SDL_RenderCopy(gRenderer, gTexture, NULL, &destinationRect);
        }

        if (!loadMedia("src/cards/BACK.png"))
        {
            cout << "Fail to load back img" << endl;
            return 1;
        }
        else
        {
            SDL_Rect destinationRect = {50, SCREEN_HEIGHT / 2 - 145, 100, 145};
            SDL_RenderCopy(gRenderer, gTexture, NULL, &destinationRect);

            destinationRect = {SCREEN_WIDTH - 150, SCREEN_HEIGHT / 2 - 145, 100, 145};
            SDL_RenderCopy(gRenderer, gTexture, NULL, &destinationRect);

            destinationRect = {SCREEN_WIDTH / 2 - 100, 5, 100, 145};
            SDL_RenderCopy(gRenderer, gTexture, NULL, &destinationRect);
        }

        SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);

        // Update screen
        SDL_RenderPresent(gRenderer);

        while (!quit)
        {
            // Handle events on queue
            while (SDL_PollEvent(&e) != 0)
            {
                // User requests quit
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
            }
        }
    }

    // Free resources and close SDL
    close();

    return 0;
}