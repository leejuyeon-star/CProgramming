// 콘솔게임 제작

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include "Console.h"


#define STR_LEN    256
#define BULMAX    5       //화면에 보일 수 있는 최대 총알 개수
#define CONSOLESIZEX    30      //화면 크기
#define CONSOLESIZEY     30      //화면 크기


int _tmain(void){
    DWORD playerPosX = 14, playerPosY = 28;       //플레이어의 처음 스폰장소
    STARTUPINFO si[BULMAX];
    int siCount = 0;
    for(int i=0;i<BULMAX;i++){
        si[i] = {0,};
    }
    PROCESS_INFORMATION pi[BULMAX];	
    DWORD state[BULMAX] = {0,};
    SetConsoleSize(CONSOLESIZEX, CONSOLESIZEY);     //화면 크기 조정

    while(1){
        // Clear();
        PrintPlayer(playerPosX,playerPosY);
        //이 함수가 호출되기 이전에 키를 누른 상태였는지와 함수가 호출된 시점인 현재에도 누르고 있는 상태인지를 알 수 있다
        if (GetAsyncKeyState(VK_LEFT) & 0X8000){        //← 키 누를시 왼쪽으로 한칸 이동
            Clear();
            if(2 > --playerPosX){
                playerPosX = 2;         //화면크기보다 조금 크게 스폰해야 오류안남
            }
            PrintPlayer(playerPosX,playerPosY);
            Sleep(50);
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0X8000){           //→ 키 누를시 오른쪽으로 한칸 이동
            Clear();
            if(++playerPosX > CONSOLESIZEY-2){
                playerPosX = CONSOLESIZEY-2;            //화면크기보다 조금 작게 스폰해야 오류안남
            }
            PrintPlayer(playerPosX,playerPosY);
            Sleep(50);
        }
        if (GetAsyncKeyState(VK_SPACE) & 0X8000){       //스페이스바 누를시 총쏘기
            si[siCount].cb = sizeof(si[siCount]);     
            TCHAR command[]=_T("shooting.exe");			//총쏘는 프로세스 경로

            TCHAR bx = playerPosX + '0';
            TCHAR by = playerPosY + '0';
            TCHAR cmdStringWithOptions[STR_LEN] = {0,};

			//실행할 파일명과 인자 합쳐 준비
			_stprintf(cmdStringWithOptions, _T("%s %c"), command, bx);
			_stprintf(cmdStringWithOptions, _T("%s %c"), cmdStringWithOptions, by);
			
            ZeroMemory(&pi[siCount], sizeof(pi[siCount]));		// 메모리 영역을 0으로 채우는 매크로

            CreateProcess(	NULL,cmdStringWithOptions,NULL,NULL,FALSE,0,NULL,NULL,&si[siCount],&pi[siCount]);   //프로세스 생성
            SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);     //우선순위 높이기

            for(int i=0;i<BULMAX; i++){
           
                GetExitCodeProcess(pi[siCount].hProcess, &state[i]);     // 임의의 프로세스의 리턴값(상태)을 확인하는 함수

                if(state[i] != STILL_ACTIVE )
                { 
                    CloseHandle(pi[siCount].hProcess);	//현 프로세스의 handle table에서 해당 프로세스의 핸들을 삭제하는 함수
                    CloseHandle(pi[siCount].hThread);	//해당 프로세스의 thread 핸들을 삭제하는 함수
                }
            }

            if(++siCount>BULMAX-1){
                siCount=0; 
            }
        }
    }
    system("pause");
    return 0;
}

