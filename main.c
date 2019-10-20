/**********************************************************
*           SNAKE GAME FOR LINUX TERMINAL                 *
*           -----------------------------                 *
*  Developers: Konstantinos Lazaros & Nefeli Stefanatou   *
*  Lesson    : Advanced Programming                       *
***********************************************************/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define UP_ARROW 'w'
#define LEFT_ARROW 'a'
#define RIGHT_ARROW 'd'
#define DOWN_ARROW 's'
#define ENTER_KEY 10
// #define EXIT_BUTTON 27 //ESC
// #define PAUSE_BUTTON 112 //P
#define SNAKE_SIZE 310


const char SNAKE_HEAD = '@';
const char SNAKE_BODY = '*';
const char WALL = '|';
const char CEIL = '-';
const char FOOD = 'X';
const char BLANK = ' ';


void gotoxy(int x,int y) {
    printf("%c[%d;%df",0x1B,y,x);
}


int kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}


char getch() {
    char c;
    system("stty raw");
    c= getchar();
    system("stty sane");
    return(c);
}


void clrscr() {
    system("clear");
    return;
}


char waitForAnyKey(void) {
    int pressed;
    while(!kbhit());
    pressed = getch();
    return((char)pressed);
}


int getGameSpeed() {
    int speed = 1;
    clrscr();

    return(speed);
    return 1;
}


// This function checks if a key has pressed,
// then checks if its any of the wsad keys.
// It changes direction acording to the key pressed.
int checkKeysPressed(int direction) {
    int pressed;
    if(kbhit()) { //If a key has been pressed
        pressed=getch();
        if (direction != pressed) {
            if(pressed == DOWN_ARROW && direction != UP_ARROW)
                direction = pressed;
            else if (pressed == UP_ARROW && direction != DOWN_ARROW)
                direction = pressed;
            else if (pressed == LEFT_ARROW && direction != RIGHT_ARROW)
                direction = pressed;
            else if (pressed == RIGHT_ARROW && direction != LEFT_ARROW)
                direction = pressed;
        }
    }
    return(direction);
}

// Cycles around checking if the x y coordinates ='s the snake coordinates as one of this parts
// One thing to note, a snake of length 4 cannot collide with itself,
// therefore there is no need to call this function when the snakes length is <= 4
int collisionSnake (int x, int y, int snakeXY[][SNAKE_SIZE], int snakeLength, int detect) {
    int i;
    for (i = detect; i < snakeLength; i++) { //Checks if the snake collided with itself
        if (x == snakeXY[0][i] && y == snakeXY[1][i])
            return(1);
    }
    return(0);
}

//Generates food & Makes sure the food doesn't appear on top of the snake <-
// This sometimes causes a lag issue!!! Not too much of a problem tho
int generateFood(int foodXY[], int width, int height, int snakeXY[][SNAKE_SIZE], int snakeLength) {
    do {
        srand ( time(NULL) );
        foodXY[0] = rand() % (width-2) + 2;
        srand ( time(NULL) );
        foodXY[1] = rand() % (height-6) + 2;
      // This should prevent the "Food" from being created on top of the snake.
      // However the food has a chance to be created ontop of the snake,
      // in which case the snake should eat it...
    } while (collisionSnake(foodXY[0], foodXY[1], snakeXY, snakeLength, 0));

    gotoxy(foodXY[0] ,foodXY[1]);
    printf("%c", FOOD);
    return(0);
}

/*
Moves the snake array forward, i.e.
This:
 x 1 2 3 4 5 6
 y 1 1 1 1 1 1
Becomes This:
 x 1 1 2 3 4 5
 y 1 1 1 1 1 1

 Then depending on the direction (in this case west - left) it becomes:

 x 0 1 2 3 4 5
 y 1 1 1 1 1 1

 snakeXY[0][0]--; <- if direction left, take 1 away from the x coordinate
*/
void moveSnakeArray(int snakeXY[][SNAKE_SIZE], int snakeLength, int direction) {
    int i;
    for (i = snakeLength-1; i >= 1; i--) {
        snakeXY[0][i] = snakeXY[0][i-1];
        snakeXY[1][i] = snakeXY[1][i-1];
    }
    switch(direction) {
        case DOWN_ARROW:
            snakeXY[1][0]++;
            break;
        case RIGHT_ARROW:
            snakeXY[0][0]++;
            break;
        case UP_ARROW:
            snakeXY[1][0]--;
            break;
        case LEFT_ARROW:
            snakeXY[0][0]--;
            break;
    }
    return;
}


void move(int snakeXY[][SNAKE_SIZE], int snakeLength, int direction) {
    int x;
    int y;
    //Remove the tail ( HAS TO BE DONE BEFORE THE ARRAY IS MOVED!!!!! )
    x = snakeXY[0][snakeLength-1];
    y = snakeXY[1][snakeLength-1];
    gotoxy(x,y);
    printf("%c",BLANK);
    //Changes the head of the snake to a body part
    gotoxy(snakeXY[0][0],snakeXY[1][0]);
    printf("%c", SNAKE_BODY);
    moveSnakeArray(snakeXY, snakeLength, direction);
    gotoxy(snakeXY[0][0],snakeXY[1][0]);
    printf("%c",SNAKE_HEAD);
    gotoxy(1,1); //Gets rid of the darn flashing underscore.
    return;
}


int eatFood(int snakeXY[][SNAKE_SIZE], int foodXY[]) {
    if (snakeXY[0][0] == foodXY[0] && snakeXY[1][0] == foodXY[1]) {
        foodXY[0] = 0;
        foodXY[1] = 0;
        printf("\7"); //Beep
        return(1);
    }
    return(0);
}


int collisionDetection(int snakeXY[][SNAKE_SIZE], int consoleWidth, int consoleHeight, int snakeLength ) {
    int colision = 0;
    // Checks if the snake collided wit the wall or it's self
    if ((snakeXY[0][0] == 1) || (snakeXY[1][0] == 1) ||
        (snakeXY[0][0] == consoleWidth) || (snakeXY[1][0] == consoleHeight - 4))
        colision = 1;
    else
        // If the snake collided with the wall, there's no point in checking if it collided with itself.
        if (collisionSnake(snakeXY[0][0], snakeXY[1][0], snakeXY, snakeLength, 1))
            colision = 1;
    return(colision);
}


void refreshInfoBar(int score, int speed) {
    gotoxy(45,24);
    printf("Score: %d", score);
    gotoxy(30,24);
    printf("Speed: %d", speed);
    gotoxy(5,22);
    printf("Developers: Konstantinos Lazaros & Nefeli Stefanatou");
    gotoxy(5,24);
    printf("Version   : 0.1");
    return;
}


void gameOverScreen(void) {
    int x = 7, y = 3;
    gotoxy(x,y++);
    printf("*************  *********        ****      **********  *********\n");
    gotoxy(x,y++);
    printf("*************  *********       **  **     **********  *********\n");
    gotoxy(x,y++);
    printf("     **        **            /*     **    * *    * *   *\n");
    gotoxy(x,y++);
    printf("     **        ******        **     **    * *    * *    *\n");
    gotoxy(x,y++);
    printf("     **        ******      /*        **   * *    * *    *\n");
    gotoxy(x,y++);
    printf("     **        **          **        **   * *    * *   *\n");
    gotoxy(x,y++);
    printf("     **        *********  **          **  **********  *********\n");
    gotoxy(x,y++);
    printf("     **        *********  **          **  **********  *********\n");
    waitForAnyKey();
    clrscr(); //clear the console
    return;
}


void startGame(int snakeXY[][SNAKE_SIZE],
               int foodXY[],
               int consoleWidth,
               int consoleHeight,
               int snakeLength,
               int direction,
               int score,
               int speed)
{
    int gameOver = 0;
    clock_t endWait;

    // CLOCKS_PER_SEC-(n-1)*(CLOCKS_PER_SEC/10)
    // Sets the correct wait time according to the selected speed
    int waitMili = CLOCKS_PER_SEC-(speed)*(CLOCKS_PER_SEC/10);
    int tempScore = 10*speed;
    int oldDirection;
    int canChangeDirection = 1;
    endWait = clock() + waitMili;
    do
    {
        if (canChangeDirection) {
            oldDirection = direction;
            direction = checkKeysPressed(direction);
        }
        if (oldDirection != direction)
            canChangeDirection = 0;
        if (clock() >= endWait) {
            move(snakeXY, snakeLength, direction);
            canChangeDirection = 1;
            if (eatFood(snakeXY, foodXY)) {
                generateFood( foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
                snakeLength++;
                score += 10;
                if(score >= 10 *speed + tempScore) {
                    speed++;
                    tempScore = score;
                    if(speed <= 9)
                        waitMili = waitMili - (CLOCKS_PER_SEC / 10);
                    else {
                        if(waitMili >= 40)
                            waitMili = waitMili - (CLOCKS_PER_SEC / 200);
                    }
                }
                refreshInfoBar(score, speed);
            }
            endWait = clock() + waitMili;
        }
        gameOver = collisionDetection(snakeXY, consoleWidth, consoleHeight, snakeLength);
        if (snakeLength >= SNAKE_SIZE-5) {
            gameOver = 2;
            score+=1500;
        }
    } while (!gameOver);
    switch(gameOver) {
        case 1:
            printf("\7"); //Beep
            printf("\7"); //Beep
            gameOverScreen();
            break;
    }
    return;
}


void loadEnviroment(int consoleWidth, int consoleHeight) {
    int x = 1, y = 1;
    int rectangleHeight = consoleHeight - 4;
    clrscr(); //clear the console
    gotoxy(x,y); //Top left corner
    for (; y < rectangleHeight; y++) {
        gotoxy(x, y); //Left Wall
        printf("%c",WALL);
        gotoxy(consoleWidth, y); //Right Wall
        printf("%c",WALL);
    }
    y = 1;
    for (; x < consoleWidth+1; x++) {
        gotoxy(x, y); //Left Wall
        printf("%c",CEIL);
        gotoxy(x, rectangleHeight); //Right Wall
        printf("%c",CEIL);
    }
    return;
}


void loadSnake(int snakeXY[][SNAKE_SIZE], int snakeLength) {
    int i;
    for (i = 0; i < snakeLength; i++) {
        gotoxy(snakeXY[0][i], snakeXY[1][i]);
        printf("%c", SNAKE_BODY);
    }
    return;
}


void prepairSnakeArray(int snakeXY[][SNAKE_SIZE], int snakeLength) {
    int i;
    int snakeX = snakeXY[0][0];
    int snakeY = snakeXY[1][0];
    for (i = 1; i <= snakeLength; i++) {
        snakeXY[0][i] = snakeX + i;
        snakeXY[1][i] = snakeY;
    }
    return;
}


//This function loads the environment, snake, etc
void loadGame(void) {
    // Two Dimensional Array, the first array is for the X coordinates
    // and the second array for the Y coordinates
    int snakeXY[2][SNAKE_SIZE];
    int snakeLength = 4; //Starting Length
    int direction = LEFT_ARROW;
    int foodXY[] = {5,5}; // Stores the location of the food
    int score = 0;
    //Window Width * Height
    int consoleWidth = 80;
    int consoleHeight = 25;
    int speed = getGameSpeed();
    //The starting location of the snake
    snakeXY[0][0] = 40;
    snakeXY[1][0] = 10;
    loadEnviroment(consoleWidth, consoleHeight); //borders
    prepairSnakeArray(snakeXY, snakeLength);
    loadSnake(snakeXY, snakeLength);
    generateFood( foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
    refreshInfoBar(score, speed); //Bottom info bar. Score, Level etc
    startGame(snakeXY, foodXY, consoleWidth, consoleHeight, snakeLength, direction, score, speed);
    return;
}


int main() {
    loadGame();
    return(0);
}
