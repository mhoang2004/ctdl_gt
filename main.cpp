#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
        SDL_Texture* backgroundTexture = nullptr;
        //tao bo dem backgroundTexture de luu tru background (SDL_PIXELFORMAT_RGBA8888 định dạng pixel cho texture, SDL_TEXTUREACCESS_TARGET vẽ lên texture, SDL_TEXTUREACCESS_)
        backgroundTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
       
        
        SDL_Surface* backgroundSurface = IMG_Load("src/image/OIP.png"); // Thay bằng đường dẫn đến hình ảnh của bạn
        if (backgroundSurface == nullptr) {
            printf("Failed to load image! SDL_image Error: %s\n", IMG_GetError());
        }
        else
        {
            backgroundTexture = SDL_CreateTextureFromSurface(gRenderer, backgroundSurface);// Chuyển đổi hình ảnh Surface sang Texture
            SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);
            //có NULL vì muốn thao tác với toàn bộ nội dung của texture hoặc renderer, mà không muốn áp đặt giới hạn về vị trí và kích thước.
            SDL_FreeSurface(backgroundSurface);
        }

        // Event handler
        SDL_Event e;

        // init user's 13 cards
        User player(plCards);

        // init computer
        Computer computer1(1, plCards);
        Computer computer2(2, plCards);
        Computer computer3(3, plCards);

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
        for(int i = 0; i < 1; i++)
        {
            if(player.isSpecialCards())
            {
                passWin = loadTexture("src/image/win.png");
                break;
            }
            if(computer1.isSpecialCards())
            {
                passWin = loadTexture("src/image/win1.png");
                break;
            }
            if(computer2.isSpecialCards())
            {
                passWin = loadTexture("src/image/win2.png");
                break;
            }
            if(computer3.isSpecialCards())
            {
                passWin = loadTexture("src/image/win3.png");
                break;
            }
            checkPass = -1;
            
        }

        if(checkPass == 1)
        {
            renderPassWin();
            SDL_RenderPresent(gRenderer);
            sleep(3);
            quit = true;
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
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    // user cards area
                    int cardCout = player.getCardCount();
                    SDL_Rect cardArea = {50, 560, 80 * cardCout + 40, 174};
                    if (mouseX >= cardArea.x && mouseX <= cardArea.x + cardArea.w &&
                        mouseY >= cardArea.y && mouseY <= cardArea.y + cardArea.h)
                    {
                        int index;

                        // calculate the index user clicked
                        int minValue = 50;
                        int maxValue;
                        for (int i = 0; i < cardCout; i++)
                        {
                            maxValue = minValue + 80;

                            if (minValue < mouseX && maxValue > mouseX)
                            {
                                index = i;
                                break;
                            }

                            if (i == cardCout - 1)
                            {
                                index = cardCout - 1;
                                break;
                            }

                            minValue = maxValue;
                        }

                        player.changeSelected(index);

                        SDL_RenderClear(gRenderer);
                        SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

                        renderBtn();
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

                        player.setUserTurn(true);
                    }
                }
            }
        }

        // Free resources and close SDL
        close();

        return 0;
    }
}
