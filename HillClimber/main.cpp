#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <thread>
#include <algorithm>
#include <mutex>
using namespace std;
mutex mtx;
const int mNT = 10;
const int numDigits = 100;
double eval(int *pj);

int newPosition(mt19937 &rng,vector<int> positions)
{
  uniform_int_distribution<mt19937::result_type> dist(0,int(positions.size())-1);
  return positions[dist(rng)];
}

void rngArr(int* arr, int arrSize, mt19937 &rng)
{
  uniform_int_distribution<mt19937::result_type> dist(0,1);
  for(int i = 0; i < arrSize; i++)
  {
    arr[i] = int(dist(rng));
  }
}

void overwriteSolution(int* solFrom, int* solTo)
{
  for(int i = 0; i < numDigits; i++)
  {
    solTo[i] = solFrom[i];
  }
}

void modify(int* arrBase, int* arrNew, int index)
{
  for(int i = 0; i < numDigits; i++)
  {
    arrNew[i] = index != i? arrBase[i]: int((arrBase[index]+1)%2);
  }
}

int updateThreadDigits(int* solution, int* conNums)
{
  int rValue = 0;
  for(int i = 0; i < numDigits; i++)
  {
    if(solution[i] == conNums[i])
    {
      rValue++;
    }
    else if(conNums[i] < 2)
    {
      solution[i] = conNums[i];
    }
  }
  return rValue;
}

void runThread(int (&correctPositions)[numDigits], vector<int>& positions,int(&bestBase)[mNT][numDigits],double(&bestFit)[mNT], int tIDX)
{
  int nIter = 1000;
  int nCount = 0;
  int testPosition = -1;
  int distancePos = 0;
  random_device dev;
  mt19937 randGen(dev());
  int sBase[numDigits];
  int sNew[numDigits];
  int numNewDigits = 0;
  int hitBest = 0;
  rngArr(sBase,numDigits, randGen);
  double fBase = eval(sBase);
  double fNew = 0;
  overwriteSolution(sBase,sNew);
  while((nCount < nIter && fBase < 100) && hitBest == 0)
  {
    mtx.lock();
    numNewDigits = updateThreadDigits(sBase,correctPositions);
    if(numNewDigits)
    {
      fBase = eval(sBase);
    }
    if(positions.size() && fBase < 100)
    {
      testPosition = newPosition(randGen,positions);
      modify(sBase, sNew, testPosition);
      fNew = eval(sNew);
      if(fNew > fBase)
      {
        distancePos = distance(positions.begin(),find(positions.begin(),positions.end(),testPosition));
        if(distancePos < positions.size())
        {
          correctPositions[testPosition] = sNew[testPosition];
          positions.erase(positions.begin()+distancePos);
        }
        overwriteSolution(sNew,sBase);
        fBase = fNew;
      }
    }
    for(int i = 0; i < mNT; i++)
    {
      hitBest = bestFit[i] == 100? 1: hitBest;
    }
    mtx.unlock();
    nCount++;
  }
  mtx.lock();
  for(int i = 0; i < numDigits; i++)
  {
    bestBase[tIDX][i] = sBase[i];
  }
  bestFit[tIDX] = fBase;
  mtx.unlock();
}

void runFinish(int (&correctPositions)[numDigits], vector<int>& positions)
{
  int numPos = int(positions.size());
  int k = -1;
  int attempt[100];
  int attemptFit = 0;
  for(int i = 0; i < numPos;i++)
  {
    attempt[i] = 0;
  }
  for(int i = positions[positions.size()-1]+1; i < numDigits; i++)
  {
    attempt[i] = correctPositions[i];
  }
  long int iterations = 0;
  while(iterations < pow(2,numPos) && attemptFit < 100)
  {
    for(int i = numPos-1; i >= 0; i--)
    {
      k = iterations >> i;
      if(k & 1)
      {
        attempt[i] = 1;
      }
      else
      {
        attempt[i] = 0;
      }
    }
    if(iterations%100000000 == 0)
    {
      cout << iterations << " complete" << endl;
    }
    attemptFit = eval(attempt);
    iterations++;
  }
  for(int i = 0; i < numDigits; i++)
  {
    correctPositions[i] = attempt[i];
  }

}

int main()
{
    std::vector<int> positions;
    int confirmedPositions[numDigits];
    int bestSolution[mNT][numDigits];
    double bestFitness[mNT];
    int solution[100];
    double fitness = 0;
    int index = 0;
    for(int i = 0; i < numDigits; i++)
    {
      positions.push_back(i);
      confirmedPositions[i] = 2;
    }
    std::thread myThreads[mNT];
    for(int threadIDX = 0; threadIDX < mNT; threadIDX++)
    {
      myThreads[threadIDX] = thread(runThread, ref(confirmedPositions), ref(positions), ref(bestSolution),ref(bestFitness), threadIDX);
    }
    for(int threadIDX = 0; threadIDX < mNT; threadIDX++)
    {
      myThreads[threadIDX].join();
    }
    for(int threadIDX = 0; threadIDX < mNT; threadIDX++)
    {
      if(bestFitness[threadIDX] > fitness)
      {
        index = threadIDX;
        fitness = bestFitness[threadIDX];
      }
    }
    for(int i = 0; i < numDigits; i++)
    {
      if(confirmedPositions[i] < 2)
      {
      solution[i] = confirmedPositions[i];
      }
      else
      {
      solution[i] = bestSolution[index][i];
      }
    }
    fitness = eval(solution);
    ofstream file;
    file.open("hardFunction_results.txt",ios::app);
    file << "fitness = " << fitness << endl;
    file << "positions\t";
    for(int i = 0; i < numDigits; i++)
    {
      file << solution[i] << "\t";
    }
    file << endl;
    file.close();
}