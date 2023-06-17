#include <ncurses.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#define FALSE 0
#define TRUE 1

// #define speed 1000000
// #define speed 500000
// #define speed 250000
// #define speed 100000
#define speed 50000

class item
{
public:
    int condition; // 10 = growth, -10 = poison
    int Is;        // 1 true 0 false
};

// snake 클래스입니다.
class snake
{
    int snake__;
    int y;
    int len;

public:
    snake *point;

    void set(int a, int b)
    {
        snake__ = a;
        y = b;
        len = 3;
        point = new snake;
        (this->point)->snake__ = a + 1;
        (this->point)->y = b;
        (this->point)->point = new snake;
        ((this->point)->point)->snake__ = a + 2;
        ((this->point)->point)->y = b;
        ((this->point)->point)->point = NULL;
    }
    int getX()
    {
        return snake__;
    }
    int getY()
    {
        return y;
    }
    int getLen()
    {
        return len;
    }
    void add_body()
    {
        len++;

        int temp_x = 0, temp_y = 0;

        snake *temp, *temp2;
        temp = new snake;
        temp2 = new snake;
        temp = this;
        while (1)
        {
            if (temp->point == NULL)
            {
                break;
            }
            temp_x = temp->snake__;
            temp_y = temp->y;
            temp = temp->point;
        }
        // 2 case
        if (temp_x - temp->snake__ == 1)
        {
            temp2->snake__ = temp->snake__ - 1;
            temp2->y = temp->y;
        }
        else if (temp_x - temp->snake__ == -1)
        { // 1 case
            temp2->snake__ = temp->snake__ + 1;
            temp2->y = temp->y;
        }
        else if (temp_y - temp->y == 1)
        { // 4 case
            temp2->snake__ = temp->snake__;
            temp2->y = temp->y - 1;
        }
        else
        { // 3case
            temp2->snake__ = temp->snake__;
            temp2->y = temp->y + 1;
        }
        temp2->point = NULL;
        temp->point = temp2;
    }
    void delete_body()
    {
        len--;
        snake *temp;
        temp = new snake;
        temp = this;
        while (1)
        {
            if ((temp->point)->point == NULL)
                break;

            temp = temp->point;
        }
        delete temp->point;
        temp->point = NULL;
    }

    void move(int input)
    {
        int i;
        int last_x = 0, last_y = 0;
        int temp_x = 0, temp_y = 0;
        snake *temp;
        temp = new snake;
        temp = this;

        i = len - 1;
        last_x = snake__;
        last_y = y;
        if (input == KEY_UP)
        {
            snake__ -= 1;
        }
        else if (input == KEY_DOWN)
        {
            snake__ += 1;
        }
        else if (input == KEY_LEFT)
        {
            y -= 1;
        }
        else if (input == KEY_RIGHT)
        {
            y += 1;
        }

        for (; i > 0; i--)
        {
            temp = temp->point;
            temp_x = temp->snake__;
            temp_y = temp->y;
            temp->snake__ = last_x;
            temp->y = last_y;
            last_x = temp_x;
            last_y = temp_y;
        }
    }
};