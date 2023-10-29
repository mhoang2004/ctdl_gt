#pragma once
#include <bits/stdc++.h>
#include <algorithm>
#include "card.h"
#include "user.h"
#include "algorithms.h"
using namespace std;
// Tra ve gia tri dung la skip, con false la danh
bool check_computer(vector<Card> computer, vector<Card> history)
{
    if ((int)history.size() == 0)
    {
        return false;
    }
    if ((int)computer.size() < (int)history.size())
    {
        return true;
    }
    int temp = computer.size();
    mergeSort(computer, 0, temp - 1);
    // Xu ly coc
    if ((int)history.size() == 1)
    {
        for (int i = 0; i < temp; ++i)
        {
            if (computer[i].getValue() > history[0].getValue())
            {
                return false;
            }
            else if ((computer[i].getValue() == history[0].getValue()) && (computer[i].getSuits() > history[0].getSuits()))
            {
                return false;
            }
        }
        return true;
    }
    // Xu ly doi
    else if ((int)history.size() == 2)
    {
        Card his1, his2;
        his1 = history[0];
        his2 = history[1];
        int suit_his1 = his1.getSuits();
        int suit_his2 = his2.getSuits();
        map<int, int> mp;
        for (auto it : computer)
        {
            mp[it.getValue()]++;
        }
        // Chua xu ly chat heo
        // Xu ly neu co doi bang gia tri
        if (mp[his1.getValue()] == 2)
        {
            if (suit_his1 == 3 || suit_his2 == 3)
            {
                return true;
            }
            return false;
        }
        for (int i = 0; i < (int)computer.size(); i++)
        {
            if (computer[i].getValue() > his1.getValue())
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        // Đánh dấu lá bài của máy với lá bài trên bàn
        map<int, int> mp, mp1;
        for (auto it : computer)
        {
            mp[it.getValue()]++;
        }
        int index = 0;
        for (auto it : history)
        {
            index = it.getValue();
            mp1[it.getValue()]++;
        }
        // Nếu có sáp hoặc tú quý của máy có không
        if (mp1[index] == 3 || mp1[index] == 4)
        {
            for (int i = index + 1; i <= 15; i++)
            {
                // Có thể là dấu lớn nma tao tiếc vụ máy có tứ quý mà đánh sáp(3 lá giống nhau)
                if (mp[i] == mp1[index])
                {
                    return false;
                }
            }
            return true;
        }

        int temp1 = (int)history.size(), temp2 = (int)computer.size();
        // Kiem tra tam cuoi cua la truoc
        Card the_last_history = history[temp1 - 1];
        Card the_first_history = history[0];
        // Nếu lá cuối sảnh là lá át cơ thì không đánh
        if (the_last_history.getValue() == 14 && the_last_history.getSuits() == 3)
        {
            return true;
        }
        else
        {
            // Xu ly sanh
            // 3 4 4 5 6 7
            // Xử lý cặp đôi thành 3 4 5 6 7 4
            for (int i = 1; i < temp2 - 1; i++)
            {
                // Duyệt tới lá già là dừng tránh bị tràn mảng
                if (computer[i].getValue() >= 13)
                {
                    break;
                }
                if (computer[i].getValue() == computer[i - 1].getValue())
                {
                    for (int j = i + 1; j < temp2 - 1; j++)
                    {
                        Card swap_teamp = computer[i];
                        computer[i] = computer[j];
                        computer[j] = swap_teamp;
                    }
                }
            }
            int count = 0, ans = -1, val = -1;
            for (int i = 0; i < computer.size() - 1; i++)
            {
                if (computer[i].getValue() == computer[i + 1].getValue() - 1)
                {
                    val = computer[i].getValue() - count;
                    count++;
                }
                else
                {
                    ans = max(ans, count);
                    count = 0;
                }
            }
            if (ans < temp1)
            {
                return true;
            }
            else if (ans >= temp1 && val > history[0].getValue())
            {
                return false;
            }
            return true;
        }
    }
    // 3 4 4 5
}
// void computer_Hit(vector<Card> computer, vector<Card> history)
// {
//     // Sang lai sanh, cap, la rac
//     mergeSort(computer, 0, computer.size() - 1);
//     vector<Card> lobby, couple, singleCard, special;
//     for (int i = 0; i <= 15; i++)
//     {
//         if (computer)
//         {
//         }
//     }
//     temp = computer;
// }
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

    if (count == 3 - gameResult.size())
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

        // print skip text and back cards
        for (Computer computer : computers)
        {
            if (computer.getIsFinish())
            {
                computer.printWinner();
            }
            computer.printBackCard();

            if (computer.getSkip())
            {
                computer.printSkipText(computer.getId());
            }
        }

        SDL_Delay(500);
        SDL_RenderPresent(gRenderer);
    }
}