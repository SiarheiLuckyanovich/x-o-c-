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
    char chouseDificalt;
    char m, e;
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
    g.ppField = new TCell * [g.SIZE];
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
    cout << endl;
    cout << endl;
    cout << "      ---====<< The game TicTacToe is begin! >>===---";
    cout << endl;
    cout << "                ============================" ;
    cout << endl;

    cout << "                         " ;
    for (size_t x = 0; x < g.SIZE; x++)
    {
        cout << x+1 << "   ";
    }
    cout << endl;
    for (size_t y = 0; y < g.SIZE; y++)
    {
        cout  << "                    " << " " << y+1 << " |";
        for (size_t x = 0; x < g.SIZE; x++)
        {
            cout << static_cast <char> (g.ppField [y][x]) << " | ";
        }
        cout << endl;
        cout << endl;
    }
    cout << endl << "                       Human: " << static_cast <char> (g.human) ;
    cout << endl << "                       Computer: " << static_cast <char> (g.ai) <<endl <<endl;
}

//=========================================================================
TProgress __fastcall getWon(const TGame & g)
{
    //победв в строках
    for (size_t y=0; y < g.SIZE; y++)
    {
        if ((g.ppField [y][0]== g.ppField [y][1]) && (g.ppField [y][0] == g.ppField [y][2]))
        {
            if (g.ppField [y][0] == g.human)
                return WON_HUMAN;

            if (g.ppField [y][0] == g.ai)
                return WON_AI;
        }
    }
  //победа в столбцах
    for (size_t x=0; x < g.SIZE; x++)
    {
         if ((g.ppField [0][x]== g.ppField [1][x]) && (g.ppField [0][x] == g.ppField [2][x]))
        {
            if (g.ppField [0][x] == g.human)
                return WON_HUMAN;
            if (g.ppField [0][x] == g.ai)
                return WON_AI;
        }
    }

  //победа в диогоналях
    if (g.ppField [0][0] == g.ppField [1][1] && g.ppField [0][0] == g.ppField [2][2])
    {
        if (g.ppField [1][1] == g.human)
            return WON_HUMAN;
        if (g.ppField [1][1] == g.ai)
            return WON_AI;
    }

    if (g.ppField [0][2] == g.ppField [1][1] && g.ppField [1][1] == g.ppField [2][0])
    {
        if (g.ppField [1][1] == g.human)
            return WON_HUMAN;
        if (g.ppField [1][1] == g.ai)
            return WON_AI;
    }


 //ничья
    bool draw {true};
    for (size_t y=0; y < g.SIZE; y++)
    {
        for (size_t x=0; x < g.SIZE; x++)
        {
            if (g.ppField[y][x] == EMPTY)
            {
                draw = false;
                break;
            }
            if (!draw)
            break;
        }
        if (draw)
        return DRAW;
    }
    return IN_PROGRESS;
}
//=========================================================================
void __fastcall congrats(const TGame& g)
{
    if (g.progress == WON_HUMAN)
        cout << " Humsn WON! :)" << endl;
    else if (g.progress == WON_AI)
        cout << " AI WON! :(" << endl;
    else if (g.progress == DRAW)
        cout << " DRAW! :/" << endl;
}
//=========================================================================
TCoord __fastcall getHumanCoord(const TGame & g)
{
    TCoord c;
    do {
        cout << "                       Your turn" << endl;
        cout << "                       Enter column: ";
        cin >> c.x;
        cout << "                       Enter line: ";
        cin >> c.y;

        c.x--;
        c.y--;

    } while (c.x > 2 || c.y > 2 || g.ppField [c.y][c.x]!= EMPTY);

    return c;

}
//=========================================================================
TCoord __fastcall getAICoordH(TGame & g)
{
//предвыйгрышь
 for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField [y][x] == EMPTY)
            {
                g.ppField [y][x] = g.ai;
                if (getWon(g) == WON_AI)
                {
                    g.ppField [y][x] = EMPTY;
                    return { y, x};
                }
                g.ppField [y][x] = EMPTY;
            }
        }
    }
//предпроигрыш
 for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField [y][x] == EMPTY)
            {
                g.ppField [y][x] = g.human;
                if (getWon(g) == WON_HUMAN)
                {
                    g.ppField [y][x] = EMPTY;
                    return { y, x};
                }
                g.ppField [y][x] = EMPTY;
            }
        }
    }
//преоритеты
    if (g.ppField[1][1] == EMPTY)//центр
    {
        return { 1, 1};
        g.ppField [1][1] = EMPTY;
    }


    TCoord buf [4];//углы
    size_t num {0U};
    if (g.ppField[0][0] == EMPTY)
    {
         buf[num] = { 0 , 0};
         num++;
    }
    if (g.ppField[0][2] == EMPTY)
    {
         buf[num] = { 0 , 2};
         num++;
    }
    if (g.ppField[2][0] == EMPTY)
    {
         buf[num] = { 2 , 0};
         num++;
    }
    if (g.ppField[2][2] == EMPTY)
    {
         buf[num] = { 2 , 2};
         num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum( 0, 1000) %num;
        return buf[index];
    }
    //не углы

    if (g.ppField[0][1] == EMPTY)
    {
         buf[num] = { 0 , 1};
         num++;
    }
    if (g.ppField[1][0] == EMPTY)
    {
         buf[num] = { 1 , 0};
         num++;
    }
    if (g.ppField[1][2] == EMPTY)
    {
         buf[num] = { 1 , 2};
         num++;
    }
    if (g.ppField[2][1] == EMPTY)
    {
         buf[num] = { 2 , 1};
         num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum( 0, 1000) %num;
        return buf[index];
    }
 return { 0, 0};
}

TCoord __fastcall getAICoordM(TGame & g)
{
//предпроигрыш
 for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField [y][x] == EMPTY)
            {
                g.ppField [y][x] = g.human;
                if (getWon(g) == WON_HUMAN)
                {
                    g.ppField [y][x] = EMPTY;
                    return { y, x};
                }
                g.ppField [y][x] = EMPTY;
            }
        }
    }
//преоритеты
    if (g.ppField[1][1] == EMPTY)//центр
    {
        return { 1, 1};
        g.ppField [1][1] = EMPTY;
    }


    TCoord buf [4];//углы
    size_t num {0U};
    if (g.ppField[0][0] == EMPTY)
    {
         buf[num] = { 0 , 0};
         num++;
    }
    if (g.ppField[0][2] == EMPTY)
    {
         buf[num] = { 0 , 2};
         num++;
    }
    if (g.ppField[2][0] == EMPTY)
    {
         buf[num] = { 2 , 0};
         num++;
    }
    if (g.ppField[2][2] == EMPTY)
    {
         buf[num] = { 2 , 2};
         num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum( 0, 1000) %num;
        return buf[index];
    }
    //не углы

    if (g.ppField[0][1] == EMPTY)
    {
         buf[num] = { 0 , 1};
         num++;
    }
    if (g.ppField[1][0] == EMPTY)
    {
         buf[num] = { 1 , 0};
         num++;
    }
    if (g.ppField[1][2] == EMPTY)
    {
         buf[num] = { 1 , 2};
         num++;
    }
    if (g.ppField[2][1] == EMPTY)
    {
         buf[num] = { 2 , 1};
         num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum( 0, 1000) %num;
        return buf[index];
    }
 return { 0, 0};
}

TCoord __fastcall getAICoordE(TGame & g)
{
//преоритеты
    if (g.ppField[1][1] == EMPTY)//центр
    {
        return { 1, 1};
        g.ppField [1][1] = EMPTY;
    }


    TCoord buf [4];//углы
    size_t num {0U};
    if (g.ppField[0][0] == EMPTY)
    {
         buf[num] = { 0 , 0};
         num++;
    }
    if (g.ppField[0][2] == EMPTY)
    {
         buf[num] = { 0 , 2};
         num++;
    }
    if (g.ppField[2][0] == EMPTY)
    {
         buf[num] = { 2 , 0};
         num++;
    }
    if (g.ppField[2][2] == EMPTY)
    {
         buf[num] = { 2 , 2};
         num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum( 0, 1000) %num;
        return buf[index];
    }
    //не углы

    if (g.ppField[0][1] == EMPTY)
    {
         buf[num] = { 0 , 1};
         num++;
    }
    if (g.ppField[1][0] == EMPTY)
    {
         buf[num] = { 1 , 0};
         num++;
    }
    if (g.ppField[1][2] == EMPTY)
    {
         buf[num] = { 1 , 2};
         num++;
    }
    if (g.ppField[2][1] == EMPTY)
    {
         buf[num] = { 2 , 1};
         num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum( 0, 1000) %num;
        return buf[index];
    }
 return { 0, 0};
}



//=========================================================================
int main()
{
char answ;
do {
    TGame g;

    initGame(g);
    system("cls");
    printGame(g);


    cout << "             Choose the difficulty (H-hard, M-medium, E - easy): ";
    cin >> g.chouseDificalt;


    do {
        system("cls");
        printGame(g);
        if (g.turn %2 == 0)
        {
        const TCoord c = getHumanCoord(g);
        g.ppField[c.y][c.x] = g.human;
        }
        else
        {
            if (g.chouseDificalt == 'm')
            {
            cout << "           So, you choose m-medium ;). Enjoy!";
            const TCoord c = getAICoordM(g);
            g.ppField[c.y][c.x] = g.ai;
            } else if (g.chouseDificalt == 'e')
            {
            cout << "           So, you choose e-easy ;). Enjoy!";
            const TCoord c = getAICoordE(g);
            g.ppField[c.y][c.x] = g.ai;
            }
            else
            {
            cout << "           So, you choose h-hard ;). Enjoy!";
            const TCoord c = getAICoordH(g);
            g.ppField[c.y][c.x] = g.ai;
            }

        }
        system("cls");
        printGame(g);
        g.turn++;
        g.progress = getWon(g);


        } while (g.progress == IN_PROGRESS);

    deinitGame(g);
    congrats(g);

    cout << "            Do you wanna play again? (say yes): ";
    cin >> answ;
    } while (answ == 'y');

return 0;
}
//=========================================================================
