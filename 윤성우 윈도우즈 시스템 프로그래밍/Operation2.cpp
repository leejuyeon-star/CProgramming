// 예제 6-2
// 현 프로세스의 우선순위 변경하기


#include <stdio.h>
#include <windows.h>
#include <tchar.h>

int _tmain(int argc, TCHAR* argv[])
{
	// 우선순위 변경하는 함수
	// 인수 : (우선순위를 변경할 리소스의 핸들, 우선순위 정보)
	// GetCurrentProcess() : 현재 실행되고 있는 프로세스를 반환. (고정된 특정값이 반환됨)
	// HIGH_PRIORITY_CLASS : 우선순위를 가장 위로 올림
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);		

	while(1)
	{
		for(DWORD i=0; i<10000; i++)
			for(DWORD i=0; i<10000; i++); //Busy Waiting!!

		_fputts(_T("Operation2.exe \n"), stdout); 
	}

	return 0;
}
