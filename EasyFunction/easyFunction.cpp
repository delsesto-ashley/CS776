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
    for(int i = 0; i < 100; i++)
    {
        if(scripted[i] == pj[i])
        {
            outcome++;
        }
    }
    return outcome;
}