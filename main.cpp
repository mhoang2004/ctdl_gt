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
    bool winByDefault = false;
    if (player.isSpecialCards())
    {
        renderPassWin(player.getWinTexture());
        winByDefault = true;
    }
    for (Computer computer : computers)
    {
        if (computer.isSpecialCards())
        {
            renderPassWin(computer.getWinTexture());
            winByDefault = true;
        }
    }
    return winByDefault;
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

    bool winByDefault = checkWinByDefault(player, computers);

    if (winByDefault)
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

bool doneTurn(User player, vector<Computer> computers)
{
    int count = 0;

    if (player.getSkip())
        count++;
    for (Computer computer : computers)
    {
        if (computer.getSkip())
        {
            count++;
        }
    }

    if (count == 3)
    {
        return true;
    }

    return false;
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

        bool winByDefault = checkWinByDefault(player, computers);

        if (winByDefault)
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

        while (!quit)
        {
            // each turn

            // user play
            while (SDL_PollEvent(&e) != 0)
            {
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

                    if (isGameFinish)
                    {
                        SDL_Rect againBtnArea = {470, 300, 155, 50};
                        if (mouseX >= againBtnArea.x && mouseX <= againBtnArea.x + againBtnArea.w &&
                            mouseY >= againBtnArea.y && mouseY <= againBtnArea.y + againBtnArea.h)
                        {
                            isGameFinish = false;
                            playAgain(plCards, player, computers);
                        }
                    }
                }
            }

            // computer play
            if (!isGameFinish)
            {
                while (!doneTurn(player, computers))
                {
                }
                if (!player.isUserTurn() || player.getIsFinish())
                {
                    for (int i = 0; i < COMPUTER_NUM; i++)
                    {
                        if (!computers[i].getIsFinish() && computers[i].isUserTurn())
                        {
                            SDL_Delay(500);

                            // rand() % 2 (if computers[i] CAN hit?)
                            if ((rand() % 2) && !computers[i].getIsFirst())
                            {
                                computers[i].setUserTurn(false);
                                computers[i].printSkipText(computers[i].getId());
                                computers[i].setSkip(true);
                            }
                            else
                            {
                                // what cards computers[i] will hit
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

                                    if (computer.getSkip())
                                    {
                                        computer.printSkipText(computer.getId());
                                    }
                                }

                                renderHistory(history);

                                if (!player.getIsFinish())
                                {
                                    renderBtn();
                                    player.printCards();

                                    if (player.getSkip())
                                    {
                                        player.printSkipText();
                                    }
                                }
                                else
                                {
                                    player.printWinner();
                                }
                            }

                            SDL_RenderPresent(gRenderer);
                        }

                        int j = i == COMPUTER_NUM - 1 ? 0 : i + 1;
                        for (; j < COMPUTER_NUM; j++)
                        {
                            if (!computers[j].getSkip() && !computers[j].getIsFinish())
                            {
                                computers[j].setUserTurn(true);
                                break;
                            }
                        }
                    }

                    if (!player.getIsFinish() && !player.getSkip())
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

                if (isGameFinish)
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
                }
            }

            if (doneTurn(player, computers))
            {
                if (!player.getSkip())
                {
                    player.setUserTurn(true);
                    player.setIsFirst(true);
                }
                player.setSkip(false);
                for (Computer &computer : computers)
                {
                    if (!computer.getSkip())
                    {
                        computer.setUserTurn(true);
                        player.setIsFirst(true);
                    }
                    computer.setSkip(false);
                }
            }
        }
        // Free resources and close SDL
        close();
    }

    return 0;
}
