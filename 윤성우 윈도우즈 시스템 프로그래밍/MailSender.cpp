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

//**************************************************

#define SLOT_NAME2 _T("\\\\.\\mailslot\\mailbox2")

	HANDLE hMailSlot2;  //mailslot 핸들
	TCHAR messageBox[50];
  	DWORD bytesRead;  // number of bytes read

	/* mailslot(우체통) 생성 */
	hMailSlot2=CreateMailslot(
			SLOT_NAME2, 	//슬롯 이름
			0, 		//buffer 크기(우체통 크기) (0일경우 최대크기로 설정됨)
			MAILSLOT_WAIT_FOREVER, 	//ReadFile 함수 특성
			NULL	//보안
			);

	if(hMailSlot2==INVALID_HANDLE_VALUE)
	{
		_fputts(_T("Unable to create mailslot!\n"), stdout);
		return 1;
	}

	/* Message 수신 */
	_fputts(_T("******** Message ********\n"), stdout);
	while(1)
	{
		if(!ReadFile(	//데이터 읽는 함수
				hMailSlot2, 	//mailslot의 핸들. 이를 통해 데이터 읽어들임
				messageBox, 	//읽은 데이터를 저장할 버퍼
				sizeof(TCHAR)*50, 	//읽을 데이터의 최대크기
				&bytesRead, 	//실제로 읽어들인 데이터 크기 얻기 위해
				NULL)){
			_fputts(_T("Unable to read!"), stdout);
			CloseHandle(hMailSlot2);		//현 프로세스의 handle table에서 해당 프로세스의 핸들을 삭제하는 함수
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

	CloseHandle(hMailSlot2);

	HANDLE hMailSlot;  //mailslot 핸들
	TCHAR message[50];
    DWORD bytesWritten;  // number of bytes write

	hMailSlot=CreateFile(		//전달할 파일 마련
		//모두 고정
		SLOT_NAME, 		//연결할 상대 슬롯 이름
		GENERIC_WRITE, 		//메일 슬롯 용도(읽기,쓰기,읽기/쓰기)
		FILE_SHARE_READ, 
		NULL,
		OPEN_EXISTING, 		//파일의 생성 방식(새로운 파일 생성 or 기존 파일에 접근)
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if(hMailSlot==INVALID_HANDLE_VALUE)
	{
		_fputts(_T("Unable to create mailslot!\n"), stdout);
		return 1;
	}

	while(1)
	{
		_fputts(_T("MY CMD>"), stdout);
		_fgetts(message, sizeof(message)/sizeof(TCHAR), stdin);

		if(!WriteFile(		//데이터 보내는 함수
			hMailSlot, 	//연결할 파일 핸들
			message, 	//전송 데이터 버퍼(전송할 데이터 담겨있음)
			_tcslen(message)*sizeof(TCHAR), 	//전송 크기
			&bytesWritten, 	//전송 결과
			NULL))	
		{
			_fputts(_T("Unable to write!"), stdout);
			CloseHandle(hMailSlot);		//현 프로세스의 handle table에서 해당 프로세스의 핸들을 삭제하는 함수
			return 1;
		}

		if(!_tcscmp(message, _T("exit")))
		{
			_fputts(_T("Good Bye!"), stdout);
			break;
		}

	}

	CloseHandle(hMailSlot);

	return 0;
}
