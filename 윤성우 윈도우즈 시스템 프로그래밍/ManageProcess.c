/*
	자식 프로세스 생성, 소멸시키기
*/

#include <stdio.h>
#include <tchar.h>
#include <windows.h>



int _tmain(int argc, TCHAR* argv[])
{
	STARTUPINFO si={0,};
	PROCESS_INFORMATION pi;	
	DWORD state;

	//*****************STARTUPINFO 구조체 속성 지정********************
		si.cb = sizeof(si);     //STARTUPINFO 구조체 변수의 크기. 업그레이드 버전과 그 전 버전을 구분하기 위함 (필수)
		//si.dwFlags=STARTF_USEPOSITION|STARTF_USESIZE;	//si 구조체 변수에 설정된 값중 반영하려 하는 멤버들의 정보
		//si.dwX=100;		//화면 좌상단 기준으로 값만큼 떨어진 위치에서 프로세스 출력
		//si.dwY=200;		//화면 좌상단 기준으로 값만큼 떨어진 위치에서 프로세스 출력
		//si.dwXSize=300;		//size 설정
		//si.dwYSize=200;		//size 설정
		//si.lpTitle=_T("I am a boy!");	//콘솔 윈도우의 타이틀 바 제목
	//******************************************************************

	TCHAR command[]=_T("calc.exe");			//@@@ 생성할 프로세스의 소스 or 소스의 경로
	SetCurrentDirectory(_T("C:\\WINDOWS\\system32"));		//@@@ 현 경로를 바꾸는 함수
															//"C:\\WINDOWS\\system32"는 시스템 디렉터리(표준 검색경로)를 뜻함

	while(true)
	{
			ZeroMemory(&pi, sizeof(pi));		//? 메모리 영역을 0으로 채우는 매크로


			/**************************************CreateProcess() 정보 **********************************
			CreateProcess(
				_In_opt_ LPCWSTR lpApplicationName,		//생성할 프로세스명 or 경로, NULL이면 현 디렉터리에서 찾음
				_Inout_opt_ LPWSTR lpCommandLine,			//첫번째 인자가 NULL인 경우 이번 인자를 통해 생성할 프로세스명과 전달할 인자까지 함께 전달가능, 
															  문자열불가, 변수만 가능, 인자는 띄어쓰기 기준으로 main의 argv배열에 나뉘어 저장됨
				_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,		//해당 자식 프로세스 핸들을 상속(다른 자식프로세스에게 공개하는 것)할지 결정지음 (true는 상속 허락)
				_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
				_In_ BOOL bInheritHandles,					// 자식 프로세스에게 부모의 핸들 테이블에 들어있는 핸들정보를 상속해줄 것인지 결정 (true는 상속)
				_In_ DWORD dwCreationFlags,					//디폴트값은 0, (CREATE_NEW_CONSOLE인 경우 새 창 띄우기)
				_In_opt_ LPVOID lpEnvironment,
				_In_opt_ LPCWSTR lpCurrentDirectory,
				_In_ LPSTARTUPINFOW lpStartupInfo,					//STARTUPINFO 구조체 변수의 주소값
				_Out_ LPPROCESS_INFORMATION lpProcessInformation	//LPPROCESS_INFORMATION 구조체 변수의 주소값
				);	

				// LPSECURITY_ATTRIBUTES 정보
				typedef struct _SECURITY_ATTRIBUTES {
				DWORD nLength;				//구조체 크기
				LPVOID lpSecurityDescriptor;		
				BOOL bInheritHandle;			// 해당 자식 프로세스 핸들을 상속(다른 자식프로세스에게 공개하는 것)할지 결정지음(true는 상속 허락)
				}

			**********************************************************************************************/
			
			// @@@ 해당 자식 프로세스 핸들을 상속(다른 자식프로세스에게 공개하는 것)을 허락할 시 코드 삽입
			// SECURITY_ATTRIBUTES sa;
			// sa.nLength = sizeof(sa);
			// sa.lpSecurityDescriptor = NULL;
			// sa.bInheritHandle = TRUE;

			//자식 프로세스 생성
			BOOL isRun = CreateProcess(		
				NULL,  
				command,	
				NULL,
				NULL, 
				TRUE, 
				0,
				NULL, 
				NULL, 
				&si, 	
				&pi		
			);   //CreateProcess


			// 임의의 프로세스가 종료될 때까지 기다리는 블로킹 함수
			//인자 : (커널 오브젝트의 핸들, INFINITE)
			WaitForSingleObject(pi.hProcess, INFINITE);		

			// 복수개의 프로세스가 종료될 때까지 기다리는 블로킹 함수
			// 인자 : (핸들 개수, 핸들을 저장하고 있는 배열의 주소, True(모든 프로세스가 종료될 시) or False(하나라도 종료될 시), INFINITE)
			// HANDLE hProcesses[] = {}
			// WaitForMultipleObjects()

			// 임의의 프로세스의 리턴값(상태)을 확인하는 함수
			//인자 : (자식 커널 오브젝트의 핸들, Exit code를 저장할 변수)
			//리턴값 : 실행중이라면 STILL_ACTIVE(259), 실행 종료라면 종료된 프로세스의 리턴값
			GetExitCodeProcess(pi.hProcess, &state);	


			if(state == STILL_ACTIVE )
			{
				_tprintf(_T("STILL_ACTIVE \n WaitForSingleObject() is not working!"));
			}
			else{ //자식 프로세스가 실행 종료시 부모 프로세스의 handle table에서 자식 프로세스의 핸들 삭제. 
				  //이로써 자식 프로세스 종료시 자식KernelObject도 삭제하도록 함
				_tprintf(_T("state : %d \n\n"), state);
				CloseHandle(pi.hProcess);	//현 프로세스의 handle table에서 해당 프로세스의 핸들을 삭제하는 함수
				CloseHandle(pi.hThread);	//해당 프로세스의 thread 핸들을 삭제하는 함수
			}
			break;
	}
	return 0;
}
