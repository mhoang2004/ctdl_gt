#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>

using namespace std;

#include "src/module/global.h"
#include "src/module/init.h"
#include "src/module/close.h"
#include "src/module/texture.h"
#include "src/module/card.h"
#include "src/module/render.h"
#include "src/module/logic_game.h"
#include "src/module/user.h"
#include "src/module/control.h"

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

        // Main loop flag
        bool quit = false;

        // set background
        loadBackground();

        // Event handler
        SDL_Event e;

        // init shuffle deck
        PlayingCards plCards;

        // init user
        User player(plCards);

        // init computers
        vector<Computer> computers;
        for (int i = 0; i < COMPUTER_NUM; i++)
        {
            // init id and 13 cards
            Computer temp(i + 1, plCards);
            computers.push_back(temp);
        }

        if (winTexture)
        {
            renderPassWin();
            SDL_RenderPresent(gRenderer);
            SDL_Delay(2000);
            quit = true;
        }
        else
        {
            backTexture = loadTexture("src/cards/BACK.png");
            renderBackTexture();

            hitBtnTexture = loadTexture("src/image/play.png");
            skipBtnTexture = loadTexture("src/image/skip.png");
            renderBtn();

            // Render cards
            player.printCards();

            // Update screen
            SDL_RenderPresent(gRenderer);
        }

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

                bool isPlayerFinish = player.getIsFinish();
                if (e.type == SDL_MOUSEBUTTONUP)
                {
                    // handle events
                    if (!isPlayerFinish)
                    {
                        // event handler
                        int mouseX, mouseY;
                        SDL_GetMouseState(&mouseX, &mouseY);

                        cardSelectEvent(player, mouseX, mouseY);
                        hitBtnEvent(player, mouseX, mouseY);
                        skipBtnEvent(player, mouseX, mouseY);
                    }
                }

                for (Computer &computer : computers)
                    computer.printBackCard();

                // each turn
                if (!player.isUserTurn() || isPlayerFinish)
                {
                    for (Computer &computer : computers)
                    {
                        SDL_Delay(500);

                        // logic computer (how computer play?)
                        if (rand() % 2)
                        {
                            computer.printSkipText(computer.getId());
                        }
                        else
                        {
                            if (computer.getCardCount() > 0)
                                computer.changeSelected(0);

                            computer.hit();
                            if (!computer.getIsFinish())
                            {
                                // from which computer?
                                computer.animationCard(computer.getId());

                                SDL_RenderClear(gRenderer);
                                SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);
                                renderHistory(history);

                                computer.printBackCard();
                            }
                            else
                            {
                                computer.printWinner();
                            }

                            if (!player.getIsFinish())
                            {
                                renderBtn();
                                player.printCards();
                            }
                            else
                            {
                                player.printWinner();
                            }

                            SDL_RenderPresent(gRenderer);
                        }
                    }

                    player.setUserTurn(true);
                }
            }
        }

        // Free resources and close SDL
        close();

        return 0;
    }
}
