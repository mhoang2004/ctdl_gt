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

        // game init
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
            for (Computer computer : computers)
                computer.printBackCard();

            hitBtnTexture = loadTexture("src/image/play.png");
            skipBtnTexture = loadTexture("src/image/skip.png");
            renderBtn();

            // Render cards
            player.printCards();

            // Update screen
            SDL_RenderPresent(gRenderer);

            if (player.isUserTurn())
            {
                computers[0].setUserTurn(true);
            }
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

                if (e.type == SDL_MOUSEBUTTONUP)
                {
                    // handle events
                    if (!player.getIsFinish())
                    {
                        // event handler
                        int mouseX, mouseY;
                        SDL_GetMouseState(&mouseX, &mouseY);

                        cardSelectEvent(player, mouseX, mouseY);
                        hitBtnEvent(player, mouseX, mouseY);
                        skipBtnEvent(player, mouseX, mouseY);

                        for (Computer computer : computers)
                            computer.printBackCard();

                        SDL_RenderPresent(gRenderer);
                    }
                }
            }

            // each turn
            if (!player.isUserTurn() || player.getIsFinish())
            {
                for (int i = 0; i < COMPUTER_NUM; i++)
                {
                    if (!computers[i].getIsFinish() && computers[i].isUserTurn())
                    {
                        SDL_Delay(500);

                        // logic computers[i] (how computers[i] play?)
                        if (0)
                        {
                            computers[i].printSkipText(computers[i].getId());
                        }
                        else
                        {
                            // what computers[i] will hit
                            computers[i].changeSelected(0);

                            computers[i].hit();
                            if (computers[i].checkWin())
                            {
                                computers[i].setPlace();
                            }

                            SDL_Delay(500);

                            // from which computers[i]?
                            computers[i].animationCard(computers[i].getId());

                            SDL_RenderClear(gRenderer);
                            SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

                            // render computer card
                            for (Computer computer : computers)
                            {
                                if (!computer.getIsFinish())
                                {
                                    computer.printBackCard();
                                }
                                else
                                {
                                    computer.printWinner();
                                }
                            }

                            renderHistory(history);

                            if (!player.getIsFinish())
                            {
                                renderBtn();
                                player.printCards();
                            }
                            else
                            {
                                player.printWinner();
                            }
                        }

                        SDL_RenderPresent(gRenderer);
                    }
                    if (i == COMPUTER_NUM - 1)
                        computers[0].setUserTurn(true);
                    else
                        computers[i + 1].setUserTurn(true);
                }
                player.setUserTurn(true);
            }

            bool flag = true;
            for (Computer computer : computers)
            {
                if (!computer.getIsFinish())
                    flag = false;
            }

            if (flag)
            {
                SDL_RenderClear(gRenderer);
                SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

                // render computer card
                for (Computer computer : computers)
                {
                    computer.printWinner();
                }
                player.printWinner();
                SDL_RenderPresent(gRenderer);

                SDL_Delay(5000);
                quit = true;
            }
        }
        // Free resources and close SDL
        close();
    }

    return 0;
}
