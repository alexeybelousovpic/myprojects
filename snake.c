#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

#include "lib/snake.h"

void renderSnake(Window* W, Snake* S){
    CoordLL* first = S->first;
    while(S->first){
        wmvaddch(W,S->first->loc->y,S->first->loc->x,ACS_DIAMOND);
        S->first = S->first->next;
    }
    S->first = first;
    wrefresh(W);
}
bool moveSnake(GameWindow* GW, Snake* S, int choice){
    bool eating = reachingFood(S,choice);
    int xShift, yShift, newX, newY;
    xShift = yShift = 0;
    if(S->lastDir == KEY_UP && choice == KEY_DOWN) choice = KEY_UP;
    if(S->lastDir == KEY_DOWN && choice == KEY_UP) choice = KEY_DOWN;
    if(S->lastDir == KEY_LEFT && choice == KEY_RIGHT) choice = KEY_LEFT;
    if(S->lastDir == KEY_RIGHT && choice == KEY_LEFT) choice = KEY_RIGHT;
    switch(choice){
        case KEY_UP:
            yShift = -1;
            break;
        case KEY_DOWN:
            yShift = 1;
            break;
        case KEY_LEFT:
            xShift = -1;
            break;
        case KEY_RIGHT:
            xShift = 1;
            break;
    }
    newX = S->first->loc->x + xShift;
    newY = S->first->loc->y + yShift;
    if(newX > 0 && newY > 0 && newX < S->bounds->x && newY < S->bounds->y
            && (eating || !isOccupied(GW,newY,newX,S->bounds->x))){
            if(!eating){
                mvwprintw(GW->W,S->last->loc->y,S->last->loc->x," ");
                GW->isOccupied[toOneD(S->last->loc->y,S->last->loc->x,S->bounds->x)] = false;
                S->first->prev = S->last; // Reusing tail memory as head
                S->last = S->last->prev;
                S->first = S->first->prev;
                S->first->prev = S->last->next = NULL;
                S->first->loc->y = newY;
                S->first->loc->x = newX;
            }
            else{
                CoordLL* newHead = calloc(1,sizeof(CoordLL));
                Coord* newCoord = calloc(1,sizeof(Coord));
                if(!newHead || !newCoord){
                    printf("Allocation of head or head's coordinate failed!");
                    exit(1);
                }
                newHead->loc = newCoord;
                newHead->loc->x = newX;
                newHead->loc->y = newY;
                newHead->next = S->first;
                S->first->prev = newHead;
                S->first = S->first->prev;
                S->first->prev = NULL;
                S->len++;
                placeFood(GW,S);
            }
            GW->isOccupied[toOneD(newY,newX,S->bounds->x)] = true;

            // Unfortunately clunky for now
            switch(choice){
                case KEY_UP:
                    S->lastDir = KEY_UP;
                    break;
                case KEY_DOWN:
                    S->lastDir = KEY_DOWN;
                    break;
                case KEY_LEFT:
                    S->lastDir = KEY_LEFT;
                    break;
                case KEY_RIGHT:
                    S->lastDir = KEY_RIGHT;
                    break;
            }
            return false;
    }
    return true;
}
