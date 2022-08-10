// 콘솔게임 제작

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>   //srand를 위함
#include "Console.h"


#define STR_LEN    256
#define BULMAX    5       //화면에 보일 수 있는 최대 총알 개수
#define ENEMYMAX    10       //화면에 보일 수 있는 최대 적 수   CONSOLESIZEX-4보다 작아야 함
#define CONSOLESIZEX    30      //화면 크기
#define CONSOLESIZEY     30      //화면 크기


int _tmain(void){
    //****************변수 선언************
    struct ST_OBJECT{
        int x = 0;     //오브젝트의 X좌표
        int y = 0;     //오브젝트의 Y좌표
        bool state = false;    //오브젝트의 상태
    };

    int score;              //총 점수(생성된 적의 수)
    int enemyCount;         //화면에 보이는 적의 수
    // int enemyY[CONSOLESIZEX] = {0};    //적의 X 좌표에 해당하는 Y좌표
    // bool enemyState[CONSOLESIZEX] = {false};    //적의 상태
    int randX;
    // **************************************

    //***********변수 정의********************
    srand(time(NULL));
    score = 0;
    enemyCount = 0;
    ST_OBJECT enemy[ENEMYMAX];
    ST_OBJECT player;
    player.state = true;
    player.x = 14;
    player.y = 28;
    



    //*****shooting.cpp 프로세스 생성하기 위한 정의*****
    STARTUPINFO si[BULMAX];
    int siCount = 0;
    for(int i=0;i<BULMAX;i++){
        si[i] = {0,};
    }
    PROCESS_INFORMATION pi[BULMAX];	
    DWORD state[BULMAX] = {0,};
    //**************************************

    SetConsoleSize(CONSOLESIZEX, CONSOLESIZEY);     //화면 크기 조정
    SetTitle(_T("avoid star"));           //제목 지정
    SetColor(1,2);        //배경색, 글자색 설정


    while(1){
        PrintPlayer(player.x,player.y);
        //이 함수가 호출되기 이전에 키를 누른 상태였는지와 함수가 호출된 시점인 현재에도 누르고 있는 상태인지를 알 수 있다
        if (GetAsyncKeyState(VK_LEFT) & 0X8000){        //← 키 누를시 왼쪽으로 한칸 이동
            Clear();
            if(2 > --player.x){
                player.x = 2;         //화면크기보다 조금 크게 스폰해야 오류안남
            }
            PrintPlayer(player.x,player.y);
            Sleep(50);
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0X8000){           //→ 키 누를시 오른쪽으로 한칸 이동
            Clear();
            if(++player.x > CONSOLESIZEY-2){
                player.x = CONSOLESIZEY-2;            //화면크기보다 조금 작게 스폰해야 오류안남
            }
            PrintPlayer(player.x,player.y);
            Sleep(50);
        }

        // ****************************적 생성**********************************
        // 화면에 보이는 적 수 >= 화면에 보일 수 있는 최대 적 수 이면 적 생성 중지
        if(enemyCount < ENEMYMAX-1){
            // 적의 x위치 랜덤으로 하나 선택
            randX = (rand() % (CONSOLESIZEX - 4)) + 2;
            //자리가 남는곳이 한곳이라도 있으면 적 하나만 생성하고 빠져나오기
            int i=0;
            while(1){
                if(enemy[i].state == false){
                    enemy[i].state = true;
                    enemy[i].x = randX;
                    enemy[i].y = 0;
                    enemyCount++;
                    score++;
                    break;
                }else{
                    i++;
                }
            }
        }
        //**************************************************************************

        //****************************적의 상태, 위치 변환********************
        Clear();
        for(int i=0; i<ENEMYMAX ; i++){
            if(enemy[i].state == true){
                //적이 밑바닥에 다다를때까지 한칸씩 내려오기
                if(enemy[i].y < CONSOLESIZEY + 1){
                    // 플레이어와 적이 맞닿을시 게임 오버
                    if(player.y <= enemy[i].y && player.x == enemy[i].x){
                        Clear();
                        GotoXY(CONSOLESIZEX/2-5, CONSOLESIZEX/2);
                        _tprintf(_T("Game Over \n"));
                        GotoXY(CONSOLESIZEX/2-5, CONSOLESIZEX/2+1);
                        _tprintf(_T("score : %d \n"), score);
                        system("pause");
                        return 0;
                    }
                    PrintEnemy(enemy[i].x,enemy[i].y);
                    enemy[i].y++;
                }
                else{
                    enemyCount--;
                    enemy[i].state = false;
                    enemy[i].y = 0;
                }
            }
        }
        Sleep(50);
        //*************************************************************************************


        if (GetAsyncKeyState(VK_SPACE) & 0X8000){       //스페이스바 누를시 총쏘기
            si[siCount].cb = sizeof(si[siCount]);     
            TCHAR command[]=_T("shooting.exe");			//총쏘는 프로세스 경로

            TCHAR bx = player.x + '0';
            TCHAR by = player.y + '0';
            TCHAR cmdStringWithOptions[STR_LEN] = {0,};

			//실행할 파일명과 인자 합쳐 준비
			_stprintf(cmdStringWithOptions, _T("%s %c"), command, bx);
			_stprintf(cmdStringWithOptions, _T("%s %c"), cmdStringWithOptions, by);
			
            ZeroMemory(&pi[siCount], sizeof(pi[siCount]));		// 메모리 영역을 0으로 채우는 매크로

            CreateProcess(	NULL,cmdStringWithOptions,NULL,NULL,FALSE,0,NULL,NULL,&si[siCount],&pi[siCount]);   //프로세스 생성
            SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);     //우선순위 높이기

            for(int i=0;i<BULMAX; i++){
                GetExitCodeProcess(pi[siCount].hProcess, &state[i]);     // 임의의 프로세스의 리턴값(상태)을 확인하는 함수
                if(state[i] != STILL_ACTIVE ){ 
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

