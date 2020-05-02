#include <bits/stdc++.h>
#include <ncurses.h>
using namespace std;

class ioFrame{
    public:
    int startx, starty, height, width, cursor, pointer, topline, screenlines;
    string content, screen;
    WINDOW * win;
    /*
    (startx, starty) : start location on sceen
    (height, width)  : dimension of the window
    cursor           : location of cursor on the screen
    pointer          : location of cursor in screen string
    topline          : the character at which the printing of page starts
    screenlines      : how many lines are printed on the screen
    content          : content in the file
    screen           : part of content displayed on the screen
    */

    ioFrame(int strtx, int strty, int h, int w, string s){
        startx = strtx;
        starty = strty;
        height = h;
        width = w;
        content = "";
        pointer = 0;
        screenlines = 1;
        win = newwin(height, width, starty, startx);
        box(win, 0, 0);
        wattron(win, A_BOLD | COLOR_PAIR(2));
        mvwaddstr(win, 0, 2, s.c_str());
        wattroff(win, A_BOLD | COLOR_PAIR(2));
        wrefresh(win);
    }

    void setcursor(){
        int r = 1 + cursor/(width-2);
        int c = 1 + cursor%(width-2);
        wmove(win, r, c);
    }

    void setcursor(int newc){
        cursor = newc;
        int r = 1 + cursor/(width-2);
        int c = 1 + cursor%(width-2);
        wmove(win, r, c);
    }

    pair<int, int> getcursor(){
        int r = 1 + cursor/(width-2);
        int c = 1 + cursor%(width-2);
        return {startx+c, starty+r};
    }

    void focus(){
        pair<int,int> c = getcursor();
        move(c.second, c.first);
    }

    string getPage(){
        int charinline = 0;
        int lines = 0;
        string output = "";
        for(int i = topline; i<content.length(); i++){
            char c = content[i];
            if(c == '\n'){
                lines++;
                charinline = 0;
            }else{
                charinline++;
                if(charinline == width-2){
                    lines++;
                    charinline = 0;
                }
            }
            output = output + c;
            if(lines == height-2) return output;
        }
        return output;
    }

    void typechr(char c){
        
    }
};

class toolbar{
    public:
    toolbar(string tooltip){
        setTooltip(tooltip);
    }
    void setTooltip(string tooltip){
        tooltip = "| "+tooltip;
        int ROW, COL;
        getmaxyx(stdscr, ROW, COL);
        mvaddstr(ROW-1, 1, "TermIDE");
        mvaddstr(ROW-1, 9, tooltip.c_str());
        mvchgat(ROW-1, 0, 9, A_BOLD | A_ITALIC, 1, NULL);
        mvchgat(ROW-1, 9, -1, 0, 1, NULL);
        refresh();
    }
};