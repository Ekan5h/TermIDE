#include <bits/stdc++.h>
#include <ncurses.h>
using namespace std;

class ioFrame{
    public:
    int startx, starty, height, width, cursor, pointer, topline;
    string content;
    WINDOW * win;
    /*
    (startx, starty) : start location on sceen
    (height, width)  : dimension of the window
    cursor           : location of cursor on the screen
    pointer          : location of cursor in screen string
    topline          : the character at which the printing of page starts
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
        win = newwin(height, width, starty, startx);
        box(win, 0, 0);
        wattron(win, A_BOLD | COLOR_PAIR(2));
        mvwaddstr(win, 0, 2, s.c_str());
        wattroff(win, A_BOLD | COLOR_PAIR(2));
        setcursor();
        wrefresh(win);
    }

    void setcursor(){
        int r = 1 + cursor/(width-2);
        int c = 1 + cursor%(width-2);
        wmove(win, r, c);
    }

    pair<int, int> getcursor(){
        int r = 1 + cursor/(width-2);
        int c = 1 + cursor%(width-2);
        wmove(win, r, c);
        return {startx+c, starty+r};
    }

    void focus(){
        pair<int,int> c = getcursor();
        move(c.second, c.first);
    }

    void scrollIntoView(){
        if(cursor>=(height-2)*(width-2)){
            int temp = topline;
            while(content[topline]!='\n' && topline-temp != width-2) topline++;
            if(content[topline]=='\n' && topline-temp != width-2) topline++;
            setPage();
            cursor-=width-2;
            focus();
        }else if(cursor<0){
            int temp = --topline;
            topline--;
            while(topline >=0 && content[topline]!='\n') topline--;
            topline++;
            topline += ((temp-topline)/(width-2))*(width-2);
            setPage();
            cursor+=width-2;
            focus();
        }
    }

    void setPage(){
        int temp = cursor;
        cursor = 0;
        focus();
        cleartillend();
        putstr(content.substr(topline, content.length()));
        cursor = temp;
        focus();
    }

    void cleartillend(){
        int temp = cursor;
        while(cursor < (height-2)*(width-2)){
            waddch(win, ' ');
            cursor++;
            setcursor();
        }
        cursor = temp;
        focus();
        wrefresh(win);
    }

    void typechr(char c){
        if(c=='\r') c = '\n';
        content = content.substr(0,pointer) + c + content.substr(pointer,content.length());
        cleartillend();
        putstr(content.substr(pointer,content.length()));
        pointer++;
        if(c != '\n')
            cursor++;
        else{
            cursor+= width-2 - cursor%(width-2);
        }
        scrollIntoView();
        focus();
    }

    void untypechr(){
        if(pointer==0) return ;
        char delchar = content[pointer-1];
        content = content.substr(0,pointer-1)+content.substr(pointer,content.length());
        pointer--;
        int temp = pointer;
        if(delchar == '\n'){
            cursor -= width-2 + cursor%(width-2);
            while(pointer>=0 && content[--pointer]!='\n');
            int prev = pointer;
            while(++pointer!=temp){
                if((pointer - prev)%(width-2)==0){
                    cursor -= width-2;
                }
                cursor++;
            }
        }else
            cursor--;
        setcursor();
        scrollIntoView();
        cleartillend();
        putstr(content.substr(pointer,content.length()));
        focus();
    }

    void deleteChar(){
        if(pointer < content.length()){
            curright();
            untypechr();
        }
    }

    void putstr(string s){
        int temp = cursor;
        for(char c:s){
            if(c!='\n'){
                waddch(win, c);
                cursor++;
            }else{
                cursor+= width-2 - cursor%(width-2);
            }
            if(cursor >= (height-2)*(width-2)) break;
            setcursor();
        }
        cursor = temp;
        focus();
        wrefresh(win);
    }

    void curdown(){
        int temp = pointer, pos = cursor%(width-2);
        while(pointer<content.length() && content[pointer] != '\n' && pointer-temp+pos < width-2) pointer++;
        if(content[pointer]=='\n' || pointer-temp+pos == width-2){
            // Next Line available
            if(content[pointer]=='\n' && pointer-temp+pos != width-2) pointer++;
            cursor += width-2 - cursor%(width-2);
            temp = pointer;
            while(pointer<content.length() && pointer-temp<pos && content[pointer]!='\n'){
                pointer++;
                cursor++;
            }
            focus();
        }else{
            // Next line not available
            pointer = temp;
        }
        scrollIntoView();
    }

    void curup(){
        int temp = pointer, pos = cursor%(width-2);
        while(cursor%(width-2)!=0){
            cursor-=1;
            pointer-=1;
        }
        if(pointer!=0){
            // Previous line available
            pointer-=2;
            cursor -= width-2;
            int len = 0;
            while(pointer>=0 && content[pointer]!='\n'){
                pointer--;
                len++;
            }
            pointer++;
            pointer += len/(width-2)*(width-2);
            temp = pointer;
            while(pointer-temp<pos && content[pointer]!='\n'){
                pointer++;
                cursor++;
            }
            focus();
        }else{
            // Previous line not available
            cursor += pos;
            pointer = temp;
        }
        scrollIntoView();
    }

    void curleft(){
        if(pointer == 0) return ;
        pointer-=1;
        if(content[pointer] == '\n'){
            cursor -= width-2 + cursor%(width-2);
            while(pointer>=0 && content[--pointer]!='\n');
            int temp = pointer;
            while(content[++pointer]!='\n'){
                if((pointer-temp)%(width-2)==0 && (pointer-temp)/(width-2)>0){
                    cursor -= width-2;
                }
                cursor++;
            }
        }else{
            cursor-=1;
        }
        focus();
        scrollIntoView();
    }

    void curright(){
        if(pointer == content.length()) return ;
        if(content[pointer] == '\n'){
            cursor += width-2 - cursor%(width-2);
        }else{
            cursor+=1;
        }
        pointer+=1;
        focus();
        scrollIntoView();
    }

    void nextWord(){
        curright();
        while(pointer<content.length() && content[pointer]!='\n' && content[pointer]!=' '){
            pointer++;
            cursor++;
        }
        focus();
        scrollIntoView();
    }

    void prevWord(){
        curleft();
        if(content[pointer]=='\n') return ;
        while(pointer>0 && content[pointer]!='\n' && content[pointer]!=' '){
            pointer--;
            cursor--;
        }
        if(content[pointer]=='\n'){
            pointer++;
            cursor++;
        }
        focus();
        scrollIntoView();
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