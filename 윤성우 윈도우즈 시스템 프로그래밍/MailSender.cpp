/*
	예제 7-3
	메일슬롯 방식의 IPC
	메일 슬롯으로 한/여러 프로세스에게 문자열 보내는 프로세스 만들기
*/

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, LPTSTR argv[])
{	
	HANDLE hMailSlot;  //mailslot 핸들
	TCHAR message[50];
	DWORD bytesWritten;

	/**************************************CreateFile() 정보 **********************************
	보낼 파일 마련하는 함수
	hMailSlot=CreateFile(		
		//모두 고정
		SLOT_NAME, 		//연결할 상대 슬롯 이름
		GENERIC_WRITE, 		//메일 슬롯 용도(읽기,쓰기,읽기/쓰기)
		FILE_SHARE_READ, 
		NULL,				//자식 핸들 테이블 상속 여부. 상속시 LPSECURITY_ATTRIBUTES 구조체의 주소 넣기
		OPEN_EXISTING, 		//파일의 생성 방식(새로운 파일 생성 or 기존 파일에 접근)
		FILE_ATTRIBUTE_NORMAL, 
		NULL
		);

	// LPSECURITY_ATTRIBUTES 정보
	typedef struct _SECURITY_ATTRIBUTES {
		DWORD nLength;				//구조체 크기
		LPVOID lpSecurityDescriptor;		
		BOOL bInheritHandle;			// 해당 자식 프로세스 핸들을 상속(다른 자식프로세스에게 공개하는 것)할지 결정지음(true는 상속 허락)
	}
	**********************************************************************************************/

	/***@@@ 해당 자식 프로세스 핸들을 상속(다른 자식프로세스에게 공개하는 것)을 허락할 시 코드 적용****
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	*****************************************************************************************/


	hMailSlot=CreateFile(SLOT_NAME,GENERIC_WRITE,FILE_SHARE_READ, 
		,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if(hMailSlot==INVALID_HANDLE_VALUE){
		_fputts(_T("Unable to create mailslot!\n"), stdout);
		return 1;
	}


	while(1){
		_fputts(_T("MY CMD>"), stdout);
		//보낼 데이터 입력받기
		_fgetts(message, sizeof(message)/sizeof(TCHAR), stdin);

		/***************WriteFile() 정보*********************************
			데이터 담아 보내는 함수
			if(!WriteFile(		
			hMailSlot, 	//보낼 파일 핸들
			message, 	//전송 데이터 버퍼(전송할 데이터 담겨있음)
			_tcslen(message)*sizeof(TCHAR), 	//전송 크기
			&bytesWritten, 	//전송 결과
			NULL))	
			{
			_fputts(_T("Unable to write!"), stdout);
			CloseHandle(hMailSlot);		//현 프로세스의 handle table에서 해당 프로세스의 핸들을 삭제하는 함수
			return 1;
			}
		***********************************************************/
		if(!WriteFile(hMailSlot, message,_tcslen(message)*sizeof(TCHAR),&bytesWritten,NULL)){
			_fputts(_T("Unable to write!"), stdout);
			CloseHandle(hMailSlot);		//현 프로세스의 handle table에서 해당 프로세스의 핸들을 삭제하는 함수
			return 1;
		}

		//exit 보낼시 종료
		if(!_tcscmp(message, _T("exit"))){		
			_fputts(_T("Good Bye!"), stdout);
			break;
		}

	}

	CloseHandle(hMailSlot);
	return 0;
}
