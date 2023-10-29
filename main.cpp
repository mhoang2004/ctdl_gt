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
#include "src/module/user.h"
#include "src/module/logic_game.h"
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

    history.clear();
    gameResult.clear();

    srand(time(0));

    themeCard = themesCard[rand() % THEME_NUM];

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

        renderHitBtn();
        if (!player.getIsFirst())
        {
            renderSkipBtn();
        }
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

        // theme of the game
        themeCard = themesCard[rand() % THEME_NUM];

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
            string backPath = "src/cards/" + themeCard + "BACK.png";
            backTexture = loadTexture(backPath);
            for (Computer computer : computers)
                computer.printBackCard();

            hitBtnTexture = loadTexture("src/image/play.png");
            skipBtnTexture = loadTexture("src/image/skip.png");
            againBtnTexture = loadTexture("src/image/again.png");

            renderHitBtn();
            if (!player.getIsFirst())
            {
                renderSkipBtn();
            }
            player.printCards();

            SDL_RenderPresent(gRenderer);

            if (player.isUserTurn())
            {
                computers[0].setUserTurn(true);
            }
        }

        // check if finished A GAME
        bool isGameFinish = false;

        // game loop
        while (!quit)
        {
            // user play -----------------------------------------------------------------
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
                        // doneTurn(player, computers);

                        // event handler
                        cardSelectEvent(player, computers, mouseX, mouseY);

                        if (mouseX >= skipBtnArea.x && mouseX <= skipBtnArea.x + skipBtnArea.w &&
                            mouseY >= skipBtnArea.y && mouseY <= skipBtnArea.y + skipBtnArea.h)
                        {
                            skipBtnEvent(player, computers);
                        }

                        if (mouseX >= hitBtnArea.x && mouseX <= hitBtnArea.x + hitBtnArea.w &&
                            mouseY >= hitBtnArea.y && mouseY <= hitBtnArea.y + hitBtnArea.h)
                        {

                            hitBtnEvent(player, computers);
                        }

                        SDL_RenderPresent(gRenderer);
                    }

                    // play again btn event
                    if (isGameFinish)
                    {
                        if (mouseX >= againBtnArea.x && mouseX <= againBtnArea.x + againBtnArea.w &&
                            mouseY >= againBtnArea.y && mouseY <= againBtnArea.y + againBtnArea.h)
                        {
                            isGameFinish = false;
                            playAgain(plCards, player, computers);
                        }
                    }
                }
                if (e.type == SDL_KEYUP)
                {
                    if (e.key.keysym.sym == SDLK_RETURN) // press Enter
                    {
                        hitBtnEvent(player, computers);
                    }

                    if (e.key.keysym.sym == SDLK_SPACE) // press Space
                    {
                        skipBtnEvent(player, computers);
                    }

                    if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) // 0 - 9 numbers
                    {
                        // The number key was pressed
                        int index = e.key.keysym.sym - SDLK_0 - 1; // Get the numeric value

                        player.changeSelected(index);

                        renderSelectEvent(player, computers);
                    }

                    SDL_RenderPresent(gRenderer);
                }
            }

            // computer play
            if (!isGameFinish)
            {
                doneTurn(player, computers);

                if (!player.isUserTurn() || player.getIsFinish())
                {
                    for (int i = 0; i < COMPUTER_NUM; i++)
                    {
                        if (!computers[i].getIsFinish() && computers[i].isUserTurn())
                        {
                            SDL_Delay(500);

                            doneTurn(player, computers);
                            bool check_com = true;
                            // rand() % 2 (if computers[i] CAN hit?)
                            if ((int)history.size() == 0)
                            {
                                check_com = false;
                            }
                            else
                            {
                                check_com = check_computer(computers[i].getUserCards(), history[(int)history.size() - 1]);
                            }
                            if (check_com && !computers[i].getIsFirst())
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
                                    SDL_Delay(400);

                                    // from which computer?
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
                                    renderHitBtn();
                                    if (!player.getIsFirst())
                                    {
                                        renderSkipBtn();
                                    }

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

                            // set next computer
                            int j = i + 1;
                            int countNextTurn = 0;
                            while (countNextTurn != COMPUTER_NUM)
                            {
                                if (j == COMPUTER_NUM) // 3
                                {
                                    j = 0;
                                }

                                if (!computers[j].getSkip() && !computers[j].getIsFinish())
                                {
                                    computers[j].setUserTurn(true);
                                    break;
                                }

                                countNextTurn++;
                                j++;
                            }
                        }
                    }

                    if (!player.getIsFinish() && !player.getSkip())
                    {
                        player.setUserTurn(true);
                    }

                    doneTurn(player, computers);
                }
            }

            //----------------------------------------------------------------------------

            // check if game finish
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

        // Free resources and close SDL
        close();
    }

    return 0;
}