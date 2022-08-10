// 총 쏘는 프로세스

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include "Console.h"
#include "Console.cpp"

#define STR_LEN    256

int _tmain(int agrc, TCHAR *argv[]){
    TCHAR xString[STR_LEN] = {0, };
    TCHAR yString[STR_LEN] = {0, };
    
    _stprintf(xString, _T("%s"), argv[1]);
    _stprintf(yString, _T("%s"), argv[2]);

    int bx = xString[0] - '0';
    int by = yString[0] - '0';

    while(1){
        Clear();
        GotoXY(bx,by);
        printf("↑");
        by--;
        if(by < 0){
            return 0;
        }
    }
}
