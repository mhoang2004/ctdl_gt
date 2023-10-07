#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cstdio>

using namespace std;

// Screen dimension constants
const int SCREEN_WIDTH = 1250;
const int SCREEN_HEIGHT = 700;

#include "src/module/texture.h"
#include "src/module/card.h"
#include "src/module/render.h"
#include "src/module/user.h"

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
    // SDL_DestroyTexture(gTexture);
    // gTexture = NULL;

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

        // init user's 13 cards
        User player(plCards);

        // init computer
        vector<User> computers;
        User computer1(plCards);
        User computer2(plCards);
        User computer3(plCards);
        computers.push_back(computer1);
        computers.push_back(computer2);
        computers.push_back(computer3);

        // init users
        vector<User> users;
        users.push_back(computer1);
        users.push_back(computer2);
        users.push_back(computer3);
        users.push_back(player);

        backTexture = loadTexture("src/cards/BACK.png");
        hitBtnTexture = loadTexture("src/image/play.png");
        skipBtnTexture = loadTexture("src/image/skip.png");

        renderBackCard();
        renderComputerCards();

        // Render cards
        player.printCards();

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
                    SDL_Rect cardArea = {50, 560, 80 * player.getCardCount() + 40, 174};
                    if (mouseX >= cardArea.x && mouseX <= cardArea.x + cardArea.w &&
                        mouseY >= cardArea.y && mouseY <= cardArea.y + cardArea.h)
                    {
                        int index;

                        // calculate the index user clicked
                        int minValue = 50;
                        for (int i = 0; i < cardArea.w; i++)
                        {
                            int maxValue = minValue + 80;

                            if (minValue < mouseX && maxValue > mouseX)
                            {
                                index = i;
                                break;
                            }

                            minValue = maxValue;

                            if (index > 12)
                                index = 12;
                        }

                        player.changeSelected(index);

                        SDL_RenderClear(gRenderer);

                        renderBackCard();
                        renderComputerCards();
                        renderHistory(history);

                        player.printCards();

                        SDL_RenderPresent(gRenderer);
                    }

                    SDL_Rect hitBtnArea = {800, 450, 135, 59};
                    if (mouseX >= hitBtnArea.x && mouseX <= hitBtnArea.x + hitBtnArea.w &&
                        mouseY >= hitBtnArea.y && mouseY <= hitBtnArea.y + hitBtnArea.h)
                    {
                        SDL_RenderClear(gRenderer);

                        renderBackCard();
                        renderComputerCards();

                        player.hit();
                        renderHistory(history);

                        player.printCards();

                        SDL_RenderPresent(gRenderer);
                    }
                }

                if (!player.isUserTurn())
                {
                    for (User computer : computers)
                    {
                        SDL_Delay(1000);

                        computer.changeSelected(0);

                        SDL_RenderClear(gRenderer);

                        renderBackCard();
                        renderComputerCards();

                        computer.hit();
                        renderHistory(history);

                        player.printCards();

                        SDL_RenderPresent(gRenderer);
                    }
                    player.setUserTurn(true);
                }
            }
        }
    }
    // Free resources and close SDL
    close();

    return 0;
}