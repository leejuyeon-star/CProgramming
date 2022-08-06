/*
	핸들을 상속받은 자식 프로세스에게 파일 이용해 특정 핸들의 정보 보내기 
	메일 슬롯 버전
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
		NULL,				//자식 핸들 테이블 상속 여부
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
	
	// @@@ 해당 자식 프로세스 핸들을 상속(다른 자식프로세스에게 공개하는 것)을 허락할 시 코드 삽입
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;


	hMailSlot=CreateFile(SLOT_NAME,GENERIC_WRITE,FILE_SHARE_READ, 
		&sa,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if(hMailSlot==INVALID_HANDLE_VALUE){
		_fputts(_T("Unable to create mailslot!\n"), stdout);
		return 1;
	}

	//*******자식 프로세스에게 상속되어있는 특정 핸들 정보를 보내는 코드(파일 이용)***********
	_tprintf(_T("Inheritable Handle : %d \n"), hMailSlot);
	FILE* file=_tfopen(_T("InheritableHandle.txt"), _T("wt"));
	_ftprintf(file, _T("%d"), hMailSlot);
	fclose(file);
	//********************************************************************************

	//**********자식 프로세스 생성**********
	STARTUPINFO si={0,};
	PROCESS_INFORMATION pi;	
	si.cb=sizeof(si);

	TCHAR command[]=_T("MailSender2_2.exe");

	CreateProcess(NULL,
			      command,
				  NULL,
				  NULL, 
				  TRUE,  // 자식 프로세스에게 핸들 상속!
				  CREATE_NEW_CONSOLE,
				  NULL, 
				  NULL, 
				  &si, 
				  &pi
	);  //CreateProcess
	//***************************************



	// Receiver에게 데이터 보내기
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
