#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cstdio>

using namespace std;

#include "src/module/loadImage.h"

#include "src/module/card.h"
#include "src/module/user.h"

// Screen dimension constants
const int SCREEN_WIDTH = 1250;
const int SCREEN_HEIGHT = 700;

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

int main(int argc, char *args[])
{
    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        // set a seed
        srand(time(0));

        // init shuffle deck
        PlayingCards plCards;

        // Main loop flag
        bool quit = false;

        // set background
        SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
        SDL_RenderClear(gRenderer);
        SDL_RenderPresent(gRenderer);

        // Event handler
        SDL_Event e;

        User player;

        // init 13 cards
        player.setUserCards(plCards);
        player.sortCard();

        // Render init cards
        for (int i = 0; i < player.getCardCount(); i++)
        {
            Card currCard = player.getUserCards()[i];
            // padding each card is 80 (change here!!)
            currCard.setX(i * 80 + 50);

            // Render texture to screen
            SDL_RenderCopy(gRenderer, currCard.getTexture(), NULL, currCard.getDestinationRect());
        }

        if (!loadMedia("src/cards/BACK.png"))
        {
            cout << "Fail to load back img" << endl;
            return 1;
        }
        else
        {
            // where computer's cards will appear
            SDL_Rect destinationRect;

            destinationRect = {50, SCREEN_HEIGHT / 2 - 145, 100, 145};
            SDL_RenderCopy(gRenderer, gTexture, NULL, &destinationRect);

            destinationRect = {SCREEN_WIDTH - 200, SCREEN_HEIGHT / 2 - 145, 100, 145};
            SDL_RenderCopy(gRenderer, gTexture, NULL, &destinationRect);

            destinationRect = {SCREEN_WIDTH / 2 - 100, 5, 100, 145};
            SDL_RenderCopy(gRenderer, gTexture, NULL, &destinationRect);
        }

        if (!loadMedia("src/image/skip.png"))
        {
            cout << "Fail to load skip img" << endl;
            return 1;
        }
        else
        {
            // where the button will appear
            SDL_Rect destinationRect;

            destinationRect = {210, 450, 135, 59};
            SDL_RenderCopy(gRenderer, gTexture, NULL, &destinationRect);
        }

        if (!loadMedia("src/image/play.png"))
        {
            cout << "Fail to load play img" << endl;
            return 1;
        }
        else
        {
            // where the button will appear
            SDL_Rect destinationRect;

            destinationRect = {800, 450, 135, 59};
            SDL_RenderCopy(gRenderer, gTexture, NULL, &destinationRect);
        }

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

                if (e.type == SDL_MOUSEBUTTONUP)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    // user cards area
                    SDL_Rect rectangle = {50, 560, 120 * player.getCardCount(), 174};
                    if (mouseX >= rectangle.x && mouseX <= rectangle.x + rectangle.w &&
                        mouseY >= rectangle.y && mouseY <= rectangle.y + rectangle.h)
                    {
                        for (int i = 0; i < player.getCardCount(); i++)
                        {
                            Card currCard = player.getUserCards()[i];
                            // padding each card is 80 (change here!!)
                            currCard.setX(i * 80 + 50);

                            if (i == 11)
                            {
                                currCard.setY(525);
                            }

                            // Render texture to screen
                            SDL_RenderCopy(gRenderer, currCard.getTexture(), NULL, currCard.getDestinationRect());
                        }
                    }
                }
            }
        }
    }

    // Free resources and close SDL
    close();

    return 0;
}