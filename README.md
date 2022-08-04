ServerProgramming
==================

주석에 @@@ 있는 부분만 수정하여 기능 변경, 추가하기


tcpip프로그래밍
--------------------
+ firstClient.c
  + 클라이언트 소스 - 서버와 채팅 주고받기
    + 서버에 연결하는 방법

+ firstServer.c
  + 서버 소스
    + 서버 열고 클라이언트와 연결하는 방법  



윤성우 윈도우즈 시스템 프로그래밍
-------------------------------

+ Calculator2.c
   + 계산기 생성, 소멸시키기
      + 자식 프로세스가 종료될 때까지 기다리는 방법 - WaitForSingleObject() 함수 활용
      + 자식 프로세스의 상태를 확인하는 방법 - GetExitCodeProcess() 함수 활용

+ CommandPrompt
   + 명령 프롬프트 제작
      + undefined reference to `wWinMain@16' 오류가 나는 경우 대처방법
      + 한글 입력을 가능하게 하기 위한 방법
      + 배열에 있는 여러 문자열 붙이는 방법
      + 자식 프로세스 생성시 부모 프로세스에서 보낸 인자까지 같이 가져오는 방법
      + 명령어를 특정 문자열을 기준으로 나눠 배열에 따로 넣는 방법
      + 자식 프로세스 생성하는 방법
      + 문자열의 내에 존재하는 모든 대문자를 소문자로 변경하는 방법

+ MBCS_WBCS2.cpp
   + MBCS와 WBCS(유니코드)를 동시지원하기
      + MBCS와 WBCS(유니코드)를 동시지원하는 방법
      + windows.h에서 제공하는 동시지원 자료형 모음
      + windows.h에서 제공하는 자료형 관련 매크로 모음
      + 문자 셋 변환하는 방법 (MBCS <-> WBCS)

+ MailReceiver.cpp
   + mailslot으로 한 프로세스가 다른 프로세스에게 문자열 받는 프로세스 만들기
     + mailslot을 생성해 한 프로세스가 다른 프로세스에게 문자열 받는 방법 - CreateMailslot(), ReadFile() 함수 활용

+ MailSender.cpp
   + mailslot으로 한/여러 프로세스에게 문자열 보내는 프로세스 만들기 
      + mailslot을 생성해 한/여러 프로세스에게 문자열 보내는 방법 - CreateFile(), WriteFile() 함수 활용

+ Operation2.c
   + 프로세스의 실행 우선순위 변경하기
      + 현재 실행되고 있는 프로세스를 확인하는 방법 - GetCurrentProcess() 함수 활용
      + 프로세스의 우선순위 변경하는 방법 - SetPriorityClass() 함수 활용
