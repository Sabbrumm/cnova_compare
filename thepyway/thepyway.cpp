// Эту библиотеку написал Иван Саббрумм (Токарев) с ФКТИ с группы 2375
//
// Я сделал её по нескольким важным причинам.
// Первое - самому прописать типы данных для упрощенного понимания этих абстракций
// Второе - упростить работу над лабами. Много муторной работы решает эта библа
// Третье - склонить свой собственный кодстайл к любимому сердцу языку Python.
// Перенести питоновские паттерны в си поможет и понять язык лучше, и писать не мороча голову.
//
// Валерий Евгеньевич, это честно я её написал!!! Могу объяснить каждый класс стракт и функцию написанную тут!

#include "thepyway.h"

#include <iostream>
#include <windows.h>
#include <wincon.h>
using namespace std;

void localize() {
    SetConsoleOutputCP(CP_UTF8);
}

void window_fullscreen(){
    SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
}


void window_hidecursor(){
    CONSOLE_CURSOR_INFO cinf;
    cinf.dwSize = 100;
    cinf.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cinf);
}

void window_settitle(const wchar_t* myTitle){
    SetConsoleTitleW(myTitle);
}

void window_dec(){
    localize();
    window_fullscreen();
    window_hidecursor();
}

void interface_border(int width, int height){
    cout << string(width, '=') << string(height-2,'\n') << string(width, '=') << endl;
}

struct PW_Button{
    unsigned char shortcut;
    string content;
};

class PW_Screen{
    private:
        int width;
        int height;
        int point_counter = 0;
        string* buttons{};
    public:
        PW_Screen(int _width, int _height){
            this->width = _width;
            this->height = _height;
        }

        void run();
        void Input();
        void addButton();
        void Output();
        void addOutputField();
        void addHeader();
        void skip();
        bool Confirmation();
};

void PW_Screen::run() {

}


void SetCur(short X, short Y){
    COORD cursor;
    cursor.X = X;
    cursor.Y = Y;
    SC(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}