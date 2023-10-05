#include <bits/stdc++.h>

#include "algorithms.h"
using namespace std;

class User
{
private:
    vector<Card> userCards;
    vector<Card> selectedCards;

    // The window renderer
    SDL_Renderer *gRenderer = NULL;

    int money = 0;
    int cardCount = 13;
    bool isFirst = false;
    bool isSpecial = false;

public:
    void setUserCards(PlayingCards &plCards)
    {
        for (int i = 0; i < 13; i++)
        {
            Card lastCard = plCards.get1Card();
            this->userCards.push_back(lastCard);
        }
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

    void setCardCount(int num)
    {
        this->cardCount -= num;
    }

    void clearUserCards()
    {
        userCards.clear();
    }
};