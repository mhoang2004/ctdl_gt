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
        computers.push_back(computer1);
        computers.push_back(computer2);
        computers.push_back(computer3);

        backTexture = loadTexture("src/cards/BACK.png");
        hitBtnTexture = loadTexture("src/image/play.png");
        skipBtnTexture = loadTexture("src/image/skip.png");

        renderBtn();
        renderComputerCards();

        // Render cards
        player.printCards();

        // Update screen
        SDL_RenderPresent(gRenderer);

        int checkPass = 1;
        player.checkSpecialCards();
        computer1.checkSpecialCards();
        computer2.checkSpecialCards();
        computer3.checkSpecialCards();
        for (int i = 0; i < 1; i++)
        {
            if (player.isSpecialCards())
            {
                passWin = loadTexture("src/image/win.png");
                break;
            }
            if (computer1.isSpecialCards())
            {
                passWin = loadTexture("src/image/win1.png");
                break;
            }
            if (computer2.isSpecialCards())
            {
                passWin = loadTexture("src/image/win2.png");
                break;
            }
            if (computer3.isSpecialCards())
            {
                passWin = loadTexture("src/image/win3.png");
                break;
            }
            checkPass = -1;
        }

        if (checkPass == 1)
        {
            renderPassWin();
            SDL_RenderPresent(gRenderer);
            sleep(3);
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

                                player.printCards();

                                SDL_RenderPresent(gRenderer);
                            }

                            SDL_Rect hitBtnArea = {800, 450, 135, 59};
                            if (mouseX >= hitBtnArea.x && mouseX <= hitBtnArea.x + hitBtnArea.w &&
                                mouseY >= hitBtnArea.y && mouseY <= hitBtnArea.y + hitBtnArea.h)
                            {
                                SDL_RenderClear(gRenderer);
                                SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

                                renderBtn();
                                renderComputerCards();

                                player.hit();
                                renderHistory(history);

                                player.printCards();

                                SDL_RenderPresent(gRenderer);
                            }

                            SDL_Rect skipBtnArea = {210, 450, 135, 59};
                            if (mouseX >= skipBtnArea.x && mouseX <= skipBtnArea.x + skipBtnArea.w &&
                                mouseY >= skipBtnArea.y && mouseY <= skipBtnArea.y + skipBtnArea.h)
                            {
                                player.setUserTurn(false);
                            }

                            if (!player.isUserTurn())
                            {
                                for (int i = 0; i < 3; i++)
                                {
                                    SDL_Delay(500);

                                    computers[i].changeSelected(0);
                                    computers[i].changeSelected(1);

                                    computers[i].hit();

                                    // from which computer?
                                    computers[i].animationCard(computers[i].getId());

                                    SDL_RenderClear(gRenderer);
                                    SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

                                    renderBtn();
                                    renderComputerCards();
                                    player.printCards();

                                    renderHistory(history);

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
