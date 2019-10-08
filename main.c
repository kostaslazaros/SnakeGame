#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>

#define vertical 40
#define horizontal 200
#define down 115
#define up 119
#define left 97
#define right 100


typedef struct Snake
{
    char symbol;
    int size;
    char direction;
    char prev_direction;
    int tail_X;
    int tail_Y;
    int head_X;
    int head_Y;

}snake;


typedef struct snake_pos
{
    int Y[vertical*horizontal];
    int X[vertical*horizontal];

}snake_pos;


typedef struct food
{
    int X;
    int Y;
    char symbol;

}food;


void snake_init(snake *snake1);
void pos_init(snake_pos *pos1);
void food_init(food *food1);
void gotoxy(int,int);
void snake_place(snake *snake1, snake_pos *pos1);
void snake_move(snake *snake1, snake_pos *pos1, food *food1, int*);
void move_tail(snake *snake1, snake_pos *pos1);
void move_head(snake *snake1, snake_pos *pos1);
void food_print(food *food1);
int game_over(snake *snake1, snake_pos *pos1);
void set_borders();
void print_score(int*);











void snake_init(snake *snake1)
{
    snake1->symbol='@';
    snake1->size=10;
    snake1->direction=right;
    snake1->prev_direction=down;
    snake1->tail_X=5;
    snake1->tail_Y=5;
    snake1->head_X=snake1->tail_X+snake1->size-1;
    snake1->head_Y=5;
}


void snake_place(snake *snake1, snake_pos *pos1)
{
    int i;
    for (i=0; i<snake1->size; ++i)
    {
        gotoxy(snake1->tail_X,snake1->tail_Y);
        printf("%c",snake1->symbol);
        pos1->X[i]=snake1->tail_X;
        pos1->Y[i]=snake1->tail_Y;
        snake1->tail_X+=1;
    }
}


void set_borders()
{
    int i;
    for (i=0; i<vertical; ++i)
    {
        gotoxy(0,i);
        printf("|");
        gotoxy(horizontal,i);
        printf("|");
    }

    for (i=0; i<horizontal; ++i)
        {
            gotoxy(i,0);
            printf("-");
            gotoxy(i,vertical);
            printf("-");
        }
}


void snake_move(snake *snake1, snake_pos *pos1, food *food1, int *score)
{
    move_head(snake1, pos1);

    if(!((snake1 -> head_X == food1 -> X) && (snake1 -> head_Y == food1 -> Y)))
    {
        move_tail(snake1, pos1);
    }
    else
    {
        snake1 -> size++;
        *score = *score + 10;
        food1 -> X = rand() % (horizontal - 5);
        food1 -> Y = rand() % (vertical - 5);
        food_print(food1);
    }
}


void move_tail(snake *snake1, snake_pos *pos1)
{
    int i;

    //removing the last cell from the tail
    gotoxy(pos1 -> X[0], pos1 -> Y[0]);
    printf(" ");

    //update of the new position of the tail
    for(i = 0; i < snake1 -> size; ++i)
    {
        pos1 -> X[i] = pos1 -> X[i + 1];
        pos1 -> Y[i] = pos1 -> Y[i + 1];
    }
}


void move_head(snake *snake1, snake_pos *pos1)
{
    switch (snake1 -> direction)
    {
        case right:
            if(snake1 -> prev_direction == left)
            {
                snake1 -> head_X--;
                break;
            }
                snake1 -> head_X++;
                break;

        case left:
            if(snake1 -> prev_direction == right)
            {
                snake1 -> head_X++;
                break;
            }
                snake1 -> head_X--;
                break;

        case up:
            if(snake1 -> prev_direction == down)
            {
                snake1 -> head_Y++;
                break;
            }
                snake1 -> head_Y--;
                break;

        case down:
            if(snake -> prev_direction == up)
            {
                snake1 -> head_Y--;
                break;
            }
                snake1 -> head_Y++;
                break;

        default:
            break;
    }

    //update tail position here
    pos1 -> X[snake1 -> size] = snake1 -> head_X;
    pos1 -> Y[snake1 -> size] = snake1 -> head_Y;
    gotoxy(pos1 -> X[snake1 -> size], pos1 -> Y[snake1 -> size]);
    printf("%c", snake1 -> symbol);
}

//This is where food is initialized
void food_init(food *food1)
{
    food1 -> X = (rand() % (horizontal - 5)) + 1;
    food1 -> Y = (rand() % (vertical - 5)) + 1;
    food1 -> symbol = 'F';
}


void food_print(food *food1)
{
    gotoxy(food1 -> X, food1 -> Y);
    printf("%c", food1 -> symbol);
}


void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, x, y);
}





































































































































































