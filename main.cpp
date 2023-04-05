#include <iostream>
#include <windows.h>
#include <fstream>
#include "thepyway/thepyway.h"
#include <algorithm>
#include "LCS_algo.h"
#include <list>
#include <conio.h>

int unicode_len(std::string s){
    return (s.length() - count_if(s.begin(), s.end(), [](char c)->bool { return (c & 0xC0) == 0x80; }));
}
void ConsoleColorDefault(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
}
void ConsoleColorPurple(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN|
                                                             BACKGROUND_BLUE | BACKGROUND_RED);
}

void ConsoleColorCyan(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN|
                                                             BACKGROUND_BLUE | BACKGROUND_GREEN);
}

void ConsoleColorRed(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN|
                                                             BACKGROUND_RED);
}

const int borderwidth = 107;
const int borderinner = 105;
const int halfborder = 52;
const int fillingroom = 50;

void greetings(){
    std::cout<<"===========================================================================================================\n"
               "=  CNOVA Compare v1.0a.                                      Внимание! Если в строке больше 50 символов,  =\n"
               "=  Условные обозначения:                                     она не будет выведена полностью!             =\n";
    std::cout<<"=  "; ConsoleColorCyan(); std::cout<<"|"; ConsoleColorDefault();
    std::cout<<    " Текущая строка                                                                                       =\n";
    //std::cout<<"=  "; ConsoleColorPurple(); std::cout<<"/"; ConsoleColorDefault();
    //std::cout<<    " Перенос строки                                                                                       =\n";
    std::cout<<"=    "; ConsoleColorRed(); std::cout<<"Различие в тексте"; ConsoleColorDefault();
    std::cout<<                      "                                                                                    =\n";
    //std::cout<<"=    "; ConsoleColorRed(); std::cout<<"<...>"; ConsoleColorDefault();
    //std::cout<<          " Пустая строка                                                                                  =\n";
    std::cout<<"===========================================================================================================\n";
}
void print_curstring(){ConsoleColorCyan(); std::cout<<"|"; ConsoleColorDefault();std::cout<<" ";}
void print_transstring(){ConsoleColorPurple(); std::cout<<"/"; ConsoleColorDefault();std::cout<<" ";}
void print_emptystring(){ConsoleColorRed(); std::cout<<"<...>"; ConsoleColorDefault();}

enum SkipReason{EmptyString, TransString};

struct skipper_single{
    int line = 0;
    SkipReason skipped = EmptyString;
    skipper_single *next = nullptr;
};

struct skipper{
    skipper_single *first = nullptr;
    void clear(){
        while (first){
            skipper_single *t = first->next;
            delete first;
            first = t;
        }
    }
    void append(int line, SkipReason skipped){
        skipper_single *s = new skipper_single;
        s->line=line;
        s->skipped=skipped;
        s->next = first;
        first = s;
        }
    int inside(int line){
        skipper_single *t = first;
        while (t){
            if (t->line == line){
                return 1;
            }
            t=t->next;
        }
        return 0;
    }
    void removeline(int line){
        skipper_single *t = first;
        skipper_single *h = t;
        while (t){

            if (t->line == line){
                h->next=t->next;
                if (t==first){
                    first = nullptr;
                    delete t;
                }
                delete t;
                return;
            }
            h = t;
            t=t->next;
        }
    }
};

string no_any_nuans(string st){
    int cou = 0;
    string tst;
    for (int i = 0; i<st.length(); i++){
        if (st[i]=='\n'){
            tst+=st[i];
            cou=0;
        }
        else{
            if (cou<50){
                tst+=st[i];
            }
            cou++;
        }
    }
    return tst;
}

skipper* get_skipper(string src, string cmp){
    skipper *temp = new skipper;
    int topborder = 8;
    int j = 0;
    int t_c = 0; //transition counter

    for (int i = 0; i<src.length(); i++){
        if (src[i]!=cmp[j]){
            if (src[i]=='\n'){

                temp->append(topborder, EmptyString);
                t_c=0;
                topborder++;

            }
        }
        else {
            t_c++;
            if (t_c>50){
                //temp->append(topborder, TransString);
                //t_c=0;
                //topborder++;

            }
            if (src[i]=='\n'){
                t_c=0;
                topborder++;
            }
            j++;
        }
    }
    return temp;

}

skipper* filter_skippers(skipper *sk1, skipper *sk2){
    skipper *sk1copy = new skipper;
    skipper_single *t = sk1->first;
    while (t){
        sk1copy->append(t->line, t->skipped);
        t=t->next;
    }
    t=sk1->first;
    while (t){
        if (sk2->inside(t->line) and t->skipped==TransString){
            sk2->removeline(t->line);
            sk1copy->removeline(t->line);
        }
        t=t->next;
    }
    sk1->clear();
    delete sk1;
    return sk1copy;

}

int print_highlight(string src, string cmp, int leftb, int rightb, int topb, skipper *skip){
    int j = 0;
    int linecou = 1;
    int t_c = 0; //transition counter
    int need_string_label = 1;
    SetCur(leftb, topb);

    for (int i = 0; i<src.length(); i++){
        while(skip->inside(topb)){
            topb++;
            SetCur(leftb, topb);
            linecou++;
        }
        if (need_string_label){
            SetCur(leftb, topb);
            print_curstring();
            need_string_label = 0;
        }

        if (src[i]==cmp[j]){
            if (t_c<=50){
                if (src[i]!='\n'){
                    cout<<src[i];
                    t_c++;
                }
                else{
                    t_c = 0;
                    topb++;
                    linecou++;
                    //ConsoleColorPurple();cout<<"\\n";ConsoleColorDefault();
                    //if (!skip->inside(topb+1)){
                        need_string_label=1;
                    //}

                }
            }
            else{
                //t_c = 0;
                //topb++;
                //SetCur(leftb, topb);
                //print_transstring();

                //cout<<src[i];
                //t_c++;
            }
            j++;
        }
        else {
            ConsoleColorRed();
            if (t_c<=50){
                if (src[i]!='\n'){
                    cout<<src[i];
                    t_c++;
                }
                else{
                    t_c = 0;
                    topb++;
                    linecou++;
                    //ConsoleColorPurple();cout<<"\\n";
                        need_string_label=1;

                    //doactionforhandling
                }
            }
            else{
                //t_c = 0;
                //topb++;
                //linecou++;
                //SetCur(leftb, topb);
                //print_transstring();

                //cout<<src[i];
                //t_c++;
            }
            ConsoleColorDefault();
        }
    }
    return linecou;
}


std::string parsefilename(std::string path){
    int bs = path.find_last_of('\\');
    int s = path.find_last_of('/');
    if (bs!=-1 or s!=-1){
        std::string tstr = "";
        for (int i = std::max(bs, s)+1; i<path.length(); i++){
            tstr += path[i];
        }
        return tstr;
    }
    else{
        return path;
    }

    return "";
}

void filenames_greeting(std::string filename1, std::string filename2){
    std::cout<<"=";
    int len1 = unicode_len(filename1);
    int len2 = unicode_len(filename2);
    SetCur((halfborder-len1)/2,6); std::cout<<filename1;
    SetCur(53,6); std::cout<<"=";
    SetCur((halfborder+(halfborder-len2)/2),6); std::cout<<filename2;
    SetCur(106,6); std::cout<<"=\n";
    std::cout<<"===========================================================================================================\n";
}

int checkexceptions(std::fstream& file, char* filename){
    try{
        file.open(filename);
        if (file.bad() | file.fail() | !file.is_open()){
            std::cout<<"Ошибка при открытии файла "<<filename<<std::endl;
            return 0;
        }
        file.close();
    }
    catch(const std::ifstream::failure& e) {
        std::cout<<"Ошибка №"<<e.code()<<": "<< e.what()<<std::endl;
        return 0;
    }
    return 1;
}
void printtextborder(int sf, int hght) {
    int i = 0;
    for (; i<hght; i++){
        SetCur(0, sf+i);
        cout<<"=";
        SetCur(53, sf+i);
        cout<<"=";
        SetCur(106, sf+i);
        cout<<"=";
    }
    SetCur(0,sf+i);
    std::cout<<"===========================================================================================================\n";

}
void printtexts(string string1, string string2, string stringdif){
    int cur_y = 8;
    int f1_tr = 0;
    int f2_tr = 0;
    int j = 0;
    int t1 = 0;
    int t2 = 0;
    skipper *sk1 = get_skipper(string2, stringdif);
    skipper *sk2 = get_skipper(string1, stringdif);
    sk1 = filter_skippers(sk1, sk2);

    print_highlight(string1, stringdif, 1, 53, 8, sk1);

    printtextborder(8, print_highlight(string2, stringdif, 54, 106, 8, sk2));

//    SetCur(0, cur_y); cout<<'='; print_curstring();

//    for (int i = 0; i<string1.length(); i++){
//        if (string1[i]==stringdif[j]){
//            j++;
//            f1_tr++;
//            if (f1_tr>=fillingroom){
//                cur_y+=1;
//                SetCur(0, cur_y);
//                cout<<"="; print_transstring();
//                f1_tr = 0;
//            }
//            if (string1[i]=='\n'){
//                cur_y+=1;
//                SetCur(0, cur_y);
//                cout<<"="; print_curstring();
//                f1_tr = 0;
//            }
//            else{
//                cout<<string1[i];
//            }
//            continue;
//
//        }
//        else{
//            f1_tr++;
//            if (f1_tr>=fillingroom){
//                cur_y+=1;
//                SetCur(0, cur_y);
//                cout<<"="; print_transstring();
//                f1_tr = 0;
//            }
//            if (i!=string1.length()-1 and string1[i]=='\n' and string1[i+1]=='\n'){
//                print_emptystring();
//                cur_y+=1;
//                SetCur(0, cur_y);
//                f1_tr = 0;
//                cout<<"="; print_curstring();
//            }
//            else if ()
//            else{
//                ConsoleColorRed(); cout<<string1[i]; ConsoleColorDefault();
//            }
//
//            continue;
//        }
//    }
//    j=0;
//    cur_y=10;
//    SetCur(53,cur_y); cout<<'='; print_curstring();
//    for (int i = 0; i<string2.length(); i++){
//        if (string2[i]==stringdif[j]){
//            j++;
//            f2_tr++;
//            if (f2_tr>=fillingroom){
//                SetCur(106, cur_y);
//                cout<<"=";
//                cur_y+=1;
//                SetCur(53, cur_y);
//                cout<<"="; print_transstring();
//                f2_tr = 0;
//                t1++;
//            }
//            if (string2[i]=='\n'){
//                SetCur(106, cur_y);
//                cout<<"=";
//                cur_y+=1;
//                SetCur(53, cur_y);
//                cout<<"="; print_curstring();
//                f2_tr = 0;
//            }
//            else{
//                cout<<string2[i];
//            }
//            if (i==string2.length()-1){
//                SetCur(106, cur_y);
//                cout<<"=";
//            }
//            continue;
//
//        }
//        else{
//            f2_tr++;
//            if (f2_tr>=fillingroom){
//                SetCur(106, cur_y);
//                cout<<"=";
//                cur_y+=1;
//                SetCur(53, cur_y);
//                cout<<"="; print_transstring();
//                f2_tr = 0;
//                t2++;
//            }
//            if (i!=string2.length()-1 and string2[i]=='\n' and string2[i+1]=='\n'){
//                print_emptystring();
//                SetCur(106, cur_y);
//                cout<<"=";
//                cur_y+=1;
//                SetCur(53, cur_y);
//                f2_tr = 0;
//                cout<<"="; print_curstring();
//            }
//
//            else{
//                ConsoleColorRed(); cout<<string2[i]; ConsoleColorDefault();
//            }
//            if (i==string2.length()-1){
//                SetCur(106, cur_y);
//                cout<<"=";
//            }
//            continue;
//        }
//    }
//    int maxh = max(
//            std::count(string1.begin(), string1.end(),'\n')+t1,
//            std::count(string2.begin(), string2.end(),'\n'+t2)
//    )+1;
//    SetCur(0, 10+maxh);
//    std::cout<<"===========================================================================================================\n";

}



int main(int argc, char* argv[]) {
    std::fstream file1;
    std::fstream file2;
    system("cls");
    SetConsoleOutputCP(CP_UTF8);
    window_hidecursor();


    if (argc==1){
        std::cout<<"Вы не передали никаких аргументов!\n"
                   "Вызовите функцию как cnova_compare.exe <Файл 1> <Файл 2>\n";
        //system("pause");
    }
    else if (argc==2){
        std::cout<<"Вы передали недостаточно аргументов!\n"
                   "Вызовите функцию как cnova_compare.exe <Файл 1> <Файл 2>\n";
    }
    else if(argc==3){
        if (!checkexceptions(file1, argv[1])){
            return 0;
        }
        if (!checkexceptions(file2, argv[2])){
            return 0;
        }
        //greetings();
        filenames_greeting(parsefilename(argv[1]), parsefilename(argv[2]));

        //Получили строки из файла
        string fstring1 = no_any_nuans(FileToString(argv[1]));
        string fstring2 = no_any_nuans(FileToString(argv[2]));

        int **LCSmatrix = LCS_Matrix(fstring1, fstring2); //Получили матрицу для алгоритма
        string result = LCS_String(fstring1, fstring2, LCSmatrix); //Получили строку LCS
        printtexts(fstring1, fstring2, result);
        getch();
    }
    else{
        std::cout<<"Вы передали слишком много аргументов!\n"
                   "Вызовите функцию как cnova_compare.exe <Файл 1> <Файл 2>\n";

    }
    return 0;
}
