/*
	예제 7-2
	메일슬롯 방식의 IPC
	메일 슬롯으로 한 프로세스에게서 문자열 받는 프로세스 만들기
	MailReceiver.cpp
	
*/

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define SLOT_NAME    _T("\\\\.\\mailslot\\mailbox")		//.은 local 컴퓨터를, * 쓰면 broadcast를 의미. mailbox가 실질적인 mailslot의 이름임

int _tmain(int argc, LPTSTR argv[])
{
	HANDLE hMailSlot;  //mailslot 핸들
	TCHAR messageBox[50];
  	DWORD bytesRead;  // number of bytes read

	/*************함수 정보********************
	CreateMailslotW(
		_In_     LPCWSTR lpName,		//슬롯 이름(주소 지정)
		_In_     DWORD nMaxMessageSize,		//buffer 크기 (0일경우 최대크기로 설정됨)
		_In_     DWORD lReadTimeout,			//ReadFile 함수의 블로킹 여부(MAILSLOT_WAIT_FOREVER 인 경우 블로킹 상태, 0인 경우 블로킹x)
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes			//핸들 상속(다른 자식프로세스에게 공개하는 것) 여부를 결정짓는 구조체의 주소
		);

	// LPSECURITY_ATTRIBUTES 정보
		typedef struct _SECURITY_ATTRIBUTES {
		DWORD nLength;				//구조체 크기
		LPVOID lpSecurityDescriptor;		
		BOOL bInheritHandle;			// 해당 자식 프로세스 핸들을 상속(다른 자식프로세스에게 공개하는 것)할지 결정지음(true는 상속 허락)
	} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;


	********************************************************/


	// @@@ 해당 자식 프로세스 핸들을 상속(다른 자식프로세스에게 공개하는 것)을 허락할 시 아래 코드 삽입
	// SECURITY_ATTRIBUTES sa;
	// sa.nLength = sizeof(sa);
	// sa.lpSecurityDescriptor = NULL;
	// sa.bInheritHandle = TRUE;


	/* mailslot(우체통) 생성 */
	hMailSlot=CreateMailslot(
			SLOT_NAME, 	
			0, 		
			MAILSLOT_WAIT_FOREVER, 	
			NULL
			);

	if(hMailSlot==INVALID_HANDLE_VALUE)
	{
		_fputts(_T("Unable to create mailslot!\n"), stdout);
		return 1;
	}

	/* Message 수신 */
	_fputts(_T("******** Message ********\n"), stdout);
	while(1)
	{
		if(!ReadFile(	//데이터 읽는 함수
				hMailSlot, 	//mailslot의 핸들. 이를 통해 데이터 읽어들임
				messageBox, 	//읽은 데이터를 저장할 버퍼
				sizeof(TCHAR)*50, 	//읽을 데이터의 최대크기
				&bytesRead, 	//실제로 읽어들인 데이터 크기 얻기 위해
				NULL)){
			_fputts(_T("Unable to read!"), stdout);
			CloseHandle(hMailSlot);		//현 프로세스의 handle table에서 해당 프로세스의 핸들을 삭제하는 함수
			return 1;
		}

		if(!_tcsncmp(messageBox, _T("exit"), 4))
		{
			_fputts(_T("Good Bye!"), stdout);
			break;
		}

		messageBox[bytesRead/sizeof(TCHAR)]=0; //NULL 문자 삽입.
		_fputts(messageBox, stdout);
	}

	CloseHandle(hMailSlot);


//*********************************************************
#define SLOT_NAME2 _T("\\\\.\\mailslot\\mailbox2")
	HANDLE hMailSlot2;  //mailslot 핸들
	TCHAR message[50];
    DWORD bytesWritten;  // number of bytes write

	hMailSlot2=CreateFile(		//전달할 파일 마련
		//모두 고정
		SLOT_NAME2, 		//연결할 상대 슬롯 이름
		GENERIC_WRITE, 		//메일 슬롯 용도(읽기,쓰기,읽기/쓰기)
		FILE_SHARE_READ, 
		NULL,
		OPEN_EXISTING, 		//파일의 생성 방식(새로운 파일 생성 or 기존 파일에 접근)
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if(hMailSlot2==INVALID_HANDLE_VALUE)
	{
		_fputts(_T("Unable to create mailslot!!\n"), stdout);
		return 1;
	}

	while(1)
	{
		_fputts(_T("MY CMD>"), stdout);
		_fgetts(message, sizeof(message)/sizeof(TCHAR), stdin);

		if(!WriteFile(		//데이터 보내는 함수
			hMailSlot2, 	//연결할 파일 핸들
			message, 	//전송 데이터 버퍼(전송할 데이터 담겨있음)
			_tcslen(message)*sizeof(TCHAR), 	//전송 크기
			&bytesWritten, 	//전송 결과
			NULL))	
		{
			_fputts(_T("Unable to write!"), stdout);
			CloseHandle(hMailSlot2);		//현 프로세스의 handle table에서 해당 프로세스의 핸들을 삭제하는 함수
			return 1;
		}

		if(!_tcscmp(message, _T("exit")))
		{
			_fputts(_T("Good Bye!"), stdout);
			break;
		}

	}

	return 0;
}
