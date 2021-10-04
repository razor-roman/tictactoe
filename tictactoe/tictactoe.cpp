// tictactoe.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <random> 
#include <ctime>
using namespace std;
//1

enum Cell : char 
{
    CROSS='X',
    ZERO='0',
    EMPTY='_'
};

enum Progress : uint8_t
{
    IN_PROGRESS,
    HUMAN_WIN,
    AI_WIN,
    DRAW
};

struct Coord {
    size_t y{ 0 };
    size_t x{ 0 };
};
#pragma pack(push,1)
struct Game
{
    Cell** ppfield = nullptr;
    const size_t SIZE = 3;
    size_t turn=0;
    Cell human; //?
    Cell ai; //?
    Progress progress =  IN_PROGRESS;
};
#pragma pack(pop)

inline void clearScr()
{
    system("cls");
}

void _fastcall initGame(Game& gm)
{
    gm.ppfield = new Cell * [gm.SIZE];
    for (size_t i = 0; i < gm.SIZE; i++)
    {        
        gm.ppfield[i] = new Cell[gm.SIZE];
    }
    for (size_t x = 0; x < gm.SIZE; x++)
    {
        for (size_t y = 0; y < gm.SIZE; y++)
        {
            gm.ppfield[y][x] = EMPTY;
        }
    }
    std::mt19937 gen(time(0));
    std::uniform_int_distribution<int> uid(0, 100);
    int myMysticNumber = uid(gen), x = -1;
    if (myMysticNumber >= 50)
    {
        gm.human = CROSS;
        gm.ai = ZERO;
    }
    else
    {
        gm.human = ZERO;
        gm.ai = CROSS;
    }    
    gm.turn = 0;
}

void _fastcall deinitGame(Game& gm)
{
    for (size_t i = 0; i < gm.SIZE; i++)
    {       
        delete[]   gm.ppfield[i];       
    }
    delete[] gm.ppfield;
    gm.ppfield = nullptr;
}

void printGame(const Game& gm)
{
    clearScr();
    cout << " y|x";
    for (size_t x = 0; x < gm.SIZE; x++)
        cout << x + 1 << "   ";
    cout << endl;
    for (size_t y = 0; y < gm.SIZE; y++)
    {   
        cout << y+1 << " | ";
        for (size_t x = 0; x < gm.SIZE; x++)
        {
           cout << gm.ppfield[y][x] << " | ";
        }
        cout << endl;
    }
    cout << endl << "Human: " << gm.human <<  " AI: " << gm.ai << endl;
}

void readCoord(Game& gm)
{
    uint16_t x, y;
    do
    {   
        do
        {
            cout << "put y:";
            cout << endl;
            cin >> y;
            cout << "put x:";
            cout << endl;
            cin >> x;
        } while (x > gm.SIZE || x < 0 || y > gm.SIZE || y < 0); //вводимое числа в диапазоне 0-3
    }
    while (gm.ppfield[y - 1][x - 1] != EMPTY); //клетка пустая
    gm.ppfield[y-1][x-1] = gm.human;
}

bool gameEnd(Game& gm)
{    
    //ряды x
    for (size_t x = 0; x < gm.SIZE; x++)
    {
        uint8_t count_ai = 0, count_human = 0;
        for (size_t y = 0; y < gm.SIZE; y++)
        {
            if (gm.ppfield[y][x] == gm.human) count_human++;
            if (gm.ppfield[y][x] == gm.ai) count_ai++;
        }
        if (count_human == gm.SIZE)
        {
            gm.progress = HUMAN_WIN;
            return true;
        }
        if (count_ai == gm.SIZE)
        {
            gm.progress = AI_WIN;
            return true;
        }
    }
    //ряды y
    for (size_t y = 0; y < gm.SIZE; y++)
    {
        uint8_t count_ai=0, count_human=0;
        for (size_t x = 0; x < gm.SIZE; x++)
        {
            if (gm.ppfield[y][x] == gm.human) count_human++;
            if (gm.ppfield[y][x] == gm.ai) count_ai++;
        }                
        if (count_human == gm.SIZE)
        {
            gm.progress = HUMAN_WIN;
            return true;
        }
        if (count_ai == gm.SIZE)
        {
            gm.progress = AI_WIN;
            return true;
        }
    }
    // диагональ ↗
    {
        uint8_t count_ai = 0, count_human = 0;
        size_t x = 0;
        for (size_t y = gm.SIZE-1; y>=0; --y)
        {
            if (gm.ppfield[x][y] == gm.human) count_human++;
            if (gm.ppfield[x][y] == gm.ai) count_ai++;
            if (count_human == gm.SIZE)
            {
                gm.progress = HUMAN_WIN;
                return true;
            }
            if (count_ai == gm.SIZE)
            {
                gm.progress = AI_WIN;
                return true;
            }
            ++x;
            if (x >= gm.SIZE) break;
        }
    }
    // диагональ ↘
    {
        uint8_t count_ai = 0, count_human = 0;
        for (size_t y = 0; y < gm.SIZE; y++)
        {
            for (size_t x = 0; x < gm.SIZE; x++)
            {
                if (x == y && gm.ppfield[y][x] == gm.human) count_human++;
                if (x == y && gm.ppfield[y][x] == gm.ai) count_ai++;
            }
            if (count_human == gm.SIZE)
            {
                gm.progress = HUMAN_WIN;
                return true;
            }
            if (count_ai == gm.SIZE)
            {
                gm.progress = AI_WIN;
                return true;
            }
        }
    }
    // все ходы закончились
    if (gm.turn == gm.SIZE* gm.SIZE)
    {
        gm.progress = DRAW;
        return true;
    }
    return false;
}

void aiMove(Game& gm)
{
    
   //x axis
        for (size_t x = 0; x < gm.SIZE; x++)
        {
            uint8_t counter_human = 0,counter_ai = 0;
            for (size_t y = 0; y < gm.SIZE; y++)
            {
                if (gm.ppfield[x][y] == gm.human) counter_human++;
                if (gm.ppfield[x][y] == gm.ai) counter_ai++;
            }
            if (counter_human == 2 || counter_ai==2)
            {
                for (size_t y = 0; y < gm.SIZE; y++)
                {
                    if (gm.ppfield[x][y] == EMPTY && gm.ppfield[x][y] != gm.human)
                    {
                        gm.ppfield[x][y] = gm.ai;
                        return;
                    }
                }
            }
        }  
    //y axis
        for (size_t y = 0; y < gm.SIZE; y++)
        {
            uint8_t counter_human = 0, counter_ai = 0;
            for (size_t x = 0; x < gm.SIZE; x++)
            {
                if (gm.ppfield[x][y] == gm.human) counter_human++;
                if (gm.ppfield[x][y] == gm.ai) counter_ai++;
            }
            if (counter_human == 2 || counter_ai == 2)
            {
                for (size_t x = 0; x < gm.SIZE; x++)
                {
                    if (gm.ppfield[x][y] == EMPTY && gm.ppfield[x][y] != gm.human)
                    {
                        gm.ppfield[x][y] = gm.ai;
                        return;
                    }
                }
            }
        }
    //priority 1 middle
    if (gm.ppfield[1][1] == EMPTY)
    {
        gm.ppfield[1][1] = gm.ai;
        return;
    }
    
    // диагональ ↗
    {
        uint8_t count_ai = 0, count_human = 0;
        size_t x = 0;
        for (size_t y = gm.SIZE - 1; y >= 0; --y)
        {
            if (count_human == 2 || count_ai == 2)
            {
                if (gm.ppfield[x][y] == EMPTY)
                {
                    gm.ppfield[x][y] = gm.ai;
                    return;
                }
            }
            if (gm.ppfield[x][y] == gm.human) count_human++;
            if (gm.ppfield[x][y] == gm.ai) count_ai++;            
            ++x;
            if (x >= gm.SIZE) break;
        }
    }
    // диагональ ↗ обратно
    {
        uint8_t count_ai = 0, count_human = 0;
        size_t x = gm.SIZE-1;
        for (size_t y = 0; y < gm.SIZE; y++)
        {
            if (count_human == 2 || count_ai == 2)
            {
                if(gm.ppfield[x][y]==EMPTY) 
                {
                gm.ppfield[x][y] =gm.ai;
                return;
                }
                    
            }
            if (x < 0) break;
            if (gm.ppfield[x][y] == gm.human) count_human++;
            if (gm.ppfield[x][y] == gm.ai) count_ai++;
            --x;
            
        }
    }
    // диагональ ↘ (не требуется, исключается логикой поведения AI)
    
    //priority corners
    for (size_t x = 0; x < gm.SIZE; x++)
    {
        for (size_t y = 0; y < gm.SIZE; y++)
        {
            if (x == 1 || y  == 1) continue;
            if (gm.ppfield[x][y] == EMPTY && gm.ppfield[x][y] != gm.ai)
            {
                gm.ppfield[x][y] = gm.ai;
                return;
            }

        }
    }
    //empty search
    for (size_t x = 0; x < gm.SIZE; x++)
    {
        for (size_t y = 0; y < gm.SIZE; y++)
        {
            if (gm.ppfield[x][y] == EMPTY && gm.ppfield[x][y] != gm.ai)
            {
                gm.ppfield[x][y] = gm.ai;
                return;
            }
        }
    }
}

int main()
{
    Game gm;
    initGame(gm);
    printGame(gm);
    do
    {
        if (gm.human == CROSS)
        {
            readCoord(gm);
            printGame(gm);
            gm.turn++;
            if (gameEnd(gm)) break;
            aiMove(gm);
            gm.turn++;
            printGame(gm);
            if (gameEnd(gm)) break;
        }
        else
        {
            aiMove(gm);
            gm.turn++;
            printGame(gm);
            if (gameEnd(gm)) break;
            readCoord(gm);
            printGame(gm);
            gm.turn++;
            if (gameEnd(gm)) break;
            
        }
        
    } 
    while (gm.progress==IN_PROGRESS);
    if (gm.progress == HUMAN_WIN) cout << "Human win!";
    if (gm.progress == AI_WIN) cout << "AI win!";
    if (gm.progress == DRAW) cout << "DRAW!";
    cout << endl;
    system("pause");
    deinitGame(gm);
    return 0;
}
