#include <ncurses.h>
#include <bits/stdc++.h>
#include "elements.h"

using namespace std;

int main(){
    initscr();
    noecho();
    use_default_colors();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_YELLOW, -1);
    refresh();

    int ROW, COL;
    getmaxyx(stdscr, ROW, COL);
    ROW--;
    ioFrame * code = new ioFrame(0, 0, ROW, (2*COL)/3, "Editor");
    ioFrame * arg = new ioFrame((2*COL)/3, 0, ROW/4, COL-(2*COL)/3, "Arguments");
    ioFrame * input = new ioFrame((2*COL)/3, ROW/4, ROW/2-ROW/4, COL-(2*COL)/3, "Stdin");
    ioFrame * output = new ioFrame((2*COL)/3, ROW/2, ROW-ROW/2, COL-(2*COL)/3, "Stdout");
    toolbar * toolb = new toolbar("Terminal IDE");
    
    code->focus();

    int ch;
    while((ch=getch())!=KEY_F(1)){
        continue;
    }
    
    endwin();
}