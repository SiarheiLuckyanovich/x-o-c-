#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>
#include <windows.h>

using namespace std;
//=========================================================================
enum TCell : char
{
     CROSS = 'X',
     ZERO = 'O',
     EMPTY = '_'
};

struct TCoord
{
    size_t y { 0U };
    size_t x { 0U };
};
enum TProgress
{
    IN_PROGRESS,
    WON_HUMAN,
    WON_AI,
    DRAW
};

//=========================================================================
#pragma pack(push, 1)
struct TGame
{
    TCell** ppField { nullptr };
    const size_t SIZE { 3U };
    TProgress progress { IN_PROGRESS };
    TCell human, ai;
    size_t turn { 0U };
};
#pragma pack(pop)
//=========================================================================
//inline void clearScr() {...}
//=========================================================================
int32_t __fastcall getRandomNum(int32_t min, int32_t max)
{
    const static auto seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937_64 generator(seed);
    uniform_int_distribution<int32_t> dis(min, max);
    return dis(generator);
}

void __fastcall initGame(TGame & g)
{
    g.ppField = new TCell* [g.SIZE];
    for (size_t i = 0U; i < g.SIZE; i++)
    {
        g.ppField [i]= new TCell [g.SIZE];
    }
    for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            g.ppField [y][x] = EMPTY;
        }
    }
    if (getRandomNum(0, 1000) > 500)
    {
        g.human = CROSS;
        g.ai = ZERO;
        g.turn = 0;
    }
    else
    {
        g.human = ZERO;
        g.ai = CROSS;
        g.turn = 1;
    }

}
void __fastcall deinitGame(TGame & g)
{
    for (size_t i = 0U; i < g.SIZE; i++)
    {
        delete[] g.ppField[i];
    }
    delete[] g.ppField;
    g.ppField = nullptr;
}
void __fastcall printGame(const TGame & g)
{
    cout << "     ";
    for (size_t x = 0; x < g.SIZE; x++)
    {
        cout << x+1 << "   ";
    }
    cout << endl;
    for (size_t y = 0; y < g.SIZE; y++)
    {
        cout << " " << y+1 << " |";
        for (size_t x = 0; x < g.SIZE; x++)
        {
            cout << g.ppField [y][x] << " |";
        }
        cout << endl;
    }
    cout << endl << "Human: " << g.human << endl << "Computer: " << g.ai <<endl;
}
void __fastcall congrats(const TGame& g)
{
    if (g.progress == WON_HUMAN)
        cout << " Humsn WON! :)" << endl;
    else if (g.progress == WON_AI)
        cout << " AI WON! :(" << endl;
    else if (g.progress == DRAW)
        cout << " DROW! :\ " << endl;
}

TProgress __fastcall getWon(const TGame & g)
{


}
TCoord __fastcall getHumanCoord(const TGame & g)
{
    TCoord c;
    do {

        cout << "Enter X: ";
        cin >> c.x;
        cout << "Enter Y: ";
        cin >> c.y;

        c.x--;
        c.y--;

    } while (c.x > 2 || c.y > 2 || g.ppField [c.y][c.x]!= EMPTY);

    return c;

}

TCoord __fastcall getAICoord(TGame & g)
{


}

//=========================================================================
int main()
{
 TGame g;
 initGame(g);
 system("cls");
 printGame(g);

 do {
    if (g.turn %2 == 0)
    {
        const TCoord c = getHumanCoord(g);
        g.ppField[c.y][c.x] = g.human;
    }
    else
    {
        const TCoord c = getAICoord(g);
        g.ppField[c.y][c.x] = g.ai;
    }
    system("cls");
    printGame(g);
    g.turn++;
    g.progress = getWon(g);

 } while (g.progress == IN_PROGRESS);

  deinitGame(g);
  return 0;
}
