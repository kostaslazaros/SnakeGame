


/**********************************************************
*           SNAKE GAME FOR LINUX TERMINAL                 *
*           -----------------------------                 *
*  Developers: Konstantinos Lazaros & Nefeli Stefanatou   *
*  Lesson    : Advanced Programming                       *
***********************************************************/

#include <stdio.h>/*functions for performing input and output.*/
#include <time.h>/*functions for manipulating date and time.*/
#include <math.h>/* defines various mathematical functions .*/
#include <string.h>/* functions for manipulating arrays of characters.*/
#include <ctype.h>/*functions that are useful for testing and mapping characters.*/
#include <stdlib.h>/* functions for performing general functions*/
#include <termios.h>/*contains the definitions used by the terminal I/O interfaces.termios general terminal inerface gives an interface for asynchronous communication ports (syskeyes epikoinonias). form of serial communication
ports where the communication endparts interfaces are not synchronixed by a common clc*/
#include <unistd.h>/*declares miscellaneous functions*/
#include <fcntl.h>

#define UP_ARROW 'w' /*for w to run as up arrow */
#define LEFT_ARROW 'a'/*for a to run as left arrow*/
#define RIGHT_ARROW 'd'/*for d to run as right arrow */
#define DOWN_ARROW 's'/*for s to run as dwon arrow*/
#define ENTER_KEY 10 /*we type 10 to run the game in terminal*/
#define SNAKE_SIZE 310/*snake size is defines as 310*/


const char SNAKE_HEAD = '@';/*that is head of snake*/
const char SNAKE_BODY = '*';/*that is snake body*/
const char WALL  = '|';/*that is wall right left*/
const char CEIL  = '-';/*that is ceiling & down */
const char FOOD  = 'X';/*that is food*/
const char BLANK = ' ';


void gotoxy(int x,int y) { /*goes the cursor where we tell him to go (x,y is the position where we place the cursor)*/
    printf("%c[%d;%df",0x1B,y,x);
    /*[ and ; (between %d and %df) this is just put inside the the printf statement to display itself on output screen just like we print “hello”*/
/*the statement 0x1B hexadecimal number for 27 and ascii value 27 stand for “?” this symbol
%c represent "0X1B"
%d represent "y"
%df reprsent "x"
the 0x1B, which is equivalent to 27 in decimals, and is translated as the escape character (ESC) in the ASCII coding system.
 The rest now becomes an ANSI escape sequence and thus, is used for cursor positioning. Ansi escape sequence is a standar for in-band signaling(process where you
 send info into the same band or channel) to control color, position etc options in terminals*/
}


int kbhit(void) {
/*checks if keys where pressed or not. If a key has been pressed then it returns a non zero value otherwise returns zero. */
  struct termios oldt, newt;

  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  /*declared as:int tcgetattr( int fd, struct termios termios_p);
The tcgetattr() function shall get the parameters associated with the termial reffered to by the fildes
and store them in the termios structure referenced by termios_p. The fidles argument is an open file descriptor associated with a terminal*/
    /*retrieve old terminal settings */
    newt = oldt;
    /*Disable canonical input mode, and input characters echoing*/
  newt.c_lflag &= ~(ICANON | ECHO);
  /*c_lflag=local modes */
  /*echo= writes arguments to the standard output*/

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

/* Reads a character from input. It is a way to get a user inputted character. */
char getch() {
    char c;
    system("stty raw");/*The raw setting means that the input and output is not processed, just sent straight through. */
    c= getchar();/*get the character and put it in c*/
    system("stty sane");/*sets al modes of the system to resonable valuses*/
    return(c);/*return the character*/
}


void clrscr() {
    system("clear");
    return;
}
/*clears things*/

/*while user hasnt pressed a key wait for key and when pressed return the pressed key*/
char waitForAnyKey(void) {
    int pressed;
    while(!kbhit());
    pressed = getch();
    return((char)pressed);
}

/*the game speed is set as 1*/
int getGameSpeed() {
    int speed = 1;
    clrscr();/*clears the screen and move cursor to upper left hand corner of screen.*/

    return(speed);

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
    for (i = detect; i < snakeLength; i++) { /*checks if it collided withitslf aka forming a "cross"*/
        if (x == snakeXY[0][i] && y == snakeXY[1][i])
            return(1);
    }
    return(0);
}

//Generates food & Makes sure the food doesn't appear on top of the snake
int generateFood(int foodXY[], int width, int height, int snakeXY[][SNAKE_SIZE], int snakeLength) {
    do {
        srand ( time(NULL) );/*uses surrent time as seed to rand*/
        foodXY[0] = rand() % (width-2) + 2; /*x random coordinate */
        srand ( time(NULL) );
        /* foodXY[]is an array where coordinates of x and y of food is stored. foodXY[0] is for food's x coord
        and foodXY[1] is for food's y coord */
        foodXY[1] = rand() % (height-6) + 2;/*y random coordinate*/
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
/*moves head and makes sure that each body part takes the place of the body part that was before it*/
    int i;
    /*snakeXY is a board that holds coordinates of the snake.snakeXY[i][0] is for the X coordinates
    and snakeXY is for the Y coordinates*/
    for (i = snakeLength-1; i >= 1; i--) {/*goes from end to beggining till laxt * not head tho.moves only body.each
    body part takes the place of the body part that was before it */
        snakeXY[0][i] = snakeXY[0][i-1];
        snakeXY[1][i] = snakeXY[1][i-1];
    }
    /*moves head*/
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
/*goes the cursor to the body part that is before the last, then removes the last body part by printing a blank charcter
then changes the head to a body part, uses snakearray to move forward and prints head where it should be*/

    int x;
    int y;
    /*goes the cursor to the body part before the last one. like that for ex. : @*****(cursor here)* */
    x = snakeXY[0][snakeLength-1];/*snakeXY[0][] is to store x coords
    snakeXY[1][] is to store y coords */
    y = snakeXY[1][snakeLength-1];
    gotoxy(x,y);
    /*removes the last * bit of tail by printing the blank character in its place  */
    printf("%c",BLANK);
   /* changes the head to a snake body part * */
    gotoxy(snakeXY[0][0],snakeXY[1][0]);
    printf("%c", SNAKE_BODY);

    moveSnakeArray(snakeXY, snakeLength, direction);
    /*moves one and puts in first place head  */
    gotoxy(snakeXY[0][0],snakeXY[1][0]);
    printf("%c",SNAKE_HEAD);

    gotoxy(1,1);
    /*goes the cursor to 1,1*/
    return;
}


int eatFood(int snakeXY[][SNAKE_SIZE], int foodXY[]) {
/*checks if foods coords are same with snake's coords so food will be eaten. if food eaten
return 1 if not 0*/
    if (snakeXY[0][0] == foodXY[0] && snakeXY[1][0] == foodXY[1]) {
    /*checks if food coordinates ssame with snake coordiates*/
        foodXY[0] = 0;
        foodXY[1] = 0;
        printf("\7"); //Beep
        return(1);
        /*then food is eaten*/
    }
    return(0);/*food not eaten*/
}

/*checks if the snake collided somewhere (walls or on itself*/
int collisionDetection(int snakeXY[][SNAKE_SIZE], int consoleWidth, int consoleHeight, int snakeLength ) {

    int colision = 0;
    /*intializes the collission as false */
    // Checks if the snake collided with the wall
    if ((snakeXY[0][0] == 1) || (snakeXY[1][0] == 1) ||
        (snakeXY[0][0] == consoleWidth) || (snakeXY[1][0] == consoleHeight - 4))
        colision = 1;
    else
        // If the snake collided with the wall, there's no point in checking if it collided with itself.
        /*checks if snake coolided with itself */
        if (collisionSnake(snakeXY[0][0], snakeXY[1][0], snakeXY, snakeLength, 1))
            colision = 1;
    return(colision);
}


void refreshInfoBar(int score, int speed) {
/* intobar we have below*/
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

/* appears when you lose */
void gameOverScreen(void) {
    int x = 7, y = 3;
    gotoxy(x,y++);
    printf("*************  *********        ****      **********  *********\n");
    gotoxy(x,y++);
    printf("*************  *********       **  **     **********  *********\n");
    gotoxy(x,y++);
    printf("     **        **             *     **    * *    * *   *\n");
    gotoxy(x,y++);
    printf("     **        ******        **     **    * *    * *    *\n");
    gotoxy(x,y++);
    printf("     **        ******       *        **   * *    * *    *\n");
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

/*stars game. puts game over as false */
void startGame(int snakeXY[][SNAKE_SIZE],
               int foodXY[],
               int consoleWidth,
               int consoleHeight,
               int snakeLength,
               int direction,
               int score,
               int speed)

{
    int gameOver = 0;/*puts gameover as false*/
    clock_t endWait;/* clock counts */
    /*stops watining for snake to move on console*/

    // CLOCKS_PER_SEC-(n-1)*(CLOCKS_PER_SEC/10)
    // Sets the correct wait time according to the selected speed
    int waitMili = CLOCKS_PER_SEC-(speed)*(CLOCKS_PER_SEC/10);
    /*how much to wait to move on the concole*/
    /*stops watining for snake to move on console*/
    int tempScore = 10*speed;
    /*temporarily saves score*/

    int oldDirection;
    int canChangeDirection = 1;
    endWait = clock() + waitMili;
    do
    {
    /*checks if it can change direction. do an example with directions to make clear*/
        if (canChangeDirection) {
            oldDirection = direction;
            direction = checkKeysPressed(direction);
        }
        if (oldDirection != direction)
            canChangeDirection = 0;
            /* if old dir same as now then dont change dir*/

        if (clock() >= endWait) {
            move(snakeXY, snakeLength, direction);
            canChangeDirection = 1;
            /* if clock ( clock from the time the program started) is grater than end wait*/

            /*if eat food then make more food*/
            if (eatFood(snakeXY, foodXY)) {
                generateFood( foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
                snakeLength++;
                score += 10;
                /* make the speed bigger when needed */
                if(score >= 10 *speed + tempScore) {
                    speed++;
                    tempScore = score;

                    if(speed <= 9)
                        waitMili = waitMili - (CLOCKS_PER_SEC / 10);
                    else {/*it will have reached maximum speed*/
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

/*loads everything that should be loaded for the game to start*/
void loadEnviroment(int consoleWidth, int consoleHeight) {

    int x = 1, y = 1;
    int rectangleHeight = consoleHeight - 4;
    clrscr(); //clear the console
    gotoxy(x,y); //Top left corner
    for (; y < rectangleHeight; y++) {
    /*i made the left wall*/
        gotoxy(x, y); //Left Wall
        printf("%c",WALL);
          /*i make right wall*/
        gotoxy(consoleWidth, y); //Right Wall
        printf("%c",WALL);
    }
    y = 1;
    for (; x < consoleWidth+1; x++) {
  /*makes the up part of screen*/
        gotoxy(x, y); //Left Wall
        printf("%c",CEIL);
/*makes floor*/
        gotoxy(x, rectangleHeight); //Right Wall
        printf("%c",CEIL);
    }
    return;
}

/*prints snakes body*/
void loadSnake(int snakeXY[][SNAKE_SIZE], int snakeLength) {
    int i;
    /*prints snake body*/
    for (i = 0; i < snakeLength; i++) {
        gotoxy(snakeXY[0][i], snakeXY[1][i]);
        printf("%c", SNAKE_BODY);
    }
    return;
}

/*prepares the snake*/
void prepairSnakeArray(int snakeXY[][SNAKE_SIZE], int snakeLength) {
    int i;
    int snakeX = snakeXY[0][0];/*x coord of head*/
    int snakeY = snakeXY[1][0];/*y coord of head*/
    for (i = 1; i <= snakeLength; i++) {
        snakeXY[0][i] = snakeX + i;
        snakeXY[1][i] = snakeY;
        /* makes the snake appera like ***@ */
    }
    return;
}


/*This function loads the environment, snake, etc*/
void loadGame(void) {

    // Two Dimensional Array, the first array is for the X coordinates
    // and the second array for the Y coordinates

    int snakeXY[2][SNAKE_SIZE];
    int snakeLength = 2; //Starting Length
    int direction = LEFT_ARROW;//*starting dir*/
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
