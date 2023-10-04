#include <bits/stdc++.h>

#include "algorithms.h"
using namespace std;

class User
{
private:
    vector<Card> userCards;
    vector<Card> selectedCards;
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
    void isFirstUserCard()
    {
        for(const Card &card : this->userCards)
        {
            if(card.getSuits() == 6 && card.getValue() == 3)
            {
                this->isFirst = true;
            }
        }
    }
    int countSuits()
    {
        int countBlack = 0;
        int countRed = 0;
        for(const Card &card : this->userCards)
        {
            if(card.getSuits() == 3 || card.getSuits() == 4)
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
        for(const Card &card : this->userCards)
        {
        saveCards[card.getValue()]++;
        }   
        return saveCards;
    }
    void isSpecialCards() // Check for a perfect hand
    {
        map<int, int> saveCards = userCards.getSaveCards();
        int count = 0; // Count couple
        int countQuads = 0; 
        int countJack = 0;
        for(pair<int, int> x : saveCards)
        {
            if(x.second >= 2)
            {
                if(x.second == 4)
                {
                    if(x.first == 2)
                    {
                        countJack++;
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
        if(count == 6) //Have 6 couple
        {
            this->isSpecial = true;
        }
        else if(count == 5)
        {
            if(countQuads == 1) // Have 4 couple and a quad
            {
                this->isSpecial = true;
            }
            else // Have five consecutive pairs
            {
                int fleg = 1;
                int prevKey = -1;
                for(pair<int, int> x : saveCards)
                {
                    if(x.second >= 2)
                    {
                        if(prevKey != -1 && x.first != prevKey + 1)
                        {
                            fleg = -1;
                            break;
                        }
                        prevKey = x.first;
                    }
                }
                if(fleg == 1)
                {
                    this->isSpecial = true;
                }
            }
        }
        else if(count == 4)     //Have 2 couple and 2 quads
        {
            if(countQuads >= 2)
            {
                this->isSpecial = true;
            }
        }
        else if(countJack == 1)   //Have 4 Jack;
        {
            this->isSpecial = true;
        }
        else if(count <= 1)  //3-A 
        {
            this->isSpecial = true;
        }
        else if(userCards.countSuits() >= 12) //Have 12 black cards or 12 red cards
        {
            this->isSpecial = true;
        }
    }
};
