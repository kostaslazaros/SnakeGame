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
    snake1->symbol='*';
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
        printf("X");
        gotoxy(horizontal,i);
        printf("X");
    }

    for (i=0; i<horizontal; ++i)
        {
            gotoxy(i,0);
            printf("X");
            gotoxy(i,vertical);
            printf("X");
        }
}





































































































































