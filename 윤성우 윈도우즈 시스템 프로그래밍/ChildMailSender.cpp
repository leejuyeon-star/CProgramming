/* 
	부모 프로세스로부터 받은 핸들을 가져와서(파일 이용하여)
	한 프로세스를 부모 프로세스와 같이 조종(시용)하기
	메일 슬롯 활용버전
*/

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int _tmain(int argc, LPTSTR argv[])
{
	HANDLE hMailSlot;		//조종할 핸들
	TCHAR message[50];
    DWORD bytesWritten;

	//******* 상속받은 핸들을 가져오는 코드(파일 이용) *******
	FILE* file=_tfopen(_T("InheritableHandle.txt"), _T("rt"));
	_ftscanf(file, _T("%d"), &hMailSlot);
	fclose(file);

	_tprintf(_T("Inheritable Handle : %d \n"), hMailSlot);
	//****************************************************

	//Receiver에게 데이터 보내기
	while(1){
		_fputts(_T("MY CMD>"), stdout);
		_fgetts(message, sizeof(message)/sizeof(TCHAR), stdin);

		// hMailSlot 핸들 이용해 Receiver에게 데이터 보내기
		if(!WriteFile(hMailSlot, message, _tcslen(message)*sizeof(TCHAR), &bytesWritten, NULL)){
			_fputts(_T("Unable to write!"), stdout);
			getchar();
			CloseHandle(hMailSlot);
			return 1;
		}

		if(!_tcscmp(message, _T("exit"))){
			_fputts(_T("Good Bye!"), stdout);
			break;
		}
	}

	CloseHandle(hMailSlot);
	return 0;
}
