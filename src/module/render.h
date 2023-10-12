#pragma once
void renderBtn()
{
    SDL_Rect destinationRect;

    // play btn
    destinationRect = {210, 480, 155, 50};
    SDL_RenderCopy(gRenderer, skipBtnTexture, NULL, &destinationRect);

    // skip btn
    destinationRect = {800, 480, 150, 65};
    SDL_RenderCopy(gRenderer, hitBtnTexture, NULL, &destinationRect);
}

void renderAgainBtn()
{
    SDL_Rect destinationRect;

    // again btn
    destinationRect = {470, 300, 155, 50};
    SDL_RenderCopy(gRenderer, againBtnTexture, NULL, &destinationRect);
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

void renderPassWin(SDL_Texture *winTexture)
{
    SDL_Delay(1000);
    SDL_Rect destinationRect;
    destinationRect = {SCREEN_WIDTH / 2 - 280, SCREEN_HEIGHT / 2 - 280, 250, 250};
    SDL_RenderCopy(gRenderer, winTexture, NULL, &destinationRect);
}