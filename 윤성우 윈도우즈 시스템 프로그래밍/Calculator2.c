/*
	예제 6-8
	계산기 생성, 소멸시키기

	WaitForSingleObject(), GetExitCodeProcess() 함수 활용방법
*/

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

enum{DIV=1, MUL, ADD, MIN, ELSE, EXIT};

DWORD ShowMenu();
void Divide(double, double);
void Multiple(double, double);
void Add(double, double);
void Min(double, double);

int _tmain(int argc, TCHAR* argv[])
{
	STARTUPINFO si={0,};
	PROCESS_INFORMATION pi;	
	DWORD state;
	si.cb=sizeof(si);

	TCHAR command[]=_T("calc.exe");
	SetCurrentDirectory(_T("C:\\WINDOWS\\system32"));

	DWORD sel;
	double num1, num2;
	while(true)
	{
		sel=ShowMenu();
		if(sel==EXIT)
			return 0;

		if( sel != ELSE)
		{
		    _fputts(_T("Input Num1 Num2: "), stdout);
		    _tscanf(_T("%lf %lf"), &num1, &num2);
		}

		switch(sel)
		{
		case DIV:
			Divide(num1, num2);
			break;
		case MUL:
			Multiple(num1, num2);
			break;
		case ADD:
			Add(num1, num2);
			break;
		case MIN:
			Min(num1, num2);
			break;
		case ELSE:
			ZeroMemory(&pi, sizeof(pi));
			CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

			WaitForSingleObject(pi.hProcess, INFINITE);		//자식 프로세스가 종료될 때까지 기다리는 함수

			//자식 프로세스의 상태를 확인하는 함수
			//인수:(프로세스의 핸들, Exit code(return문에 의해 반환되는 값)(실행중이라면 STILL_ACTIVE를 얻음)를 저장할 변수)
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
	}
	return 0;
}

DWORD ShowMenu()
{
	DWORD sel;

	_fputts(_T("-----Menu----- \n"), stdout);
	_fputts(_T("num 1: Divide \n"), stdout);
	_fputts(_T("num 2: Multiple \n"), stdout);
	_fputts(_T("num 3: Add \n"), stdout);
	_fputts(_T("num 4: Minus \n"), stdout);
	_fputts(_T("num 5: Any other operations. \n"), stdout);
	_fputts(_T("num 6: Exit \n"), stdout);
	_fputts(_T("SELECTION >>"), stdout);
	_tscanf(_T("%d"), &sel);
	
	return sel;
}
void Divide(double a, double b)
{
	_tprintf(_T("%f/%f=%f \n\n"), a, b, a/b);
}
void Multiple(double a, double b)
{
	_tprintf(_T("%f*%f=%f \n\n"), a, b, a*b);
}
void Add(double a, double b)
{
	_tprintf(_T("%f+%f=%f \n\n"), a, b, a+b);
}
void Min(double a, double b)
{
	_tprintf(_T("%f-%f=%f \n\n"), a, b, a-b);
}
