#include <ncursesw/ncurses.h>

int main(){
initscr();
curs_set(0);
noecho();
keypad(stdscr, TRUE);

int x = 5;
int y = 5;

for(;;){
mvprintw(y, x, "X");

int ch = getch();
if (ch == KEY_DOWN) y++;
if (ch == KEY_UP) y--;
if (ch == KEY_LEFT) x--;
if (ch == KEY_RIGHT) x++;

clear();
refresh();
}
endwin();
}
