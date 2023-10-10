#pragma once
#include <bits/stdc++.h>
#include <algorithm>

#include "algorithms.h"
using namespace std;

vector<vector<Card>> history;

class User
{
private:
    vector<Card> userCards;

    // index of selected cards
    vector<int> selectedCards;

    int money = 0;
    int cardCount = 13;

    bool isFirst = false;
    bool isSpecial = false;
    bool isSkip = false;
    bool isTurn = false;
    bool isFinish = false;

public:
    User(PlayingCards &plCards)
    {
        for (int i = 0; i < 13; i++)
        {
            Card lastCard = plCards.get1Card();
            this->userCards.push_back(lastCard);
        }

        checkSpecialCards();

        if (isSpecial)
        {
            this->setWinTexture();
            return;
        }

        isFirstUser();
    }

    void sortCard()
    {
        mergeSort(this->userCards, 0, this->cardCount - 1);
    }

    vector<Card> getUserCards()
    {
        return userCards;
    }

    int getCardCount()
    {
        return this->cardCount;
    }

    void clearUserCards()
    {
        this->userCards.clear();
    }

    void setWinTexture()
    {
        winTexture = loadTexture("src/image/win.png");
    }

    void printCards()
    {
        sortCard();
        for (int i = 0; i < cardCount; i++)
        {
            userCards[i].setX(i * 80 + 50);

            // selected cards
            if (count(selectedCards.begin(), selectedCards.end(), i))
            {
                userCards[i].setY(525);
            }
            else
            {
                userCards[i].setY(560);
            }

            // Render texture to screen
            SDL_RenderCopy(gRenderer, userCards[i].getTexture(), NULL, userCards[i].getDestinationRect());
        }
    }

    void printWinner()
    {
        int len = gameResult.size(); // length of gamerResult

        for (int i = 0; i < len; i++)
        {
            // this opens a font style and sets a size
            TTF_Font *Lazy = TTF_OpenFont("src/fonts/Freedom-nZ4J.otf", 40);
            SDL_Color Red = {255, 0, 0};

            SDL_Rect destinationRect, Message_rect;
            if (gameResult[i] == -1)
            {
                winTexture = loadTexture("src/image/win.png");
                destinationRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 100, 170, 170};
                Message_rect = {SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 220, 100, 100};
            }

            SDL_Surface *surfaceMessage;
            if (i == 0)
            {
                surfaceMessage = TTF_RenderText_Solid(Lazy, "FIRST PLACE", Red);
            }
            else if (i == 1)
            {
                surfaceMessage = TTF_RenderText_Solid(Lazy, "SECOND PLACE", Red);
            }
            else if (i == 2)
            {
                surfaceMessage = TTF_RenderText_Solid(Lazy, "THIRD PLACE", Red);
            }
            else
            {
                surfaceMessage = TTF_RenderText_Solid(Lazy, "FOURTH PLACE", Red);
            }

            SDL_Texture *Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

            SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect);
            SDL_RenderCopy(gRenderer, winTexture, NULL, &destinationRect);
        }
    }

    void changeSelected(int x)
    {

        // check if x in changeSelected of not?
        if (count(selectedCards.begin(), selectedCards.end(), x))
        {
            selectedCards.erase(remove(selectedCards.begin(), selectedCards.end(), x), selectedCards.end());
        }
        else
        {
            selectedCards.push_back(x);
        }
    }

    bool getIsFinish()
    {
        return isFinish;
    }

    void checkWin()
    {
        if (cardCount == 0)
        {
            this->isFinish = true;
            setPlace();
        }
        if (gameResult.size() == 3)
        {
            this->isFinish = true;
            setPlace();
        }
    }

    void setPlace()
    {
        // player has index -1
        gameResult.push_back(-1);
    }

    void hit()
    {
        int selectedCardLen = selectedCards.size();

        // check is valid cards?

        // change turn (if not user's turn)
        isTurn = false;

        // sort selected cards
        sort(selectedCards.begin(), selectedCards.end());

        // append history
        int historyLen = history.size();
        if (historyLen > 2)
        {
            history.clear();
        }

        vector<Card> temp;
        for (int index : selectedCards)
        {
            temp.push_back(userCards[index]);
        }
        history.push_back(temp);

        for (int i = selectedCardLen - 1; i >= 0; i--)
        {
            userCards.erase(userCards.begin() + selectedCards[i]);
            cardCount--;
        }

        checkWin();

        selectedCards.clear();
    }

    void isFirstUser()
    {
        for (Card card : userCards)
        {
            if (card.getSuits() == SPADES && card.getValue() == 3)
            {
                this->isTurn = true;
            }
        }
    }

    bool isUserTurn()
    {
        return isTurn;
    }

    void setUserTurn(bool value)
    {
        isTurn = value;
    }

    void setSkip(bool value)
    {
        isSkip = value;
    }

    bool getSkip()
    {
        return isSkip;
    }

    void printSkipText(int id = 0)
    {
        // this opens a font style and sets a size
        TTF_Font *Lazy = TTF_OpenFont("src/fonts/Freedom-nZ4J.otf", 24);
        SDL_Color Red = {255, 0, 0};

        SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Lazy, "SKIP!", Red);
        SDL_Texture *Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

        // user is default
        SDL_Rect Message_rect = {SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 + 70, 80, 80};
        if (id == 1)
        {
            Message_rect.x = 50;
            Message_rect.y = SCREEN_HEIGHT / 2 - 40;
        }
        else if (id == 2)
        {
            Message_rect.x = SCREEN_WIDTH / 2 - 100;
            Message_rect.y = SCREEN_HEIGHT / 2 - 240;
        }
        else if (id == 3)
        {
            Message_rect.x = SCREEN_WIDTH - 200;
            Message_rect.y = SCREEN_HEIGHT / 2 - 40;
        }

        SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect);
        SDL_RenderPresent(gRenderer);

        SDL_Delay(300);
    }

    int countSuits()
    {
        int countBlack = 0;
        int countRed = 0;
        for (auto &card : this->userCards)
        {
            if (card.getSuits() == 3 || card.getSuits() == 4)
            {
                countRed++;
            }
            else
            {
                countBlack++;
            }
        }
        return countBlack < countRed ? countRed : countBlack;
    }
    map<int, int> getSaveCards() // Create a map to save cards
    {
        map<int, int> saveCards;
        for (auto &card : this->userCards)
        {
            saveCards[card.getValue()]++;
        }
        return saveCards;
    }
    void checkSpecialCards() // Check for a perfect hand
    {
        map<int, int> saveCards = getSaveCards();
        int count = 0; // Count couple
        int countQuads = 0;
        int countJack = 0;
        for (pair<int, int> x : saveCards)
        {
            if (x.second >= 2)
            {
                if (x.second == 4)
                {
                    if (x.first == 2)
                    {
                        countJack = 1;
                        break;
                    }
                    else
                    {
                        countQuads++;
                    }
                }
                count++;
            }
        }
        if (count == 6 || count == 0 || countJack == 1 || countQuads == 3 || countSuits() >= 12)
        // Have 6 couple || 3->A  || Have 4 cards(2) || have 3 Quads || Have 12 black cards or 12 red cards
        {
            this->isSpecial = true;
        }
        else if (count == 5)
        {
            if (countQuads == 1) // Have 4 couple and a quad
            {
                this->isSpecial = true;
            }
            else // Have five consecutive pairs
            {
                int fleg = 1;
                int prevKey = -1;
                for (pair<int, int> x : saveCards)
                {
                    if (x.second >= 2)
                    {
                        if (prevKey != -1 && x.first != prevKey + 1)
                        {
                            fleg = -1;
                            break;
                        }
                        prevKey = x.first;
                    }
                }
                if (fleg == 1)
                {
                    this->isSpecial = true;
                }
            }
        }
        else if (count == 4) // Have 2 couple and 2 quads
        {
            if (countQuads >= 2)
            {
                this->isSpecial = true;
            }
        }
    }
    bool isSpecialCards()
    {
        return this->isSpecial;
    }
};

class Computer : public User
{
private:
    int id;

public:
    Computer(int id, PlayingCards &plCards) : User(plCards)
    {
        this->id = id;
    }

    int getId()
    {
        return this->id;
    }

    void printBackCard()
    {
        if (!getIsFinish())
        {
            SDL_Rect destinationRect;

            int backCardWidth = 80;
            int backCardHeight = 116;

            // init 3 cards

            if (id == 1)
            {

                destinationRect = {50, SCREEN_HEIGHT / 2 - 145, backCardWidth, backCardHeight};
                SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
            }

            if (id == 2)
            {

                destinationRect = {SCREEN_WIDTH / 2 - 100, 5, backCardWidth, backCardHeight};
                SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
            }

            if (id == 3)
            {

                destinationRect = {SCREEN_WIDTH - 200, SCREEN_HEIGHT / 2 - 145, backCardWidth, backCardHeight};
                SDL_RenderCopy(gRenderer, backTexture, NULL, &destinationRect);
            }
            SDL_RenderPresent(gRenderer);
        }
    }

    void animationCard(int id)
    {
        // create a random card for performance
        vector<Card> demoCards = history.back();

        int initX, initY, padding;

        if (id == 1)
        {
            initX = 50;
            initY = SCREEN_HEIGHT / 2 - 145;

            padding = 0;
            for (Card demoCard : demoCards)
            {
                demoCard.setWidth(80);
                demoCard.setHeight(116);
                demoCard.normalColor();
                demoCard.setX(initX + padding);
                demoCard.setY(initY);

                SDL_RenderCopy(gRenderer, demoCard.getTexture(), NULL, demoCard.getDestinationRect());

                padding += 40;
            }
        }
        else if (id == 2)
        {
            initX = SCREEN_WIDTH / 2 - 100;
            initY = 5;

            padding = 0;
            for (Card demoCard : demoCards)
            {
                demoCard.setWidth(80);
                demoCard.setHeight(116);
                demoCard.normalColor();
                demoCard.setX(initX + padding);
                demoCard.setY(initY);

                SDL_RenderCopy(gRenderer, demoCard.getTexture(), NULL, demoCard.getDestinationRect());

                padding += 40;
            }
        }
        else
        {
            initX = SCREEN_WIDTH - 200;
            initY = SCREEN_HEIGHT / 2 - 145;

            padding = 0;
            for (Card demoCard : demoCards)
            {
                demoCard.setWidth(80);
                demoCard.setHeight(116);
                demoCard.normalColor();
                demoCard.setX(initX + padding);
                demoCard.setY(initY);

                SDL_RenderCopy(gRenderer, demoCard.getTexture(), NULL, demoCard.getDestinationRect());

                padding += 40;
            }
        }

        SDL_RenderPresent(gRenderer);
        SDL_Delay(400);
    }

    void setPlace()
    {
        gameResult.push_back(id);
    }

    void setWinTexture()
    {
        if (id == 1)
        {
            winTexture = loadTexture("src/image/win1.png");
        }
        if (id == 2)
        {
            winTexture = loadTexture("src/image/win2.png");
        }
        if (id == 3)
        {
            winTexture = loadTexture("src/image/win3.png");
        }
    }
};
