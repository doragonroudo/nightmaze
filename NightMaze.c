#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

//-------- Global Variable ------------
char inputKey;
int Moves;
int Objects = 0;
unsigned int ArrayMap[1000][1000];
//---- declare func ----------
void GenerateMaze(char*, int, int);
void ShowMaze(const char*, int, int);
void DrawMaze(int, int);
void CreateObject(int, int);
void CarveMaze(char*, int, int, int, int);
//-------
void GotoXY(int, int);
int Movement(int, int, int);
//------
void ShowText(int);
void ShowPopup(int);
//--------
void WinSize();
int StartGame();
void delay(unsigned int);
int ShowScore(int);
int HighScore();
int MainMenu();
int Credits();
void GoodBye();
void ShowPassLevel(int);

//-------------- Maze Generate Functions Region --------------
void GenerateMaze(char *maze, int width, int height)
{
    int x, y;
    for(x = 0; x < width * height; x++)
    {
        maze[x] = 1;
    }
    maze[1 * width + 1] = 0;

    srand(time(0));

    for(y = 1; y < height; y += 2)
    {
        for(x = 1; x < width; x += 2)
        {
            CarveMaze(maze, width, height, x, y);
        }
    }

    maze[0 * width + 1] = 0;
    maze[(height - 1) * width + (width - 2)] = 0;
}

void ShowMaze(const char *maze, int width, int height)
{
    int x, y;
    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
            switch(maze[y * width + x])
            {
            case 1:
                ArrayMap[y][x]=1;
                break;
            default:
                ArrayMap[y][x]=0;
                break;
            }
        }
    }
}

void DrawMaze(int width, int height)
{
    int i, j;
    for(i=0; i<height; i++)
    {
        for(j=0; j<width; j++)
        {
            switch(ArrayMap[i][j])
            {
            case 0: //----------------------------- 0 = Way
                printf(" ");
                break;
            case 1: //------------------------------ 1 = Wall
                printf("%c",177);
                break;
            case 2: //------------------------------ 2 = Objects
                printf("%c", 184);
                break;
            case 3: //------------------------------ 3 = Locked Exit
                printf("=");
                break;
            case 4: //------------------------------ 1 = Key
                printf("%c", 244);
                break;
            case 5: //------------------------------ 3 = UnLocked Exit
                printf(" ");
                break;
            }
        }
        printf("\n");
    }
}

void CreateObject(int width, int height)
{
    int i, objX, objY, yes=0, keyX, keyY;
    time_t t;
    srand((unsigned) time(&t));
    for(i=0; i<5; i++)
    {
        do
        {
            yes = 0;
            objX = rand() % (width-2) + 2;
            objY = rand() % (height-2) + 2;
            if(ArrayMap[objY][objX] == 0 ) //--- need condition about final destination - not really
            {
                ArrayMap[objY][objX] = 2;
                yes = 1;
            }
        }
        while(yes == 0);
    }
    //--- end collectable -----
    ArrayMap[height-1][width-2] = 3; //--- final destination
    //--- the key ---
    yes = 0;
    do
    {
        yes = 0;
        objX = rand() % (width-2) + 2;
        objY = rand() % (height-2) + 2;
        if(ArrayMap[objY][objX] == 0 ) //--- need condition about final destination - not really
        {
            ArrayMap[objY][objX] = 4;
            yes = 1;
        }
    }
    while(yes == 0);

}

void CarveMaze(char *maze, int width, int height, int x, int y)
{

    int x1, y1;
    int x2, y2;
    int dx, dy;
    int dir, count;

    dir = rand() % 4;
    count = 0;
    while(count < 4)
    {
        dx = 0;
        dy = 0;
        switch(dir)
        {
        case 0:
            dx = 1;
            break;
        case 1:
            dy = 1;
            break;
        case 2:
            dx = -1;
            break;
        default:
            dy = -1;
            break;
        }
        x1 = x + dx;
        y1 = y + dy;
        x2 = x1 + dx;
        y2 = y1 + dy;
        if(x2>0 && x2<width && y2>0 && y2<height && maze[y1*width+x1] == 1 && maze[y2*width+x2] == 1)
        {
            maze[y1 * width + x1] = 0;
            maze[y2 * width + x2] = 0;
            x = x2;
            y = y2;
            dir = rand() % 4;
            count = 0;
        }
        else
        {
            dir = (dir + 1) % 4;
            count += 1;
        }
    }
}

//----------------------------------------------------------

void WinSize()
{
    SMALL_RECT windowSize = {0, 0, 100, 30};    //change the values
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
}

void hideCursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

//-------------- Movement Stuff in-game --------------------------
void GotoXY(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int Movement(int width, int height, int level)
{
    int x = 1;
    int y = 0;
    int xBefore = 1;
    int yBefore = 0;

    while(1==1)
    {
        GotoXY(xBefore,yBefore);
        printf(" ");

        GotoXY(x,y);
        printf("%c", 229);

        inputKey = _getch();

        xBefore = x;
        yBefore = y;
        switch (inputKey)
        {
        case 'w' :
            if(y!=0) y -= 1;
            Moves--;
            break;
        case 's':
            y += 1;
            Moves--;
            break;
        case 'a':
            x -= 1;
            Moves--;
            break;
        case 'd':
            x += 1;
            Moves--;
            break;
        }

        if(ArrayMap[y][x] == 1) //--- collided
        {
            x = xBefore;
            y = yBefore;
            Moves++;
        }
        else if(ArrayMap[y][x] == 3) //---- locked door
        {
            x = xBefore;
            y = yBefore;
            ShowPopup(6);
            Moves++;
        }
        else //-- movable
        {
            if(ArrayMap[y][x] == 2) //--- objects
            {
                ArrayMap[y][x] = 0; //set to way
                Objects++;
                ShowPopup(2);
                ShowPopup(3);
                GotoXY(0, 0);
                DrawMaze(width, height);
                Moves += 15;
            }
            if(ArrayMap[y][x] == 4) //--- key
            {
                ArrayMap[height-1][width-2] = 5;
                ArrayMap[y][x] = 0;
                ShowPopup(4);
                ShowPopup(3);
                ShowPopup(5);
                GotoXY(0, 0);
                DrawMaze(width, height);
                Moves += 15;
            }
            if(Moves < 1)
            {
                ShowPopup(7);
                return 0;
            }
            if(ArrayMap[y][x] == 5)
            {
                return 1;
            }
            ShowText(level);
        }
    }
}

//----------------- Text and Popup -----------------------------------------

void ShowText(int level)
{
    int i;
    //-- show tips ---
    GotoXY(34, 25);
    printf("Controls : Use W, A, S and D to move your character.");
    GotoXY(34, 26);
    printf("Objective : Get the key(%c) and get out of the maze.", 244);
    GotoXY(34, 27);
    printf("Hint : Collecting coins(%c) to get +15 moves.", 184);
    //--- top border -----
    GotoXY(1, 24);
    printf("%c", 201);
    for (i=0; i<30; i++)
        printf("%c", 205);
    printf("%c", 187);
    //--- bottom border -----
    GotoXY(1, 28);
    printf("%c", 200);
    for (i=0; i<30; i++)
        printf("%c", 205);
    printf("%c", 188);
    //--- left border -----
    GotoXY(1, 25);
    printf("%c", 186);
    GotoXY(1, 26);
    printf("%c", 186);
    GotoXY(1, 27);
    printf("%c", 186);
    //---- right border ----
    GotoXY(32, 25);
    printf("%c", 186);
    GotoXY(32, 26);
    printf("%c", 186);
    GotoXY(32, 27);
    printf("%c", 186);
    //------- status
    GotoXY(3, 25);
    printf("Level : %d", level);
    GotoXY(3, 27);
    printf("Remaining Moves : %d   ", Moves);
    GotoXY(3, 26);
    printf("Collected Coins: %d", Objects);
}

void ShowPopup(int n)
{
    GotoXY(38, 13);
    printf("%c                  ", 201);
    GotoXY(38, 14);
    if (n == 2)
        printf("   Coin Collected   ");
    if (n == 3)
        printf("     +15 Moves      ");
    if (n == 4)
        printf("   Key Collected    ");
    if (n == 5)
        printf("   Door Unlocked    ");
    if (n == 6)
        printf("   Door is Locked   ");
    if (n == 7)
        printf("    Out of Moves    ");
    if (n == 8)
        printf("     +100 Moves     ");
    GotoXY(38, 15);
    printf("                  %c", 188);
    delay(1000);
    GotoXY(38, 13);
    printf("                    ", 201);
    GotoXY(38, 14);
    printf("                    ");
    GotoXY(38, 15);
    printf("                    ", 188);
}

//---------------------------- Pages ------------------------------

int StartGame()
{

    int level = 1;
    int width, height, win;
    char *maze;

    while (1)
    {
        system("cls");
        if (level == 1)
        {
            Objects = 0;
            width = 10 * 2 + 3;
            height = 10 * 2 + 3;
            Moves = 200;
        }
        else if (level == 2)
        {
            width = 20 * 2 + 3;
            height = 10 * 2 + 3;
            Moves +=100;
            ShowPopup(8);
        }
        else if (level == 3)
        {
            width = 30 * 2 + 3;
            height = 10 * 2 + 3;
            Moves +=100;
            ShowPopup(8);
        }
        else if (level == 4)
        {
            width = 35 * 2 + 3;
            height = 10 * 2 + 3;
            Moves +=100;
            ShowPopup(8);
        }
        else if (level == 5)
        {
            width = 45 * 2 + 3;
            height = 10 * 2 + 3;
            Moves +=100;
            ShowPopup(8);
        }
        else if (level > 5)
        {
            width = 45 * 2 + 3;
            height = 10 * 2 + 3;
            Moves +=100;
            ShowPopup(8);
        }
        else
        {
            return 0;
        }
        system("cls");
        maze = (char*)malloc(width * height * sizeof(char));
        GenerateMaze(maze, width, height);
        ShowMaze(maze, width, height);
        CreateObject(width, height);
        DrawMaze(width, height);
        free(maze);
        ShowText(level);
        win = Movement(width, height, level);
        if (win == 1)
        {
            ShowPassLevel(level);
            level++;
        }
        else //------------ LOSE -----------
        {
            ShowScore(level);
            return 0;
        }
    }
}

void ShowPassLevel(int level)
{
    system("cls");
    printf("\n\n\n\t   __     _   _____  __    __    _    \t  _       _____   _    _   _____   _ \n");
    printf("\t  |  \\   | | /  _  \\ \\ \\  / /  _| |_  \t | |     /  _  \\ | |  | | /  _  \\ | | \n");
    printf("\t  |   \\  | | | |_| |  \\ \\/ /  |_   _| \t | |     | |_| | | |  | | | |_| | | | \n");
    printf("\t  | |\\ \\ | | |  ___/   \\  /     | |   \t | |     |  ___/ | |  | | |  ___/ | | \n");
    printf("\t  | | \\ \\| | | |       /  \\     | |   \t | |     | |     \\ \\  / / | |     | | \n");
    printf("\t  | |  \\   | | |___   / /\\ \\    | |   \t | |___  | |___   \\ \\/ /  | |___  | |___ \n");
    printf("\t  |_|   \\__| \\_____| /_/  \\_\\   |_|   \t |_____| \\_____|   \\__/   \\_____| |_____| \n\n\n\n\n");
    printf("\t\t\t\tCongratulations! You passed Level %d.\n\n", level);
    printf("\t\t\t\t    Next Level is loading . . .\n\n\n\n", level);
    printf("\t\t\t\t\t\t\t\t\t              |\\ \n");
    printf("\t\t\t\t\t\t\t\t\t   ___________| \\ \n");
    printf("\t\t\t\t\t\t\t\t\t  |              \\ \n");
    printf("\t\t\t\t\t\t\t\t\t  |               \\ \n");
    printf("\t\t\t\t\t\t\t\t\t  |               / \n");
    printf("\t\t\t\t\t\t\t\t\t  |___________   / \n");
    printf("\t\t\t\t\t\t\t\t\t              | / \n");
    printf("\t\t\t\t\t\t\t\t\t              |/ \n");

    delay(2000);
}

int ShowScore(int level)
{
    int score, i;
    char name[20];
    score = (level * (Objects+1)*100)+((Objects+1)*100);
    system("cls");
    GotoXY(0, 5);
    printf("\t    _______   _____   _________   _____ \t  _____  __      __  _____   ____\n");
    printf("\t   |  _____| |___  | |  _   _  | |  _  |\t |  _  | \\ \\    / / |  _  | |  __|\n");
    printf("\t   | |  ___   ___| | | | | | | | | |_| |\t | | | |  \\ \\  / /  | |_| | | |\n");
    printf("\t   | | |_  | |  _  | | | |_| | | |  ___|\t | | | |   \\ \\/ /   |  ___| | |\n");
    printf("\t   | |___| | | |_| | | |     | | | |___ \t | |_| |    \\  /    | |___  | |\n");
    printf("\t   |_______| |_____| |_|     |_| |_____|\t |_____|     \\/     |_____| |_|\n");

    //--- top border -----
    GotoXY(34, 16);
    printf("%c", 201);
    for (i=0; i<30; i++)
        printf("%c", 205);
    printf("%c", 187);
    //--- bottom border -----
    GotoXY(34, 20);
    printf("%c", 200);
    for (i=0; i<30; i++)
        printf("%c", 205);
    printf("%c", 188);
    //--- left border -----
    GotoXY(34, 17);
    printf("%c", 186);
    GotoXY(34, 18);
    printf("%c", 186);
    GotoXY(34, 19);
    printf("%c", 186);
    //---- right border ----
    GotoXY(65, 17);
    printf("%c", 186);
    GotoXY(65, 18);
    printf("%c", 186);
    GotoXY(65, 19);
    printf("%c", 186);
    //-------
    GotoXY(34, 15);
    printf("Nice try, better luck next time!");
    GotoXY(36, 17);
    printf("Level : %d", level);
    GotoXY(36, 18);
    printf("Coins : %d", Objects);
    GotoXY(36, 19);
    printf("Score : %d", score);
    //------

    char hname[6][20], ntemp[20];
    int hscore[6] = {0,0,0,0,0,0}, temp, j;

    FILE *fp;

    if((fp = fopen("highscore.txt","r"))!=NULL)
    {
        for(i=1; !feof(fp); i++)
        {
            fscanf(fp, "%s %d", hname[i], &hscore[i]);
        }
    }

    fclose(fp);

    GotoXY(34, 22);
    printf("Enter you name and press <Enter>");
    GotoXY(34, 23);
    printf(">>");
    do
    {
        GotoXY(37, 23);
        gets(hname[0]);
    }
    while(strlen(hname[0]) == 0);

    hscore[0] = score;

    int outer, inner;

    for(outer=0; outer<6-1; outer++)
    {
        for(inner=outer+1; inner<6; inner++)
        {
            if(hscore[outer] <= hscore[inner])
            {
                temp=hscore[outer];
                strcpy(ntemp, hname[outer]);

                hscore[outer] = hscore[inner];
                strcpy(hname[outer], hname[inner]);

                hscore[inner] = temp;
                strcpy(hname[inner], ntemp);
            }
        }
    }

    fp = fopen("highscore.txt","w");
    for(j=0; j<5; j++)
    {
        if(hscore[j] != 0)
        {
            fprintf(fp, "\n%s %d", hname[j], hscore[j]);
        }

    }

    fclose(fp);
    return 0;
}

int HighScore()
{
    system("cls");
    GotoXY(0, 2);
    printf("         (	 \n");
    printf("     '( '          _  _      _      __ _   _                ___                                      \n");
    printf("    ''  //)       | || |    (_)    / _` | | |_             / __|    __      ___      _ _    ___      \n");
    printf("   ( '''          | __ |    | |    \\__, | | ' \\            \\__ \\   / _|    / _ \\    | '_|  / -_)     \n");
    printf("   _||__   _____  |_||_|    |_|    |___/  |_||_|   _____   |___/   \\__|    \\___/    |_|    \\___| \n");
    printf("  (o)   |_|'''''|_|'''''|_|'''''|_|'''''|_|'''''|_|'''''|_|'''''|_|'''''|_|'''''|_|'''''|_|'''''|   \n");
    printf("  'U'0-0''`-0-0-''`-0-0-''`-0-0-''`-0-0-''`-0-0-''`-0-0-''`-0-0-''`-0-0-''`-0-0-''`-0-0-''`-0-0-'\n");

    GotoXY(30,11);
    printf("%s\t\t\t%s","Name", "Score");

    GotoXY(27,22);
    printf("Press any key to go back to Main Menu.");

    char name[5][20];
    int hscore[5], i;
    FILE *fp;
    if((fp = fopen("highscore.txt","r"))==NULL)
    {
        GotoXY(35,13);
        printf("There are no record.");
    }
    else
    {
        for(i=0; !feof(fp) && i<5; i++)
        {
            fscanf(fp, "%s %d", name[i], &hscore[i]);
            GotoXY(30,13+i);
            printf("%d. %s",i+1,  name[i]);
            GotoXY(55,13+i);
            printf("%6d", hscore[i]);
        }
    }

    fclose(fp);
    getch();
    return 0;
}

int MainMenu()
{
    system("cls");
    GotoXY(0, 2);
    printf("\t                            _          _ \n");
    printf("\t                           | |        | | \n");
    printf("\t                           | |       _| |_ \n");
    printf("\t   ___     _   _   ______  | |____  |_   _|  ___    ___   _____   ______  _____ \n");
    printf("\t  |   \\   | | |_| |  __  | |  __  |   | |   |   \\  /   | |___  | |___  / |  _  | \n");
    printf("\t  | |\\ \\  | |  _  | |  | | | |  | |   | |   | |\\ \\/ /| |  ___| |    / /  | |_| | \n");
    printf("\t  | | \\ \\ | | | | | |  | | | |  | |   | |   | | \\  / | | |  _  |   / /   |  ___| \n");
    printf("\t  | |  \\ \\| | | | | |__| | | |  | |   | |   | |  \\/  | | | |_| |  / /__  | |___ \n");
    printf("\t  |_|   \\___| |_| |____  | |_|  |_|   |_|   |_|      |_| |_____| /_____| |_____| \n");
    printf("\t                   _   | | \n");
    printf("\t                  | |__| | \n");
    printf("\t                  |______| \n");
    GotoXY(42, 15);
    printf("  Start Game  ");
    GotoXY(42, 17);
    printf("  High Score  ");
    GotoXY(42, 19);
    printf("   Credits    ");
    GotoXY(42, 21);
    printf("  Exit Game   ");
    GotoXY(5, 28);
    printf("Tips : Use W, A, S and D to move and <Enter> to select menu.");
    //----
    GotoXY(75, 19);
    printf("   _________        Z");
    GotoXY(75, 20);
    printf("  /         \\     Z");
    GotoXY(75, 21);
    printf(" /           \\  Z");
    GotoXY(75, 22);
    printf("|             |");
    GotoXY(75, 23);
    printf("| __     __   |");
    GotoXY(75, 24);
    printf("|     __      |");
    GotoXY(75, 25);
    printf(" \\           /");
    GotoXY(75, 26);
    printf("  \\_________/");
    //--------

    int x = 42;
    int y = 15;
    int xBefore = 42;
    int yBefore = 15;
    int select = 1;

    while(1==1)
    {
        GotoXY(xBefore,yBefore);
        printf(" ");
        GotoXY(xBefore+13,yBefore);
        printf(" ");

        GotoXY(x,y);
        printf(">");

        GotoXY(x+13,y);
        printf("<");

        inputKey = _getch();

        xBefore = x;
        yBefore = y;
        switch (inputKey)
        {
        case 'w' :
            if(y!=15)
            {
                y -= 2;
                select--;
            }
            break;
        case 's':
            if(y!=21)
            {
                y += 2;
                select++;
            }
            break;
        case 'a' :
            if(y!=15)
            {
                y -= 2;
                select--;
            }
            break;
        case 'd':
            if(y!=21)
            {
                y += 2;
                select++;
            }
            break;
        case 13:
        {
            return select;
        }
        break;
        }
    }
    getch();
}

int Credits()
{
    system("cls");
    GotoXY(0, 4);
    printf("\t\t   ____    ____    U _____ u ____              _____   ____      \n");
    printf("\t\tU / ___|U |  _ \\ u \\| ____|/|  _ \\    ___     |_   _| / ___| u   \n");
    printf("\t\t\\| | u   \\| |_) |/  |  _|  /| | | |  |_ _|      | |  <\\___ \\/    \n");
    printf("\t\t | |/__   |  _ <    | |___ U| |_| |\\  | |      /| |\\  u___) |    \n");
    printf("\t\t  \\____|  |_| \\_\\   |_____| |____/ uU/| |\\u   u |_|U  |____/>>   \n");
    printf("\t\t _// \\\\   //   \\\\_  <<   >>  |||_.-,_|___|_,-._// \\\\_  )(  (__)  \n");
    printf("\t\t(__)(__) (__)  (__)(__) (__)(__)_)\\_)-' '-(_/(__) (__)(__)");
    GotoXY(0, 14);
    printf("\t\t\tWaristha    Tanwongwarn         EGCO    6013124\n");
    printf("\t\t\tApinya      Pituxcoosuvarn      EGCO    6013131\n");
    printf("\t\t\tAi          Kengsurakarn        EGCO    6013132\n");
    GotoXY(0, 20);
    printf("\t\t\t    Press any key to go back to Main Menu.");
    getch();
    return 0;
}

void GoodBye()
{
    system("cls");
    printf("\n\n\n\n\n\n\n\n\n         _,---.     _,.---._       _,.---._                                            ,----.            \n");
    printf("     _.='.'-,  \\  ,-.' , -  `.   ,-.' , -  `.   _,..---._    _..---.  ,--.-.  .-,--.,-.--` , \\            \n");
    printf("    /==.'-     / /==/_,  ,  - \\ /==/_,  ,  - \\/==/,   -  \\ .' .'.-. \\/==/- / /=/_ /|==|-  _.-`           \n");
    printf("   /==/ -   .-' |==|   .=.     |==|   .=.     |==|   _   _Y==/- '=' /\\==\\, \\/=/. / |==|   `.-.           \n");
    printf("   |==|_   /_,-.|==|_ : ;=:  - |==|_ : ;=:  - |==|  .=.   |==|-,   '  \\==\\  \\/ -/ /==/_ ,    /           \n");
    printf("   |==|  , \\_.' )==| , '='     |==| , '='     |==|,|   | -|==|  .=. \\  |==|  ,_/  |==|    .-'            \n");
    printf("   \\==\\-  ,    ( \\==\\ -    ,_ / \\==\\ -    ,_ /|==|  '='   /==/- '=' ,| \\==\\-, /   |==|_  ,`-._      \n");
    printf("    /==/ _  ,  /  '.='. -   .'   '.='. -   .' |==|-,   _`/==|   -   /  /==/._/    /==/ ,     /           \n");
    printf("    `--`------'     `--`--''       `--`--''   `-.`.____.'`-._`.___,'   `--`-`     `--`-----``           \n");
    delay(3000);
}

int main()
{
    int select;
    WinSize();
    hideCursor();
    select = MainMenu();
    while(1)
    {
        switch (select)
        {
        case 0:
            select = MainMenu();
            break;
        case 1:
            select = StartGame();
            break;
        case 2:
            select = HighScore();
            break;
        case 3:
            select = Credits();
            break;
        case 4:
            GoodBye();
            exit(EXIT_SUCCESS);
            break;
        }
    }
    //Credits();
    //StartGame();
    return 0;
}
