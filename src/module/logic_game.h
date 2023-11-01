#pragma once
#include <bits/stdc++.h>
#include <algorithm>
#include "card.h"
#include "user.h"
#include "algorithms.h"
using namespace std;
vector<int> cards_will_Hit_1(vector<Card> computer, vector<Card> history)
{
    vector<int> temp1;
    int temp = computer.size();
    mergeSort(computer, 0, temp - 1);
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
    if ((int)history.size() == 1)
    {
        for (int i = 0; i < temp; ++i)
        {
            if (computer[i].getValue() > history[0].getValue())
            {
                temp1.push_back(i);
                return temp1;
            }
            else if ((computer[i].getValue() == history[0].getValue()) && (computer[i].getSuits() < history[0].getSuits()))
            {
                temp1.push_back(i);
                return temp1;
            }
        }
    }
    else if ((int)history.size() == 2)
    {
        bool ok = true;
        int suit_his1 = history[0].getSuits();
        int suit_his2 = history[1].getSuits();
        // Chua xu ly chat heo
        // Xu ly neu co doi bang gia tri
        // cout << " 2" << endl;
        // for (int i = 0; i < temp; i++)
        // {
        //     if ((mp[computer[i].getValue()] == 2) && (computer[i].getValue() > history[0].getValue()))
        //     {
        //         temp1.push_back(i);
        //         temp1.push_back(i + 1);
        //         return temp1;
        //     }
        // }
        for (int i = 0; i < temp; i++)
        {
            cout << "Check doi bang nhau nhung khac chat" << endl;
            if ((mp[computer[i].getValue()] == 2) && (computer[i].getValue() == history[0].getValue()))
            {
                if (suit_his1 != 3 && suit_his2 != 3)
                {
                    temp1.push_back(i);
                    temp1.push_back(i + 1);
                    return temp1;
                }
            }
            else if ((mp[computer[i].getValue()] == 2) && (computer[i].getValue() > history[0].getValue()))
            {
                temp1.push_back(i);
                temp1.push_back(i + 1);
                return temp1;
            }
        }
    }
    else
    {
        // Nếu có sáp hoặc tú quý của máy có không
        if (mp1[index] == 3 || mp1[index] == 4)
        {
            for (int i = index + 1; i <= 15; i++)
            {
                // Có thể là dấu lớn nma tao tiếc vụ máy có tứ quý mà đánh sáp(3 lá giống nhau)
                if (mp[i] == mp1[index])
                {
                    for (int i = 0; i < (int)computer.size(); i++)
                    {
                        if (computer[i].getValue() == index)
                        {
                            temp1.push_back(i);
                        }
                    }
                    return temp1;
                }
            }
        }
        vector<pair<int, int>> v;
        // Xu ly sanh
        v.push_back({computer[0].getValue(), 0});
        for (int i = 1; i < (int)computer.size(); i++)
        {
            if (computer[i].getValue() != computer[i - 1].getValue())
            {

                v.push_back({computer[i].getValue(), i});
            }
        }

        cout << "Vector v. first luc danh " << endl;
        for (auto it = v.begin(); it != v.end(); it++)
        {
            cout << (*it).first << ' ';
        }
        cout << endl;
        cout << "Vector v. second luc danh " << endl;
        for (auto it = v.begin(); it != v.end(); it++)
        {
            cout << (*it).second << ' ';
        }
        cout << endl;
        // Dùng sliding window
        int a[16];
        int k = (int)history.size();
        auto it = v.begin();
        for (int i = 0; i < k; i++)
        {

            a[i] = (*it).first;
            temp1.push_back((*it).second);
            it++;
        }
        if (a[0] > history[0].getValue() && a[k - 1] - a[0] + 1 == k)
        {
            return temp1;
        }
        else if (a[0] == history[0].getValue() && computer[(*(it--)).second].getSuits() < history[0].getSuits() && a[k - 1] - a[0] + 1 == k)
        {
            return temp1;
        }
        cout << "Hello khuc test 0" << endl;
        for (int i = k; i < (int)v.size(); i++)
        {
            temp1.erase(temp1.begin());
            a[i] = (*it).first;
            temp1.push_back((*it).second);
            cout << "Vong thu " << i << endl;
            for (auto ao = temp1.begin(); ao != temp1.end(); ao++)
            {
                cout << *ao << ' ';
            }
            if (a[i - k + 1] > history[0].getValue() && a[i] - a[i - k + 1] + 1 == k)
            {

                return temp1;
            }
            else if ((a[i - k + 1] == history[0].getValue()) && (computer[(*it).second].getSuits() < history[0].getSuits()) && (a[i] - a[i - k + 1] + 1 == k))
            {
                return temp1;
            }
            it++;
        }
        return temp1;
    }
}

// Tra ve gia tri dung la skip, con false la danh
bool check_computer(vector<Card> computer, vector<Card> history)
{
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
            else if ((computer[i].getValue() == history[0].getValue()) && (computer[i].getSuits() < history[0].getSuits()))
            {
                return false;
            }
        }
        return true;
    }
    // Xu ly doi
    else if ((int)history.size() == 2)
    {
        int suit_his1 = history[0].getSuits();
        int suit_his2 = history[1].getSuits();
        map<int, int> mp;
        for (auto it : computer)
        {
            mp[it.getValue()]++;
        }
        // Chua xu ly chat heo
        // Xu ly neu co doi bang gia tri
        if (mp[history[0].getValue()] == 2)
        {
            if (suit_his1 != 3 && suit_his2 != 3)
            {
                return false;
            }
        }
        for (int i = 0; i < (int)computer.size(); i++)
        {
            if (mp[computer[i].getValue()] == 2 && computer[i].getValue() > history[0].getValue())
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        cout << "May chua sort" << endl;
        for (int i = 0; i < computer.size(); i++)
        {
            cout << computer[i].getValue() << ' ';
        }
        cout << endl;
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
            vector<pair<int, int>> v;
            // Xu ly sanh
            v.push_back({computer[0].getValue(), 0});
            for (int i = 1; i < temp2; i++)
            {
                if (computer[i].getValue() != computer[i - 1].getValue())
                {
                    v.push_back({computer[i].getValue(), i});
                }
            }

            cout << "Vector v " << endl;
            for (auto it = v.begin(); it != v.end(); it++)
            {
                cout << (*it).first << ' ';
            }
            cout << endl;
            int count = 0;
            // Dùng sliding window
            int a[16];
            int k = (int)history.size();
            auto it = v.begin();
            for (int i = 0; i < k; i++)
            {

                a[i] = (*it).first;
                it++;
            }
            if (a[0] > history[0].getValue() && a[k - 1] - a[0] + 1 == k)
            {
                cout << "Silding window lan dau" << endl;
                for (int o = 0; o <= k - 1; o++)
                {
                    cout << a[o] << ' ';
                }
                cout << endl;
                return false;
            }
            else if (a[0] == history[0].getValue() && computer[(*(it--)).second].getSuits() < history[0].getSuits() && a[k - 1] - a[0] + 1 == k)
            {
                return false;
            }
            for (int i = k; i < (int)v.size(); i++)
            {

                a[i] = (*it).first;
                cout << "Siliding window lan thu " << i << "so dau va so cuoi " << a[i] << ' ' << a[i - k + 1] << endl;
                if (a[i] == 15)
                {
                    return true;
                }
                if (a[i - k + 1] > history[0].getValue() && a[i] - a[i - k + 1] + 1 == k)
                {
                    cout << "Silding window" << endl;
                    for (int o = i - k + 1; o <= i; o++)
                    {
                        cout << a[o] << ' ';
                    }
                    cout << endl;
                    return false;
                }
                else if ((a[i - k + 1] == history[0].getValue()) && (computer[(*it).second].getSuits() < history[0].getSuits()) && (a[i] - a[i - k + 1] + 1 == k))
                {
                    return false;
                }
                it++;
            }
            return true;
        }
    }
}
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