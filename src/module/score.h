#pragma once
#include <fstream>
#include <string>
#include "user.h"
#include "card.h"
//#include "global.h"

void writeData(User player, vector<Computer> computers)
{
    ofstream file("score.txt");
    file.seekp(0);
    if(!file.is_open())
    {
        //cout << "Can't open file " << endl;
    }
    else{
        //write score to file
        //cout << player.getMoney() << endl;
        file << player.getMoney() << endl;
        
        for (Computer computer : computers)
        {
            //cout << computer.getMoney() << endl;
            file << computer.getMoney() << endl;          
        }
        file.close();
    }
}

void calculateEndGameMoney(User &player, vector<Computer> &computers, vector<int> gameResult)
{
    int score = 10;
    for(int i = 0; i < gameResult.size(); i++)
    {
        if(gameResult[i] == -1)
        {
            player.setMoney(score);
        }
        else if(gameResult[i] == 1)
        {
            computers[0].setMoney(score);
        }
        else if(gameResult[i] == 2)
        {
            computers[1].setMoney(score);
        }
        else
        {
            computers[2].setMoney(score);
        }

        score -= 5;
    }
    writeData(player, computers);
}


void calculatePigChoppingMoney(User &Player, vector<Computer> &computers, vector<Card> history)
{
    int plus = 0, minus = 0;
    if(history.size() <= 2)
    {
        for(int i = 0; i < history.size(); i++)
        {
            if(history[i].getSuits() == SPADES || history[i].getSuits() == CLUBS)
            {
                plus += 3; 
                minus -= 3;
            }
            else
            {
                plus += 6;
                minus -= 6;
            }
        }
    }
    switch (justHit)
    {
        case -1: {
            if(!Player.getIsFinish())
            {
                Player.setMoney(minus);
                break;
            }
            else return;
        }
        case 1: {
            if(!computers[0].getIsFinish())
            {
                computers[0].setMoney(minus);
                break;
            }
            else return;
        }
        case 2: {
            if(!computers[1].getIsFinish())
            {
                computers[1].setMoney(minus);
                break;
            }
            else return;
        }
        case 3: {
            if(!computers[2].getIsFinish())
            {
                computers[2].setMoney(minus);
                break;
            }
            else return;
        }
    }
    if(Player.isUserTurn())
    {
        Player.setMoney(plus);
    }
    else
    {
        for(int i = 0; i < computers.size(); i++)
        {
            if(computers[i].isUserTurn())
            {
                computers[i].setMoney(plus);
            }
        }
    }
    
}



void readData(User &player, vector<Computer> &computers){
    fstream file("score.txt");
    
    if(!file.is_open())
    {
        cout << "Can't open file " << endl;
    }
    else{
        //read score from file
        int x;
        if (file >> x) 
        {
        // Assuming player is an instance of the Player class
        player.setMoney(x);
        cout << player.getMoney() << endl;
        } 
        else 
        {
            cerr << "Error reading player's money from file!" << endl;
            return; // Exit with an error code
        }

        // Read and set money for each computer
        for (Computer &computer : computers) 
        {
            if (file >> x)
            {
                computer.setMoney(x);
                //cout << computer.getMoney() << endl;
            } 
            else 
            {
                cerr << "Error reading computer's money from file!" << endl;
                return; // Exit with an error code
            }
        }
        file.close();
    }
}
 
void printResult(User player, vector<Computer> computers)
{
    TTF_Font *font = TTF_OpenFont("src/fonts/Oswald-SemiBold.ttf", 20);
    SDL_Color White = {255, 255, 255};
    //List name of user
    string textUser[4];

    textUser[0] = "YOU             " + to_string(player.getMoney());
    textUser[1] = "PLAYER 1     " + to_string(computers[0].getMoney());
    textUser[2] = "PLAYER 2     " + to_string(computers[1].getMoney());
    textUser[3] = "PLAYER 3     " + to_string(computers[2].getMoney());

    int x = SCREEN_WIDTH - 195, y = SCREEN_HEIGHT/2 - 290;
    int padding = 0;
    SDL_Surface *textSurface = NULL;
    SDL_Texture *textTexture = NULL;
    SDL_Rect renderQuad;

    for(int i = 0; i < 4; i++)
    {
        
        textSurface = TTF_RenderText_Solid(font, textUser[i].c_str(), White);
        textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        
        padding += 20;

        renderQuad = {x, y + padding , 150, 30};
        SDL_RenderCopy(gRenderer, textTexture, NULL, &renderQuad);   

        textUser[i].clear();     
    }

}
