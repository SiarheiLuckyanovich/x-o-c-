#include <iostream>
#include <clocale>
#include <math.h>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <string>
#include <windows.h>
#include <semaphore.h>

using namespace std;


enum PLAYER {HUMAN='X', AI='O', EMPTY='_'};
typedef struct
{
    int szY;
    int szX;
    PLAYER** map;
    int towin;
} Field;

#define CHK_DOT(x, sz) ((x) >= 0 && (x) < (sz))
#define POINT_ITEM(a, r, c) (*((*(a + r)) + c))
char getval(PLAYER** array, const int row, const int col) {
return POINT_ITEM(array, row, col);
}
void setval(PLAYER** array, const int row, const int col, PLAYER value) {
POINT_ITEM(array, row, col) = value;
}
int isvalid(Field &field, int x, int y) {
return CHK_DOT(x, field.szX) && CHK_DOT(y, field.szY);
}
int isempty(Field &field, int x, int y) {
return getval(field.map, y, x) == EMPTY;
}

void init(Field &field)
{
    field.towin = 3;
    field.szY = 3;
    field.szX = 3;
    field.map = (PLAYER **) calloc(sizeof(PLAYER *), field.szY);

    for (int y = 0; y < field.szY; ++y)
    {
        *(field.map + y) = (PLAYER *) calloc(sizeof(PLAYER),field.szX);
        for (int x = 0; x < field.szX; ++x)
        {
            setval(field.map, y, x, EMPTY);
        }
    }
}

void print(Field &field)
{ ////system("cls")
    std::system("clear");
    printf("-------\n");
    for (int i = 0; i < field.szY; ++i)
    {
        cout << "|";
        for (int j = 0; j < field.szX; ++j)
        {
            printf("%c|", getval(field.map, i, j));
        }
        cout << endl;
    }
}


void human(Field &field)
{
    int x;
    int y;
    do
        {
        printf("¬ведите координаты хода X и Y (от 1 до %d) через пробел >>",field.szY);
        cin >> x >> y;
// C-style
// scanf("%d %d", &x, &y);
// need to check if numbers are entered
        x--; y--;
        }
        while (!isvalid(field, x, y) || !isempty(field, x, y));
        setval(field.map, y, x, HUMAN);
}


void ai(Field &field)
{
    int x, y;
//C++11 <random>
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(0, 3);
    do
        {
// C-style
// x = rand() % field->szX;
// y = rand() % field->szY;
        x = dist(mt);
        y = dist(mt);
        }
        while (!isempty(field, x, y));
        setval(field.map, y, x, AI);
}

int winchk(Field &field, PLAYER c)
{
    for (int y = 0; y < field.szX; y++)
    {
        for (int x = 0; x < field.szY; x++)
        {
            if (linechk(field, y, x, 1, 0, field.towin, c)) return 1;
            if (linechk(field, y, x, 1, 1, field.towin, c)) return 1;
            if (linechk(field, y, x, 0, 1, field.towin, c)) return 1;
            if (linechk(field, y, x, 1, -1, field.towin, c)) return 1;
        }
    }
return 0;
}

int linechk(Field &field, int y, int x, int vx, int vy, int len, PLAYER c)
{
    const int endx = x + (len - 1) * vx;
    const int endy = y + (len - 1) * vy;
    if (!isvalid(field, endx, endy))
    return 0;
for (int i = 0; i < len; i++)
if (getval(field.map, (y + i * vy), (x + i * vx)) != c)
return 0;
return 1;
}

int isdraw(Field &field)
{
    for (int y = 0; y < field.szY; y++)
        for (int x = 0; x < field.szX; x++)
            if (isempty(field, x, y))
            return 0;
    return 1;
}


int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
