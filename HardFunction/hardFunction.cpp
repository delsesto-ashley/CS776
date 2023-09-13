#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <thread>
#include <algorithm>

using namespace std;

double eval(int *pj)
{
    int scripted[100];
    double outcome = 0;
    int tempPasses[3];
    for(int i = 0; i < 100; i++)
    {
        if(i < 30)
        {
            if(i%4 == 0)
            {
                scripted[i] = 1;
            }
            else
            {
                scripted[i] = 0;
            }
        }
        else if(i < 50)
        {
            scripted[i] = 1;
        }
        else if(i < 75)
        {
            scripted[i] = 0;
        }
        else if(i < 92)
        {
            scripted[i] = 1;
        }
        else if(i < 100)
        {
            scripted[i] = 0;
        }
    }
    for(int i = 0; i < 15; i++)
    {
        if(scripted[i] == pj[i])
        {
            tempPasses[0]++;
        }
    }
    for(int i = 15; i < 80; i++)
    {
        if(scripted[i] == pj[i])
        {
            outcome++;
        }
    }
    for(int i = 80; i < 90; i++)
    {
        if(scripted[i] == pj[i])
        {
            tempPasses[1]++;
        }
    }
    for(int i = 90; i < 100; i++)
    {
        if(scripted[i] == pj[i])
        {
            tempPasses[2]++;
        }
    }
    if(tempPasses[0] == 15)
    {
        outcome += 15;
    }
    if(tempPasses[1] == 10)
    {
        outcome += 10;
    }
    if(tempPasses[2] == 10)
    {
        outcome += 10;
    }
    return outcome;
}