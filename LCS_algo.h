//
// Created by Sabbrumm on 15.03.2023.
//

#ifndef CNOVA_COMPARE_LCS_ALGO_H
#define CNOVA_COMPARE_LCS_ALGO_H
#include <fstream>
using namespace std;
int GetFileLen(std::string filename) {
    fstream file;
    file.open(filename);
    int len=0;
    char el;
    while (file.get(el)) {
        ++len;
    }
    file.close();
    return len;
}

int GetFileHeight(std::string filename) {
    fstream file;
    file.open(filename);
    int height = 0;
    char el;
    while (file.get(el)) {
        if (el == '\n') {
            ++height;
        }
    }
    file.close();
    return height;
}

string FileToString(std::string filename) {
    fstream file;
    file.open(filename);
    string temp = "";
    char el;
    while (file.get(el)) {
        temp+= el;
    }
    file.close();
    return temp;
}

int** LCS_Matrix(string string1, string string2) {
    //height - с1
    //width - с2
    int m_height = string1.length()+1;
    int m_width = string2.length()+1;

    // the matrix consists of amounts of elements, which are common
    // in both the first and the second string

    int** LCS_Matrix = new int*[m_height];    //
    // initializing an array dynamically
    for (int i = 0; i < m_height; ++i) {      //
        LCS_Matrix[i] = new int[m_width];     //
    }

    for (int i = 0; i < m_height; ++i) {     // filling the array with zero's
        for (int j = 0; j < m_width; ++j) {
            LCS_Matrix[i][j] = 0;
        }
    }

    for (int i = 1; i <= string1.length(); ++i) {    //filling the array with numbers according to the LCS algorithm
        for (int j = 1; j <= string2.length(); ++j) {
            if (string1[i - 1] == string2[j - 1]) {
                LCS_Matrix[i][j] = LCS_Matrix[i - 1][j - 1] + 1;
            } else {
                if (LCS_Matrix[i - 1][j] > LCS_Matrix[i][j - 1]) {
                    LCS_Matrix[i][j] = LCS_Matrix[i - 1][j];
                } else {
                    LCS_Matrix[i][j] = LCS_Matrix[i][j - 1];
                }
            }
        }
    }

    return LCS_Matrix;
}

string LCS_String(string string1, string string2, int** matrix) {
    string LCS_res = "";
    int i = string1.length(), j=string2.length();
    int LCS_size = matrix[i][j];
    while (i > 0 && j > 0) {
        if (string1[i - 1] == string2[j - 1]) {
            LCS_res = string1[i - 1] + LCS_res;
            i--;
            j--;
        } else {
            if (matrix[i - 1][j] > matrix[i][j - 1]) {
                i--;
            } else {
                j--;
            }
        }
    }

    return LCS_res;
}



#endif //CNOVA_COMPARE_LCS_ALGO_H
