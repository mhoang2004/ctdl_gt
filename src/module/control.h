void skipBtnEvent(User &player, int mouseX, int mouseY)
{
    SDL_Rect skipBtnArea = {210, 440, 135, 59};
    if (mouseX >= skipBtnArea.x && mouseX <= skipBtnArea.x + skipBtnArea.w &&
        mouseY >= skipBtnArea.y && mouseY <= skipBtnArea.y + skipBtnArea.h)
    {
        if (!player.isFirstUser())
        {
            player.setUserTurn(false);
            player.printSkipText();
            player.setSkip(true);
        }
    }
}

void hitBtnEvent(User &player, int mouseX, int mouseY)
{
    if (player.isUserTurn())
    {
        SDL_Rect hitBtnArea = {800, 440, 135, 59};
        if (mouseX >= hitBtnArea.x && mouseX <= hitBtnArea.x + hitBtnArea.w &&
            mouseY >= hitBtnArea.y && mouseY <= hitBtnArea.y + hitBtnArea.h)
        {
            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

            player.hit();
            if (player.checkWin())
                player.setPlace();

            if (!player.getIsFinish())
            {
                renderBtn();
                player.printCards();
            }
            else
            {
                player.printWinner();
            }

            renderHistory(history);
        }
    }
}

void cardSelectEvent(User &player, int mouseX, int mouseY)
{
    // user cards area
    int cardCout = player.getCardCount();
    SDL_Rect cardArea = {50, 560, 80 * cardCout + 40, 174};
    if (mouseX >= cardArea.x && mouseX <= cardArea.x + cardArea.w &&
        mouseY >= cardArea.y && mouseY <= cardArea.y + cardArea.h)
    {
        // index of the card user clicked
        int index;

        // calculate the index user clicked
        int minValue = 50, maxValue;
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
        renderHistory(history);

        player.printCards();
    }
}