#pragma once

void renderHitBtn()
{
    SDL_Rect destinationRect;
    destinationRect = {800, 440, 150, 65};
    SDL_RenderCopy(gRenderer, hitBtnTexture, NULL, &destinationRect);
}

void renderSkipBtn()
{
    SDL_Rect destinationRect;
    destinationRect = {210, 440, 155, 50};
    SDL_RenderCopy(gRenderer, skipBtnTexture, NULL, &destinationRect);
}
void renderAgainBtn()
{
    SDL_Rect destinationRect;
    destinationRect = {530, 320, 155, 50};
    SDL_RenderCopy(gRenderer, againBtnTexture, NULL, &destinationRect);
}

// history area
void renderHistory(vector<vector<Card>> history)
{
    int padding = 0;
    int len = history.size();
    int numberOfCards = 0;
    for (int i = 0; i < len; i++)
    {
        padding += 70;
        numberOfCards += history[i].size();
        for (Card card : history[i])
        {
            card.setWidth(96);
            card.setHeight(139);

            if (i != len - 1)
                card.blackColor();
            else
                card.normalColor();

            if (numberOfCards >= 13)
            {
                padding = 80;
                card.setX((SCREEN_WIDTH / 6) + padding);
            }
            else
            {
                card.setX((SCREEN_WIDTH / 6) + padding);
            }
            card.setY(250);

            SDL_RenderCopy(gRenderer, card.getTexture(), NULL, card.getDestinationRect());

            padding += 40;
        }
    }
}

void renderPassWin(SDL_Texture *winTexture, int padding = 0)
{
    SDL_Delay(1000);
    SDL_Rect destinationRect;
    destinationRect = {SCREEN_WIDTH / 2 - 280 + (210 * (padding + 1)), SCREEN_HEIGHT / 2 - 280, 200, 200};
    SDL_RenderCopy(gRenderer, winTexture, NULL, &destinationRect);
}