void doneTurn(User &player, vector<Computer> &computers)
{
    int count = 0;

    if (player.getSkip())
    {
        count++;
    }
    for (Computer computer : computers)
    {
        if (computer.getSkip())
        {
            count++;
        }
    }

    if (count == 3)
    {
        if (!player.getSkip())
        {
            player.setUserTurn(true);
            player.setIsFirst(true);
        }
        else
        {
            player.setUserTurn(false);
        }

        player.setSkip(false);

        for (Computer &computer : computers)
        {
            if (!computer.getSkip())
            {
                computer.setUserTurn(true);
                computer.setIsFirst(true);
            }
            else
            {
                computer.setUserTurn(false);
            }

            computer.setSkip(false);
        }

        if (player.isUserTurn())
        {
            computers[0].setUserTurn(true);
        }

        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

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

        if (player.getSkip())
        {
            player.printSkipText();
        }

        cout << "------------------" << endl;
        cout << "Player: " << player.getIsFirst() << endl;

        // print skip text and back cards
        for (Computer computer : computers)
        {
            computer.printBackCard();
            if (computer.getSkip())
            {
                computer.printSkipText(computer.getId());
            }

            cout << "Com0: " << computer.getIsFirst() << endl;
        }
        cout << "------------------" << endl;

        SDL_Delay(500);
        SDL_RenderPresent(gRenderer);
    }
}