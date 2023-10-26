#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <conio.h>
#include <string>

using namespace std;

//-------------------------------------------------CLASSES-----------------------------------

class Player
{
public:
    int x;
    int y;
    int spawnx = 0;
    int spawny = 0;
    int lifes = 3;
    int ammo = 15;
    char movingdirection = 'd';
};

class Cannonhorizontal
{
public:
    int x;
    int y;
    char shootingdirection;
    int speed;
};

class Cannonvertical
{
public:
    int x;
    int y;
    char shootingdirection;
    int speed;
};

class Inamic
{
public:
    int x;
    int y;
    int lifes = 2;
    char movingdirection = 'd';
};

class Randomer
{
public:
    int x;
    int y;
    int xmax;
    int xmin;
    int ymax;
    int ymin;
    int lifes = 2;
};

class Chaser
{
public:
    int x;
    int y;
    bool dead = false;
};

class Bullet
{
public:
    int x;
    int y;
    char movingdirection;
    bool exists = false;
};

//-------------------------------------------------VARIABLES-----------------------------------

int daaaa = 0;
Player player;
int length, width;
int framesdelay;
bool chasersplaced = false;
char c;
Chaser chaser[100]; int nrchaser = 0;
Randomer randomer[100]; int nrrandomer = 0; int r; int scadr;
Cannonhorizontal cannonhorizontal[100]; int nrcannonhorizontal = 0;
Cannonvertical cannonvertical[100]; int nrcannonvertical = 0;
Inamic inamic[100]; int nrinamic = 0; int maxpelinie = 0;
Bullet bullet[400];
int framecount = 1;
bool exitt = false;

//-------------------------------------------------SHORTCUTS-----------------------------------

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

char getcursorchar()
{
    char c = '\0';
    CONSOLE_SCREEN_BUFFER_INFO con;
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hcon != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hcon, &con))
    {
        DWORD read = 0;
        if (!ReadConsoleOutputCharacterA(hcon, &c, 1, con.dwCursorPosition, &read) || read != 1)
            c = '\0';
    }
    return c;
}

char readchar(int x, int y)
{
    gotoxy(x, y);
    char ccccc = getcursorchar();
    return ccccc;
}

void loadlevel(string filename)
{
    int n, aux;
    string getcontent;
    ifstream fin(filename.c_str());
    gotoxy(0, 0);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    if (fin.is_open())
    {
        fin >> length >> width >> n;
        for (int i = 1; i <= n; i++)
        {
            fin >> aux;
            randomer[i].xmin = aux;
            fin >> aux;
            randomer[i].ymin = aux;
            fin >> aux;
            randomer[i].xmax = aux;
            fin >> aux;
            randomer[i].ymax = aux;
        }
        fin >> n;
        for (int i = 1; i <= n; i++)
        {
            fin >> aux;
            cannonhorizontal[i].speed = aux;
        }
        fin >> n;
        for (int i = 1; i <= n; i++)
        {
            fin >> aux;
            cannonvertical[i].speed = aux;
        }
        gotoxy(0, 0);
        getline(fin, getcontent);
        while (!fin.eof())
        {
            getline(fin, getcontent);
            cout << getcontent << endl;
        }
    }
    fin.close();
}

void showxy()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    gotoxy(14, 43);
    cout << "XY: ( " << player.x << ", " << player.y << " )";
}

void hp_gloante()
{
    gotoxy(97, 6);
    cout << "           ";
    gotoxy(96, 6);
    cout << "           ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    gotoxy(90, 3);
    cout << "LIFES: " << player.lifes;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    gotoxy(90, 6);
    cout << "AMMO: " << player.ammo;
}

void legenda()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    gotoxy(87, 0);
    cout << "------------------------------------------";
    gotoxy(87, 9);
    cout << "------------------------------------------";
    gotoxy(87, 37);
    cout << "------------------------------------------";
    for (int i = 1; i <= 11; i++)
    {
        gotoxy(0, 37 + i);
        cout << "|";
        gotoxy(40, 37 + i);
        cout << "|";
        gotoxy(86, 37 + i);
        cout << "|";
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    gotoxy(105, 10);
    cout << "LEGEND:";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    gotoxy(90, 13);
    cout << char(24) << "  ->  YOU";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    gotoxy(90, 15);
    cout << "#  ->  Unmovable object";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    gotoxy(90, 17);
    cout << "@  ->  Movable object";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    gotoxy(90, 19);
    cout << "- , |  ->  Cannon (Indestructible)";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    gotoxy(90, 21);
    cout << "S  ->  Vertically moving enemy (2HP)";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    gotoxy(90, 23);
    cout << "%  ->  Randomly moving enemy (2HP)";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
    gotoxy(90, 25);
    cout << "&  ->  Follows you (1HP)";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    gotoxy(90, 27);
    cout << "A  ->  Ammo (+5)";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    gotoxy(90, 29);
    cout << "+  ->  Life (+1)";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    gotoxy(90, 31);
    cout << 'C';
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "  ->  CheckPoint";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 51);
    gotoxy(90, 33);
    cout << "I";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "  ->  Spawn/Despawn chasers";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 153);
    gotoxy(90, 35);
    cout << "F";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "  ->  Finish";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    gotoxy(89, 41);
    cout << "ESC  ->  Quit to menu";
    gotoxy(89, 44);
    cout << "R  ->  Restart from CheckPoint";
    gotoxy(96, 45);
    cout << "(losses 1 life)";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    gotoxy(43, 41);
    cout << "W,A,S,D  ->  Move character";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    gotoxy(43, 45);
    cout << "M  ->  Shoot";
}

void createobjects()
{
    for (int j = 1; j < length; j++)
    {
        for (int i = 1; i < width; i++)
        {
            if (readchar(i, j) != '#' && readchar(i, j) != ' ')
            {
                if (readchar(i, j) == '>')
                {
                    if (player.spawnx == 0 && player.spawny == 0)
                    {
                        player.x = i;
                        player.y = j;
                        player.spawnx = i;
                        player.spawny = j;
                        gotoxy(i, j);
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                        cout << char(26);
                    }
                    else
                    {
                        gotoxy(i, j);
                        cout << ' ';
                    }
                }
                else
                    if (readchar(i, j) == '@')
                    {
                        gotoxy(i, j);
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                        cout << '@';
                    }
                    else
                        if (readchar(i, j) == 'A')
                        {
                            gotoxy(i, j);
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                            cout << 'A';
                        }
                        else
                            if (readchar(i, j) == '+')
                            {
                                gotoxy(i, j);
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                                cout << '+';
                            }
                            else
                                if (readchar(i, j) == 'C')
                                {
                                    gotoxy(i, j);
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                                    cout << 'C';
                                }
                                else
                                    if (readchar(i, j) == '&')
                                    {
                                        nrchaser++;
                                        chaser[nrchaser].x = i;
                                        chaser[nrchaser].y = j;
                                        gotoxy(i, j);
                                        if (chasersplaced == false)
                                        {
                                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                                        }
                                        else
                                        {
                                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                                        }
                                        cout << '&';
                                    }
                                    else
                                        if (readchar(i, j) == 'S')
                                        {
                                            nrinamic++;
                                            inamic[nrinamic].x = i;
                                            inamic[nrinamic].y = j;
                                            gotoxy(i, j);
                                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                                            cout << 'S';
                                        }
                                        else
                                            if (readchar(i, j) == '-')
                                            {
                                                nrcannonhorizontal++;
                                                cannonhorizontal[nrcannonhorizontal].x = i;
                                                cannonhorizontal[nrcannonhorizontal].y = j;
                                                if (readchar(i + 1, j) == '#' || readchar(i + 1, j) == 'X')
                                                {
                                                    cannonhorizontal[nrcannonhorizontal].shootingdirection = 'a';
                                                }
                                                else
                                                    if (readchar(i - 1, j) == '#' || readchar(i - 1, j) == 'X')
                                                    {
                                                        cannonhorizontal[nrcannonhorizontal].shootingdirection = 'd';
                                                    }
                                                gotoxy(i, j);
                                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                                                cout << '-';
                                            }
                                            else
                                                if (readchar(i, j) == '|')
                                                {
                                                    nrcannonvertical++;
                                                    cannonvertical[nrcannonvertical].x = i;
                                                    cannonvertical[nrcannonvertical].y = j;
                                                    if (readchar(i, j + 1) == '#' || readchar(i, j + 1) == 'X')
                                                    {
                                                        cannonvertical[nrcannonvertical].shootingdirection = 'w';
                                                    }
                                                    else
                                                        if (readchar(i, j - 1) == '#' || readchar(i, j - 1) == 'X')
                                                        {
                                                            cannonvertical[nrcannonvertical].shootingdirection = 's';
                                                        }
                                                    gotoxy(i, j);
                                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                                                    cout << '|';
                                                }
                                                else
                                                    if (readchar(i, j) == '%')
                                                    {
                                                        nrrandomer++;
                                                        randomer[nrrandomer].x = i;
                                                        randomer[nrrandomer].y = j;
                                                        gotoxy(i, j);
                                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                                                        cout << '%';
                                                    }
                                                    else
                                                        if (readchar(i, j) == 'F')
                                                        {
                                                            gotoxy(i, j);
                                                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 153);
                                                            cout << 'F';
                                                        }
                                                        else
                                                            if (readchar(i, j) == 'I')
                                                            {
                                                                gotoxy(i, j);
                                                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 51);
                                                                cout << 'I';
                                                            }
            }
        }
    }
    legenda();
    daaaa = nrinamic;
}

void death()
{
    player.lifes--;
    for (int i = 1; i <= 2; i++)
    {
        gotoxy(player.x, player.y);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
        if (player.movingdirection == 'w')
        {
            cout << char(24);
        }
        else
            if (player.movingdirection == 'a')
            {
                cout << char(27);
            }
            else
                if (player.movingdirection == 's')
                {
                    cout << char(25);
                }
                else
                    if (player.movingdirection == 'd')
                    {
                        cout << char(26);
                    }
        Sleep(700);
        gotoxy(player.x, player.y);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        if (player.movingdirection == 'w')
        {
            cout << char(24);
        }
        else
            if (player.movingdirection == 'a')
            {
                cout << char(27);
            }
            else
                if (player.movingdirection == 's')
                {
                    cout << char(25);
                }
                else
                    if (player.movingdirection == 'd')
                    {
                        cout << char(26);
                    }
        Sleep(700);
    }
    maxpelinie = 0;
    for (int i = 1; i <= 400; i++)
    {
        bullet[i].exists = false;
    }
    nrinamic = 0; nrchaser = 0; nrcannonhorizontal = 0; nrcannonvertical = 0; nrrandomer = 0;
    chasersplaced = false;
    loadlevel("map.in");
    createobjects();
    player.x = player.spawnx;
    player.y = player.spawny;
    if (player.lifes < 1)
    {
        c = char(27);
    }
}

void update()
{
    c = '0';
    if (_kbhit() == 1)
    {
        c = _getch();
    }
    if (c == 'w')
    {
        player.movingdirection = 'w';
        gotoxy(player.x, player.y);
        cout << ' ';
        if (readchar(player.x, player.y - 1) != '#' && readchar(player.x, player.y - 1) != '-' && readchar(player.x, player.y - 1) != '|' && readchar(player.x, player.y - 1) != 'X')
        {
            if (readchar(player.x, player.y - 1) == '@')
            {
                if (readchar(player.x, player.y - 2) == ' ' || readchar(player.x, player.y - 2) == 'o')
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                    gotoxy(player.x, player.y - 2);
                    cout << '@';
                    player.y--;
                }
            }
            else
            {
                player.y--;
            }
        }
    }
    else
        if (c == 'a')
        {
            player.movingdirection = 'a';
            gotoxy(player.x, player.y);
            cout << ' ';
            if (readchar(player.x - 1, player.y) != '#' && readchar(player.x - 1, player.y) != '-' && readchar(player.x - 1, player.y) != '|')
            {
                if (readchar(player.x - 1, player.y) == '@')
                {
                    if (readchar(player.x - 2, player.y) == ' ' || readchar(player.x - 2, player.y) == 'o')
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                        gotoxy(player.x - 2, player.y);
                        cout << '@';
                        player.x--;
                    }
                }
                else
                    player.x--;
            }
        }
        else
            if (c == 's')
            {
                player.movingdirection = 's';
                gotoxy(player.x, player.y);
                cout << ' ';
                if (readchar(player.x, player.y + 1) != '#' && readchar(player.x, player.y + 1) != '-' && readchar(player.x, player.y + 1) != '|' && readchar(player.x, player.y + 1) != 'X')
                {
                    if (readchar(player.x, player.y + 1) == '@')
                    {
                        if (readchar(player.x, player.y + 2) == ' ' || readchar(player.x, player.y + 2) == 'o')
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                            gotoxy(player.x, player.y + 2);
                            cout << '@';
                            player.y++;
                        }
                    }
                    else
                    {
                        player.y++;
                    }
                }
            }
            else
                if (c == 'd')
                {
                    player.movingdirection = 'd';
                    gotoxy(player.x, player.y);
                    cout << ' ';
                    if (readchar(player.x + 1, player.y) != '#' && readchar(player.x + 1, player.y) != '-' && readchar(player.x + 1, player.y) != '|')
                    {
                        if (readchar(player.x + 1, player.y) == '@')
                        {
                            if (readchar(player.x + 2, player.y) == ' ' || readchar(player.x + 2, player.y) == 'o')
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                                gotoxy(player.x + 2, player.y);
                                cout << '@';
                                player.x++;
                            }
                        }
                        else
                            player.x++;
                    }
                }
                else
                    if (c == 'm')
                    {
                        if (player.ammo > 0)
                        {
                            if (player.movingdirection == 'w')
                            {
                                if (readchar(player.x, player.y - 1) != '#' && readchar(player.x, player.y - 1) != '-' && readchar(player.x, player.y - 1) != '|' && readchar(player.x, player.y - 1) != '@' &&
                                    readchar(player.x, player.y - 1) != 'A' && readchar(player.x, player.y - 1) != '+' && readchar(player.x, player.y - 1) != 'C' && readchar(player.x, player.y - 1) != 'I' && readchar(player.x, player.y - 1) != 'F')
                                {
                                    for (int i = 1; i <= 400; i++)
                                    {
                                        if (bullet[i].exists == false)
                                        {
                                            player.ammo--;
                                            bullet[i].exists = true;
                                            bullet[i].movingdirection = 'w';
                                            bullet[i].x = player.x;
                                            bullet[i].y = player.y;
                                            i = 500;
                                        }
                                    }
                                }
                            }
                            else
                                if (player.movingdirection == 'a')
                                {
                                    if (readchar(player.x - 1, player.y) != '#' && readchar(player.x - 1, player.y) != '-' && readchar(player.x - 1, player.y) != '|' && readchar(player.x - 1, player.y) != '@' &&
                                        readchar(player.x - 1, player.y) != 'A' && readchar(player.x - 1, player.y) != '+' && readchar(player.x - 1, player.y) != 'C' && readchar(player.x - 1, player.y) != 'I' && readchar(player.x - 1, player.y) != 'F')
                                    {
                                        for (int i = 1; i <= 400; i++)
                                        {
                                            if (bullet[i].exists == false)
                                            {
                                                player.ammo--;
                                                bullet[i].exists = true;
                                                bullet[i].movingdirection = 'a';
                                                bullet[i].x = player.x;
                                                bullet[i].y = player.y;
                                                i = 500;
                                            }
                                        }
                                    }
                                }
                                else
                                    if (player.movingdirection == 's')
                                    {
                                        if (readchar(player.x, player.y + 1) != '#' && readchar(player.x, player.y + 1) != '-' && readchar(player.x, player.y + 1) != '|' && readchar(player.x, player.y + 1) != '@' &&
                                            readchar(player.x, player.y + 1) != 'A' && readchar(player.x, player.y + 1) != '+' && readchar(player.x, player.y + 1) != 'C' && readchar(player.x, player.y + 1) != 'I' && readchar(player.x, player.y + 1) != 'F')
                                        {
                                            for (int i = 1; i <= 400; i++)
                                            {
                                                if (bullet[i].exists == false)
                                                {
                                                    player.ammo--;
                                                    bullet[i].exists = true;
                                                    bullet[i].movingdirection = 's';
                                                    bullet[i].x = player.x;
                                                    bullet[i].y = player.y;
                                                    i = 500;
                                                }
                                            }
                                        }
                                    }
                                    else
                                        if (player.movingdirection == 'd')
                                        {
                                            if (readchar(player.x + 1, player.y) != '#' && readchar(player.x + 1, player.y) != '-' && readchar(player.x + 1, player.y) != '|' && readchar(player.x + 1, player.y) != '@' &&
                                                readchar(player.x + 1, player.y) != 'A' && readchar(player.x + 1, player.y) != '+' && readchar(player.x + 1, player.y) != 'C' && readchar(player.x + 1, player.y) != 'I' && readchar(player.x + 1, player.y) != 'F')
                                            {
                                                for (int i = 1; i <= 400; i++)
                                                {
                                                    if (bullet[i].exists == false)
                                                    {
                                                        player.ammo--;
                                                        bullet[i].exists = true;
                                                        bullet[i].movingdirection = 'd';
                                                        bullet[i].x = player.x;
                                                        bullet[i].y = player.y;
                                                        i = 500;
                                                    }
                                                }
                                            }
                                        }
                        }
                    }
                    else
                        if (c == 'r')
                        {
                            death();
                        }

    if (readchar(player.x, player.y) == '+')
    {
        player.lifes++;
    }
    else
        if (readchar(player.x, player.y) == 'A')
        {
            player.ammo += 5;
        }
        else
            if (readchar(player.x, player.y) == 'C')
            {
                player.spawnx = player.x;
                player.spawny = player.y;
            }
            else
                if (readchar(player.x, player.y) == 'F')
                {
                    //FINAL
                }
                else
                    if (readchar(player.x, player.y) == 'I')
                    {
                        if (chasersplaced == false)
                        {
                            chasersplaced = true;
                            for (int i = 1; i <= nrchaser; i++)
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                                gotoxy(chaser[i].x, chaser[i].y);
                                cout << '&';
                            }
                        }
                        else
                        {
                            chasersplaced = false;
                            for (int i = 1; i <= nrchaser; i++)
                            {
                                gotoxy(chaser[i].x, chaser[i].y);
                                cout << ' ';
                            }
                        }
                    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    gotoxy(player.x, player.y);
    if (player.movingdirection == 'w')
    {
        cout << char(24);
    }
    else
        if (player.movingdirection == 'a')
        {
            cout << char(27);
        }
        else
            if (player.movingdirection == 's')
            {
                cout << char(25);
            }
            else
                if (player.movingdirection == 'd')
                {
                    cout << char(26);
                }

    for (int i = 1; i <= 400; i++)
    {
        if (bullet[i].exists == true)
        {
            if (bullet[i].movingdirection == 'w')
            {
                if (readchar(bullet[i].x, bullet[i].y - 1) != '#' && readchar(bullet[i].x, bullet[i].y - 1) != '@' && readchar(bullet[i].x, bullet[i].y - 1) != 'X' && readchar(bullet[i].x, bullet[i].y - 1) != '-' && readchar(bullet[i].x, bullet[i].y - 1) != '|')
                {
                    if (readchar(bullet[i].x, bullet[i].y - 1) == 'o')
                    {
                        gotoxy(bullet[i].x, bullet[i].y);
                        cout << ' ';
                        gotoxy(bullet[i].x, bullet[i].y - 1);
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        cout << 'o';
                        bullet[i].y--;
                    }
                    else
                        if (readchar(bullet[i].x, bullet[i].y - 1) == ' ')
                        {
                            gotoxy(bullet[i].x, bullet[i].y);
                            cout << ' ';
                            gotoxy(bullet[i].x, bullet[i].y - 1);
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                            cout << 'o';
                            bullet[i].y--;
                        }
                        else
                            if (readchar(bullet[i].x, bullet[i].y - 1) == 'S')
                            {
                                for (int j = 1; j <= nrinamic; j++)
                                {
                                    if (inamic[j].x == bullet[i].x && inamic[j].y == bullet[i].y - 1)
                                    {
                                        inamic[j].lifes--;
                                        gotoxy(bullet[i].x, bullet[i].y);
                                        cout << ' ';
                                        bullet[i].exists = false;
                                    }
                                }
                            }
                            else
                                if (readchar(bullet[i].x, bullet[i].y - 1) == '%')
                                {
                                    for (int j = 1; j <= nrrandomer; j++)
                                    {
                                        if (randomer[j].x == bullet[i].x && randomer[j].y == bullet[i].y - 1)
                                        {
                                            randomer[j].lifes--;
                                            gotoxy(bullet[i].x, bullet[i].y);
                                            cout << ' ';
                                            bullet[i].exists = false;
                                        }
                                    }
                                }
                                else
                                    if (readchar(bullet[i].x, bullet[i].y - 1) == '&')
                                    {
                                        for (int j = 1; j <= nrchaser; j++)
                                        {
                                            if (chaser[j].x == bullet[i].x && chaser[j].y == bullet[i].y - 1)
                                            {
                                                chaser[j].dead = true;
                                                gotoxy(bullet[i].x, bullet[i].y);
                                                cout << ' ';
                                                bullet[i].exists = false;
                                            }
                                        }
                                    }
                                    else
                                        if (bullet[i].x == player.x && bullet[i].y - 1 == player.y)
                                        {
                                            death();
                                        }
                                        else
                                        {
                                            gotoxy(bullet[i].x, bullet[i].y);
                                            cout << ' ';
                                            bullet[i].exists = false;
                                        }
                }
                else
                {
                    gotoxy(bullet[i].x, bullet[i].y);
                    cout << ' ';
                    bullet[i].exists = false;
                }
            }
            else
                if (bullet[i].movingdirection == 'a')
                {
                    if (readchar(bullet[i].x - 1, bullet[i].y) != '#' && readchar(bullet[i].x - 1, bullet[i].y) != '@' && readchar(bullet[i].x - 1, bullet[i].y) != 'X' && readchar(bullet[i].x - 1, bullet[i].y) != '-' && readchar(bullet[i].x - 1, bullet[i].y) != '|')
                    {
                        if (readchar(bullet[i].x - 1, bullet[i].y) == 'o')
                        {
                            gotoxy(bullet[i].x, bullet[i].y);
                            cout << ' ';
                            gotoxy(bullet[i].x - 1, bullet[i].y);
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                            cout << 'o';
                            bullet[i].x--;
                        }
                        else
                            if (readchar(bullet[i].x - 1, bullet[i].y) == ' ')
                            {
                                gotoxy(bullet[i].x, bullet[i].y);
                                cout << ' ';
                                gotoxy(bullet[i].x - 1, bullet[i].y);
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                                cout << 'o';
                                bullet[i].x--;
                            }
                            else
                                if (readchar(bullet[i].x - 1, bullet[i].y) == 'S')
                                {
                                    for (int j = 1; j <= nrinamic; j++)
                                    {
                                        if (inamic[j].x == bullet[i].x - 1 && inamic[j].y == bullet[i].y)
                                        {
                                            inamic[j].lifes--;
                                            gotoxy(bullet[i].x, bullet[i].y);
                                            cout << ' ';
                                            bullet[i].exists = false;
                                        }
                                    }
                                }
                                else
                                    if (readchar(bullet[i].x - 1, bullet[i].y) == '%')
                                    {
                                        for (int j = 1; j <= nrrandomer; j++)
                                        {
                                            if (randomer[j].x == bullet[i].x - 1 && randomer[j].y == bullet[i].y)
                                            {
                                                randomer[j].lifes--;
                                                gotoxy(bullet[i].x, bullet[i].y);
                                                cout << ' ';
                                                bullet[i].exists = false;
                                            }
                                        }
                                    }
                                    else
                                        if (readchar(bullet[i].x - 1, bullet[i].y) == '&')
                                        {
                                            for (int j = 1; j <= nrchaser; j++)
                                            {
                                                if (chaser[j].x == bullet[i].x - 1 && chaser[j].y == bullet[i].y)
                                                {
                                                    chaser[j].dead = true;
                                                    gotoxy(bullet[i].x, bullet[i].y);
                                                    cout << ' ';
                                                    bullet[i].exists = false;
                                                }
                                            }
                                        }
                                        else
                                            if (bullet[i].x - 1 == player.x && bullet[i].y == player.y)
                                            {
                                                death();
                                            }
                                            else
                                            {
                                                gotoxy(bullet[i].x, bullet[i].y);
                                                cout << ' ';
                                                bullet[i].exists = false;
                                            }
                    }
                    else
                    {
                        gotoxy(bullet[i].x, bullet[i].y);
                        cout << ' ';
                        bullet[i].exists = false;
                    }
                }
                else
                    if (bullet[i].movingdirection == 's')
                    {
                        if (readchar(bullet[i].x, bullet[i].y + 1) != '#' && readchar(bullet[i].x, bullet[i].y + 1) != '@' && readchar(bullet[i].x, bullet[i].y + 1) != 'X' && readchar(bullet[i].x, bullet[i].y + 1) != '-' && readchar(bullet[i].x, bullet[i].y + 1) != '|')
                        {
                            if (readchar(bullet[i].x, bullet[i].y + 1) == 'o')
                            {
                                gotoxy(bullet[i].x, bullet[i].y);
                                cout << ' ';
                                gotoxy(bullet[i].x, bullet[i].y + 1);
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                                cout << 'o';
                                bullet[i].y++;
                            }
                            else
                                if (readchar(bullet[i].x, bullet[i].y + 1) == ' ')
                                {
                                    gotoxy(bullet[i].x, bullet[i].y);
                                    cout << ' ';
                                    gotoxy(bullet[i].x, bullet[i].y + 1);
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                                    cout << 'o';
                                    bullet[i].y++;
                                }
                                else
                                    if (readchar(bullet[i].x, bullet[i].y + 1) == 'S')
                                    {
                                        for (int j = 1; j <= nrinamic; j++)
                                        {
                                            if (inamic[j].x == bullet[i].x && inamic[j].y == bullet[i].y + 1)
                                            {
                                                inamic[j].lifes--;
                                                gotoxy(bullet[i].x, bullet[i].y);
                                                cout << ' ';
                                                bullet[i].exists = false;
                                            }
                                        }
                                    }
                                    else
                                        if (readchar(bullet[i].x, bullet[i].y + 1) == '%')
                                        {
                                            for (int j = 1; j <= nrrandomer; j++)
                                            {
                                                if (randomer[j].x == bullet[i].x && randomer[j].y == bullet[i].y + 1)
                                                {
                                                    randomer[j].lifes--;
                                                    gotoxy(bullet[i].x, bullet[i].y);
                                                    cout << ' ';
                                                    bullet[i].exists = false;
                                                }
                                            }
                                        }
                                        else
                                            if (readchar(bullet[i].x, bullet[i].y + 1) == '&')
                                            {
                                                for (int j = 1; j <= nrchaser; j++)
                                                {
                                                    if (chaser[j].x == bullet[i].x && chaser[j].y == bullet[i].y + 1)
                                                    {
                                                        chaser[j].dead = true;
                                                        gotoxy(bullet[i].x, bullet[i].y);
                                                        cout << ' ';
                                                        bullet[i].exists = false;
                                                    }
                                                }
                                            }
                                            else
                                                if (bullet[i].x == player.x && bullet[i].y + 1 == player.y)
                                                {
                                                    death();
                                                }
                                                else
                                                {
                                                    gotoxy(bullet[i].x, bullet[i].y);
                                                    cout << ' ';
                                                    bullet[i].exists = false;
                                                }
                        }
                        else
                        {
                            gotoxy(bullet[i].x, bullet[i].y);
                            cout << ' ';
                            bullet[i].exists = false;
                        }
                    }
                    else
                        if (bullet[i].movingdirection == 'd')
                        {
                            if (readchar(bullet[i].x + 1, bullet[i].y) != '#' && readchar(bullet[i].x + 1, bullet[i].y) != '@' && readchar(bullet[i].x + 1, bullet[i].y) != 'X' && readchar(bullet[i].x + 1, bullet[i].y) != '-' && readchar(bullet[i].x + 1, bullet[i].y) != '|')
                            {
                                if (readchar(bullet[i].x + 1, bullet[i].y) == 'o')
                                {
                                    gotoxy(bullet[i].x, bullet[i].y);
                                    cout << ' ';
                                    gotoxy(bullet[i].x + 1, bullet[i].y);
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                                    cout << 'o';
                                    bullet[i].x++;
                                }
                                else
                                    if (readchar(bullet[i].x + 1, bullet[i].y) == ' ')
                                    {
                                        gotoxy(bullet[i].x, bullet[i].y);
                                        cout << ' ';
                                        gotoxy(bullet[i].x + 1, bullet[i].y);
                                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                                        cout << 'o';
                                        bullet[i].x++;
                                    }
                                    else
                                        if (readchar(bullet[i].x + 1, bullet[i].y) == 'S')
                                        {
                                            for (int j = 1; j <= nrinamic; j++)
                                            {
                                                if (inamic[j].x == bullet[i].x + 1 && inamic[j].y == bullet[i].y)
                                                {
                                                    inamic[j].lifes--;
                                                    gotoxy(bullet[i].x, bullet[i].y);
                                                    cout << ' ';
                                                    bullet[i].exists = false;
                                                }
                                            }
                                        }
                                        else
                                            if (readchar(bullet[i].x + 1, bullet[i].y) == '%')
                                            {
                                                for (int j = 1; j <= nrrandomer; j++)
                                                {
                                                    if (randomer[j].x == bullet[i].x + 1 && randomer[j].y == bullet[i].y)
                                                    {
                                                        randomer[j].lifes--;
                                                        gotoxy(bullet[i].x, bullet[i].y);
                                                        cout << ' ';
                                                        bullet[i].exists = false;
                                                    }
                                                }
                                            }
                                            else
                                                if (readchar(bullet[i].x + 1, bullet[i].y) == '&')
                                                {
                                                    for (int j = 1; j <= nrchaser; j++)
                                                    {
                                                        if (chaser[j].x == bullet[i].x + 1 && chaser[j].y == bullet[i].y)
                                                        {
                                                            chaser[j].dead = true;
                                                            gotoxy(bullet[i].x, bullet[i].y);
                                                            cout << ' ';
                                                            bullet[i].exists = false;
                                                        }
                                                    }
                                                }
                                                else
                                                    if (bullet[i].x + 1 == player.x && bullet[i].y == player.y)
                                                    {
                                                        death();
                                                    }
                                                    else
                                                    {
                                                        gotoxy(bullet[i].x, bullet[i].y);
                                                        cout << ' ';
                                                        bullet[i].exists = false;
                                                    }
                            }
                            else
                            {
                                gotoxy(bullet[i].x, bullet[i].y);
                                cout << ' ';
                                bullet[i].exists = false;
                            }
                        }
        }
    }

    for (int i = 1; i <= nrcannonhorizontal; i++)
    {
        if ((framecount % cannonhorizontal[i].speed) == 0)
        {
            //gotoxy(cannonhorizontal[i].x,cannonhorizontal[i].y);
          //  Sleep(3000);
            if (cannonhorizontal[i].shootingdirection == 'a')
            {
                if (readchar(cannonhorizontal[i].x - 1, cannonhorizontal[i].y) != '@')
                {
                    if (cannonhorizontal[i].x - 1 == player.x && cannonhorizontal[i].y == player.y)
                    {
                        death();
                    }
                    for (int j = 1; j <= 400; j++)
                    {
                        if (bullet[j].exists == false)
                        {
                            bullet[j].exists = true;
                            bullet[j].movingdirection = 'a';
                            bullet[j].x = cannonhorizontal[i].x - 1;
                            bullet[j].y = cannonhorizontal[i].y;
                            gotoxy(bullet[j].x, bullet[j].y);
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                            cout << 'o';
                            j = 500;
                        }
                    }
                }
            }
            else
                if (cannonhorizontal[i].shootingdirection == 'd')
                {
                    if (readchar(cannonhorizontal[i].x + 1, cannonhorizontal[i].y) != '@')
                    {
                        if (cannonhorizontal[i].x + 1 == player.x && cannonhorizontal[i].y == player.y)
                        {
                            death();
                        }
                        for (int j = 1; j <= 400; j++)
                        {
                            if (bullet[j].exists == false)
                            {
                                bullet[j].exists = true;
                                bullet[j].movingdirection = 'd';
                                bullet[j].x = cannonhorizontal[i].x + 1;
                                bullet[j].y = cannonhorizontal[i].y;
                                gotoxy(bullet[j].x, bullet[j].y);
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                                cout << 'o';
                                j = 500;
                            }
                        }
                    }
                }
        }
    }

    for (int i = 1; i <= nrcannonvertical; i++)
    {
        if ((framecount % cannonvertical[i].speed) == 0)
        {
            if (cannonvertical[i].shootingdirection == 'w')
            {
                if (readchar(cannonvertical[i].x, cannonvertical[i].y - 1) != '@')
                {
                    if (cannonvertical[i].x == player.x && cannonvertical[i].y - 1 == player.y)
                    {
                        death();
                    }
                    for (int j = 1; j <= 400; j++)
                    {
                        if (bullet[j].exists == false)
                        {
                            bullet[j].exists = true;
                            bullet[j].movingdirection = 'w';
                            bullet[j].x = cannonvertical[i].x;
                            bullet[j].y = cannonvertical[i].y - 1;
                            gotoxy(bullet[j].x, bullet[j].y);
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                            cout << 'o';
                            j = 500;
                        }
                    }
                }
            }
            else
                if (cannonvertical[i].shootingdirection == 's')
                {
                    if (readchar(cannonvertical[i].x, cannonvertical[i].y + 1) != '@')
                    {
                        if (cannonvertical[i].x == player.x && cannonvertical[i].y + 1 == player.y)
                        {
                            death();
                        }
                        for (int j = 1; j <= 400; j++)
                        {
                            if (bullet[j].exists == false)
                            {
                                bullet[j].exists = true;
                                bullet[j].movingdirection = 's';
                                bullet[j].x = cannonvertical[i].x;
                                bullet[j].y = cannonvertical[i].y + 1;
                                gotoxy(bullet[j].x, bullet[j].y);
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                                cout << 'o';
                                j = 500;
                            }
                        }
                    }
                }
        }
    }

    for (int i = 1; i <= nrrandomer; i++)
    {
        r = rand() % 4 + 1;
        if (randomer[i].lifes > 0)
        {
            if (randomer[i].x == player.x && randomer[i].y == player.y)
            {
                death();
            }
            else
                if (r == 1)
                {
                    if (readchar(randomer[i].x, randomer[i].y - 1) != '#' && readchar(randomer[i].x, randomer[i].y - 1) != '@' && readchar(randomer[i].x, randomer[i].y - 1) != 'X' && readchar(randomer[i].x, randomer[i].y - 1) != '-' && readchar(randomer[i].x, randomer[i].y - 1) != '|')
                    {
                        if (randomer[i].y - 1 >= randomer[i].ymin && randomer[i].ymax >= randomer[i].y - 1)
                        {
                            gotoxy(randomer[i].x, randomer[i].y);
                            cout << ' ';
                            randomer[i].y--;
                        }
                    }
                }
                else
                    if (r == 2)
                    {
                        if (readchar(randomer[i].x + 1, randomer[i].y) != '#' && readchar(randomer[i].x + 1, randomer[i].y) != '@' && readchar(randomer[i].x + 1, randomer[i].y) != 'X' && readchar(randomer[i].x + 1, randomer[i].y) != '-' && readchar(randomer[i].x + 1, randomer[i].y) != '|')
                        {
                            if (randomer[i].x + 1 >= randomer[i].xmin && randomer[i].xmax >= randomer[i].x + 1)
                            {
                                gotoxy(randomer[i].x, randomer[i].y);
                                cout << ' ';
                                randomer[i].x++;
                            }
                        }
                    }
                    else
                        if (r == 3)
                        {
                            if (readchar(randomer[i].x, randomer[i].y + 1) != '#' && readchar(randomer[i].x, randomer[i].y + 1) != '@' && readchar(randomer[i].x, randomer[i].y + 1) != 'X' && readchar(randomer[i].x, randomer[i].y + 1) != '-' && readchar(randomer[i].x, randomer[i].y + 1) != '|')
                            {
                                if (randomer[i].y + 1 >= randomer[i].ymin && randomer[i].ymax >= randomer[i].y + 1)
                                {
                                    gotoxy(randomer[i].x, randomer[i].y);
                                    cout << ' ';
                                    randomer[i].y++;
                                }
                            }
                        }
                        else
                            if (r == 4)
                            {
                                if (readchar(randomer[i].x - 1, randomer[i].y) != '#' && readchar(randomer[i].x - 1, randomer[i].y) != '@' && readchar(randomer[i].x - 1, randomer[i].y) != 'X' && readchar(randomer[i].x - 1, randomer[i].y) != '-' && readchar(randomer[i].x - 1, randomer[i].y) != '|')
                                {
                                    if (randomer[i].x - 1 >= randomer[i].xmin && randomer[i].xmax >= randomer[i].x - 1)
                                    {
                                        gotoxy(randomer[i].x, randomer[i].y);
                                        cout << ' ';
                                        randomer[i].x--;
                                    }
                                }
                            }
            if (randomer[i].lifes == 1)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
            }
            else
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
            gotoxy(randomer[i].x, randomer[i].y);
            cout << '%';
        }
    }

    showxy();
    hp_gloante();

    if (chasersplaced == true)
    {
        for (int i = 1; i <= nrchaser; i++)
        {
            if (chaser[i].dead == false)
            {
                if (c == 'w')
                {
                    if (readchar(chaser[i].x, chaser[i].y - 1) == ' ' || readchar(chaser[i].x, chaser[i].y - 1) == 'o')
                    {
                        gotoxy(chaser[i].x, chaser[i].y);
                        cout << ' ';
                        chaser[i].y--;
                        gotoxy(chaser[i].x, chaser[i].y);
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                        cout << '&';
                    }
                    else
                        if (chaser[i].x == player.x && chaser[i].y - 1 == player.y)
                        {
                            death();
                        }
                }
                else
                    if (c == 'a')
                    {
                        if (readchar(chaser[i].x - 1, chaser[i].y) == ' ' || readchar(chaser[i].x - 1, chaser[i].y) == 'o')
                        {
                            gotoxy(chaser[i].x, chaser[i].y);
                            cout << ' ';
                            chaser[i].x--;
                            gotoxy(chaser[i].x, chaser[i].y);
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                            cout << '&';
                        }
                        else
                            if (chaser[i].x - 1 == player.x && chaser[i].y == player.y)
                            {
                                death();
                            }
                    }
                    else
                        if (c == 's')
                        {
                            if (readchar(chaser[i].x, chaser[i].y + 1) == ' ' || readchar(chaser[i].x, chaser[i].y + 1) == 'o')
                            {
                                gotoxy(chaser[i].x, chaser[i].y);
                                cout << ' ';
                                chaser[i].y++;
                                gotoxy(chaser[i].x, chaser[i].y);
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                                cout << '&';
                            }
                            else
                                if (chaser[i].x == player.x && chaser[i].y + 1 == player.y)
                                {
                                    death();
                                }
                        }
                        else
                            if (c == 'd')
                            {
                                if (readchar(chaser[i].x + 1, chaser[i].y) == ' ' || readchar(chaser[i].x + 1, chaser[i].y) == 'o')
                                {
                                    gotoxy(chaser[i].x, chaser[i].y);
                                    cout << ' ';
                                    chaser[i].x++;
                                    gotoxy(chaser[i].x, chaser[i].y);
                                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                                    cout << '&';
                                }
                                else
                                    if (chaser[i].x + 1 == player.x && chaser[i].y == player.y)
                                    {
                                        death();
                                    }
                            }
            }
        }
    }

    // nrinamic = daaaa;
    for (int i = 1; i <= nrinamic; i++)
    {
        if (inamic[i].lifes > 0)
        {
            if (inamic[i].movingdirection == 'd')
            {
                if (readchar(inamic[i].x + 1, inamic[i].y) == ' ' || readchar(inamic[i].x + 1, inamic[i].y) == 'o' || readchar(inamic[i].x + 1, inamic[i].y) == 'S' ||
                    readchar(inamic[i].x + 1, inamic[i].y) == '&' || readchar(inamic[i].x + 1, inamic[i].y) == '%' || (inamic[i].x + 1 == player.x && inamic[i].y == player.y))
                {
                    gotoxy(inamic[i].x, inamic[i].y);
                    cout << ' ';
                    gotoxy(inamic[i].x + 1, inamic[i].y);
                    if (inamic[i].lifes == 2)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                    }
                    else
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                    cout << 'S';
                    inamic[i].x++;
                }
                else
                {
                    inamic[i].movingdirection = 'a';
                    if (readchar(inamic[i].x - 1, inamic[i].y) == ' ' || readchar(inamic[i].x - 1, inamic[i].y) == 'o' || readchar(inamic[i].x - 1, inamic[i].y) == 'S' ||
                        readchar(inamic[i].x - 1, inamic[i].y) == '&' || readchar(inamic[i].x - 1, inamic[i].y) == '%' || (inamic[i].x - 1 == player.x && inamic[i].y == player.y))
                    {
                        gotoxy(inamic[i].x, inamic[i].y);
                        cout << ' ';
                        gotoxy(inamic[i].x - 1, inamic[i].y);
                        if (inamic[i].lifes == 2)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                        }
                        else
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                        cout << 'S';
                        inamic[i].x--;
                    }
                }
            }
            else
                if (inamic[i].movingdirection == 'a')
                {
                    if (readchar(inamic[i].x - 1, inamic[i].y) == ' ' || readchar(inamic[i].x - 1, inamic[i].y) == 'o' || readchar(inamic[i].x - 1, inamic[i].y) == 'S' ||
                        readchar(inamic[i].x - 1, inamic[i].y) == '&' || readchar(inamic[i].x - 1, inamic[i].y) == '%' || (inamic[i].x - 1 == player.x && inamic[i].y == player.y))
                    {
                        gotoxy(inamic[i].x, inamic[i].y);
                        cout << ' ';
                        gotoxy(inamic[i].x - 1, inamic[i].y);
                        if (inamic[i].lifes == 2)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                        }
                        else
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                        cout << 'S';
                        inamic[i].x--;
                    }
                    else
                    {
                        inamic[i].movingdirection = 'd';
                        if (readchar(inamic[i].x + 1, inamic[i].y) == ' ' || readchar(inamic[i].x + 1, inamic[i].y) == 'o' || readchar(inamic[i].x + 1, inamic[i].y) == 'S' ||
                            readchar(inamic[i].x + 1, inamic[i].y) == '&' || readchar(inamic[i].x + 1, inamic[i].y) == '%' || (inamic[i].x + 1 == player.x && inamic[i].y == player.y))
                        {
                            gotoxy(inamic[i].x, inamic[i].y);
                            cout << ' ';
                            gotoxy(inamic[i].x + 1, inamic[i].y);
                            if (inamic[i].lifes == 2)
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                            }
                            else
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                            cout << 'S';
                            inamic[i].x++;
                        }
                    }
                }
        }
    }


    framecount++;
    Sleep(framesdelay);
}

void alegemapa()
{
    system("CLS");
    c = ' ';
    int ales = 0;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    gotoxy(65, 15);
    cout << char(24);
    gotoxy(65, 16);
    cout << "W";

    gotoxy(65, 31);
    cout << char(25);
    gotoxy(65, 30);
    cout << "S";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    gotoxy(63, 20);
    cout << "lvl 1";
    gotoxy(63, 22);
    cout << "lvl 2";
    gotoxy(63, 24);
    cout << "lvl 3";
    gotoxy(63, 26);
    cout << "BACK";
    while (c != char(13))
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        gotoxy(59, 20 + ales);
        cout << "->";
        gotoxy(70, 20 + ales);
        cout << "<-";
        gotoxy(0, 0);
        c = _getch();
        gotoxy(59, 20 + ales);
        cout << "  ";
        gotoxy(70, 20 + ales);
        cout << "  ";
        if (c == 's')
        {
            if (ales < 6)
            {
                ales += 2;
            }
        }
        if (c == 'w')
        {
            if (ales > 0)
            {
                ales -= 2;
            }
        }
    }
    if (ales == 0)
    {
        loadlevel("map.in");
        createobjects();
        while (c != char(27))
        {
            update();
        }
    }
    else
        if (ales == 2)
        {
            loadlevel("map.in");
            createobjects();
            while (c != char(27))
            {
                update();
            }
        }
        else
            if (ales == 4)
            {
                loadlevel("map.in");
                createobjects();
                while (c != char(27))
                {
                    update();
                }
            }
    if (player.lifes == 0)
    {
        //gameover();
    }
    c = ' ';
    system("CLS");
}

void speed()
{
    system("CLS");
    c = ' ';
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    gotoxy(52, 18);
    cout << "CURRENT SPEED(MILISECONDS):";
    gotoxy(51, 23);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    cout << "<- A(-)";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << " MOVING SPEED ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    cout << "(+)D ->";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    gotoxy(53, 26);
    cout << "(Press ENTER to go back)";
    while (c != char(13))
    {
        gotoxy(64, 20);
        cout << "                    ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        gotoxy(64, 20);
        cout << framesdelay;
        gotoxy(0, 0);
        c = _getch();
        if (c == 'd')
        {
            framesdelay++;
        }
        if (c == 'a')
        {
            if (framesdelay >= 2)
            {
                framesdelay--;
            }
        }
    }
    c = ' ';
}

void meniu()
{
    framesdelay = 100;
    c = '0';
    int ales = 2;
    system("CLS");
    while (exitt == false)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        gotoxy(65, 16);
        cout << char(24);
        gotoxy(65, 17);
        cout << "W";

        gotoxy(65, 30);
        cout << char(25);
        gotoxy(65, 29);
        cout << "S";

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        gotoxy(63, 21);
        cout << "START";
        gotoxy(63, 23);
        cout << "SPEED";
        gotoxy(63, 25);
        cout << "QUIT";
        while (c != char(13))
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            gotoxy(59, 21 + ales);
            cout << "->";
            gotoxy(70, 21 + ales);
            cout << "<-";
            gotoxy(0, 0);
            c = _getch();
            gotoxy(59, 21 + ales);
            cout << "  ";
            gotoxy(70, 21 + ales);
            cout << "  ";
            if (c == 's')
            {
                if (ales < 4)
                {
                    ales += 2;
                }
            }
            if (c == 'w')
            {
                if (ales > 0)
                {
                    ales -= 2;
                }
            }
        }
        if (ales == 0)
        {
            alegemapa();
        }
        if (ales == 2)
        {
            speed();
        }
        if (ales == 4)
        {
            exitt = true;
        }
        system("CLS");
        ales = 2;
        player.ammo = 15;
        player.lifes = 3;
        player.spawnx = 0;
        player.spawny = 0;
    }
}

void tutorial()
{
    //box
    for (int i = 0; i <= 79; i++)
    {
        gotoxy(i, 0);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        if (i % 2 == 0)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << "#";
        gotoxy(i, 24);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        if (i % 2 == 0)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << "#";
    }
    for (int i = 0; i <= 24; i++)
    {
        gotoxy(0, i);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        if (i % 2 == 0)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << "#";
        gotoxy(79, i);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        if (i % 2 == 0)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << "#";
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    gotoxy(33, 3);
    cout << "INSTRUCTIUNI";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    gotoxy(11, 6);
    cout << "Apasa click pe ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "iconita de consola";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << " -> ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "proprietati";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << " -> ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "aspect";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    gotoxy(5, 7);
    cout << "Modifica din";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << " Dimensiune fereastra ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << ": ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "latimea";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << " la ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    cout << "129";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << " si ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "inaltimea";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << " la ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    cout << "49";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    gotoxy(26, 10);
    cout << "Apasa";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << " ENTER ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    cout << "pentru a continua";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    gotoxy(65, 22);
    cout << "Creat de";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << ": ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    cout << "Teglas Dennis - 11A";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
    gotoxy(11, 11);
    system("pause");
}

//-------------------------------------------------MAIN-----------------------------------
int main()
{//129 X 49
    system("mode 129,49");
    //tutorial();
    meniu();
    return 0;
}