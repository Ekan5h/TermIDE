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
    ioFrame * frames[] = { 
        new ioFrame(0, 0, ROW, (2*COL)/3, "Editor"),
        new ioFrame((2*COL)/3, 0, ROW/4, COL-(2*COL)/3, "Arguments"),
        new ioFrame((2*COL)/3, ROW/4, ROW/2-ROW/4, COL-(2*COL)/3, "Stdin"),
        new ioFrame((2*COL)/3, ROW/2, ROW-ROW/2, COL-(2*COL)/3, "Stdout")
    };
    toolbar * toolb = new toolbar("Terminal IDE");
    
    int focussed = 1;

    frames[focussed]->content = "Code\nEditor\n";
    frames[focussed]->putstr(frames[focussed]->content);
    frames[focussed]->focus();

    int ch;
    while((ch=getch())!=KEY_F(1)){
        switch (ch){
        case KEY_DOWN:
            frames[focussed]->curdown();
            break;
        case KEY_UP:
            frames[focussed]->curup();
            break;
        case KEY_LEFT:
            frames[focussed]->curleft();
            break;
        case KEY_RIGHT:
            frames[focussed]->curright();
            break;
        case 127:
        case '\b':
        case KEY_BACKSPACE:
            frames[focussed]->untypechr();
            break;
        default:
            frames[focussed]->typechr(ch);
        }
    }
    
    endwin();
}