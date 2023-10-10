#pragma once
void renderBtn()
{
    SDL_Rect destinationRect;

    // play btn
    destinationRect = {210, 450, 135, 59};
    SDL_RenderCopy(gRenderer, skipBtnTexture, NULL, &destinationRect);

    // skip btn
    destinationRect = {800, 450, 135, 59};
    SDL_RenderCopy(gRenderer, hitBtnTexture, NULL, &destinationRect);
}

void renderComputerCards()
{
    SDL_Rect destinationRect;

    int backCardWidth = 80;
    int backCardHeight = 116;

    // init 3 cards

    // id = 1
    destinationRect = {50, SCREEN_HEIGHT / 2 - 145, backCardWidth, backCardHeight};
    SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);

    // id = 2
    destinationRect = {SCREEN_WIDTH / 2 - 100, 5, backCardWidth, backCardHeight};
    SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);

    // id = 3
    destinationRect = {SCREEN_WIDTH - 200, SCREEN_HEIGHT / 2 - 145, backCardWidth, backCardHeight};
    SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
}

// history area

void renderHistory(vector<vector<Card>> history)
{
    int padding = 0;
    int len = history.size();
    for (int i = 0; i < len; i++)
    {
        padding += 50;
        for (Card card : history[i])
        {

            card.setWidth(80);
            card.setHeight(116);

            if (i != len - 1)
                card.blackColor();
            else
                card.normalColor();

            card.setX((SCREEN_WIDTH / 4) + padding);
            card.setY(250);

            SDL_RenderCopy(gRenderer, card.getTexture(), NULL, card.getDestinationRect());

            padding += 40;
        }
    }
}

void renderPassWin()
{
    SDL_Delay(1000);
    SDL_Rect destinationRect;
    destinationRect = {SCREEN_WIDTH / 2 - 280, SCREEN_HEIGHT / 2 - 280, 500, 500};
    SDL_RenderCopy(gRenderer, passWin, NULL, &destinationRect);
}
