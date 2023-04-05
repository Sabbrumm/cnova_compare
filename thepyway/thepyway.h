#ifndef THEPYWAY_THEPYWAY_H
#define THEPYWAY_THEPYWAY_H
#pragma once

#define elif else if
#define SC SetConsoleCursorPosition
#define KEY_ESC 27
#define KEY_CONTROL 224
#define KEY_BACKSPACE 8
#define KEY_ENTER 13
#define KEY_CONTROL_DOWN 80
#define KEY_CONTROL_UP 72

#define KEY_CONTROL_RIGHT 77
#define KEY_CONTROL_LEFT 75
#define KEY_0 48
#define KEY_9 57

#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100

//pyway дает о себе знать...

void localize();

void interface_border(int width, int height);

void window_fullscreen();
void window_fontsize(short fs);
void window_hidecursor();
void window_settitle(const wchar_t* myTitle);
void window_dec();

void SetCur(short X, short Y);

#endif //THEPYWAY_THEPYWAY_H
