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

bool checkWinByDefault(User player, vector<Computer> computers)
{
    bool winByDefalt = false;
    if (player.isSpecialCards())
    {
        renderPassWin(player.getWinTexture());
        winByDefalt = true;
    }
    for (Computer computer : computers)
    {
        if (computer.isSpecialCards())
        {
            renderPassWin(computer.getWinTexture());
            winByDefalt = true;
        }
    }
    return winByDefalt;
}

void playAgain(PlayingCards &plCards, User &player, vector<Computer> &computers)
{
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

    srand(time(0));

    plCards.createPlayingCards();
    plCards.shufflePlayingCards();

    player.initUser(plCards);

    for (Computer &computer : computers)
    {
        computer.initUser(plCards);
    }

    bool winByDefalt = checkWinByDefault(player, computers);

    if (winByDefalt)
    {
        SDL_RenderPresent(gRenderer);
        SDL_Delay(2000);

        playAgain(plCards, player, computers);
    }
    else
    {
        for (Computer computer : computers)
            computer.printBackCard();

        renderBtn();

        player.printCards();

        SDL_RenderPresent(gRenderer);

        if (player.isUserTurn())
        {
            computers[0].setUserTurn(true);
        }
    }
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
        // Main loop flag
        bool quit = false;

        // Event handler
        SDL_Event e;

        // set background
        loadBackground();

        // set a seed
        srand(time(0));

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

        bool winByDefalt = checkWinByDefault(player, computers);

        if (winByDefalt)
        {
            SDL_RenderPresent(gRenderer);
            SDL_Delay(2000);

            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

            playAgain(plCards, player, computers);
        }
        else
        {
            backTexture = loadTexture("src/cards/BACK.png");
            for (Computer computer : computers)
                computer.printBackCard();

            hitBtnTexture = loadTexture("src/image/play.png");
            skipBtnTexture = loadTexture("src/image/skip.png");
            againBtnTexture = loadTexture("src/image/again.png");

            renderBtn();
            player.printCards();

            SDL_RenderPresent(gRenderer);

            if (player.isUserTurn())
            {
                computers[0].setUserTurn(true);
            }
        }

        // check if finished A GAME
        bool isGameFinish = false;

        // only render once when A GAME done
        bool forEnventHandler = true;

        SDL_Delay(2000);
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

                    // handle events
                    if (!player.getIsFinish())
                    {
                        // event handler

                        cardSelectEvent(player, mouseX, mouseY);
                        hitBtnEvent(player, mouseX, mouseY);
                        skipBtnEvent(player, mouseX, mouseY);

                        for (Computer computer : computers)
                            computer.printBackCard();

                        SDL_RenderPresent(gRenderer);
                    }

                    SDL_Rect againBtnArea = {470, 300, 155, 50};
                    if (mouseX >= againBtnArea.x && mouseX <= againBtnArea.x + againBtnArea.w &&
                        mouseY >= againBtnArea.y && mouseY <= againBtnArea.y + againBtnArea.h)
                    {
                        cout << "isGameFinish1: " << isGameFinish << endl;
                        if (isGameFinish)
                        {
                            cout << "isGameFinish2: " << isGameFinish << endl;

                            isGameFinish = false;
                            forEnventHandler = true;

                            playAgain(plCards, player, computers);
                        }
                    }
                }
            }

            // each turn
            if (!isGameFinish)
            {
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
                                else
                                {
                                    SDL_Delay(500);

                                    // from which computers[i]?
                                    computers[i].animationCard(computers[i].getId());
                                }

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
                        {
                            if (!computers[0].getIsFinish())
                            {
                                computers[0].setUserTurn(true);
                            }
                        }
                        else
                        {
                            if (!computers[i + 1].getIsFinish())
                            {
                                computers[i + 1].setUserTurn(true);
                            }
                        }
                    }

                    if (!player.getIsFinish())
                        player.setUserTurn(true);
                }

                isGameFinish = true;
                for (Computer computer : computers)
                {
                    if (!computer.getIsFinish())
                    {
                        isGameFinish = false;
                        break;
                    }
                }
            }

            if (isGameFinish && forEnventHandler)
            {
                SDL_RenderClear(gRenderer);
                SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

                for (Computer computer : computers)
                {
                    computer.printWinner();
                }
                player.printWinner();

                renderAgainBtn();

                SDL_RenderPresent(gRenderer);

                forEnventHandler = false;
            }
        }

        // Free resources and close SDL
        close();
    }

    return 0;
}
