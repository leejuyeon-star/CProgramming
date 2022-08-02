#include <stdio.h>
#include <Winsock2.h>
#include <WS2tcpip.h>

// 클라이언트 소스
// 참고 : https://cafe.naver.com/gamepromaster


//**************GCC(MinGW)로 컴파일시*******************
// pragma는 GCC에서 유효하지 않으므로 무시됨. 또한 MinGW에는 "ws2_32.lib"이 없으므로 문제 발생.
// 따라서 아래 명령어로 컴파일해야 함
//> gcc 컴파일할 파일명 -o 생성될 파일명 -lws2_32
// 참고 : https://stackoverflow.com/questions/34384803/c-undefined-reference-to-wsastartup8
//*****************************************

#pragma comment (lib, "ws2_32.lib")

int main(void){
    WSADATA wsaData;
    SOCKET hSocket;
    SOCKADDR_IN servAddr;
    char *ip_address="127.0.0.1";   //서버의 ip주소

    // 1. 윈속 초기화
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        printf("Faild WSAStartup() \n");

    // 2. socket 생성
    hSocket = socket(
        AF_INET,    //IPv4 버전을 의미
        SOCK_STREAM,    //TCP를 의미 (UDP는 SOCK_DGRAM)
        IPPROTO_TCP     //TCP를 의미 (UDP는 IPPROTO_UDP)
        );
    if(hSocket == INVALID_SOCKET)  
        printf("Faild socket() \n");

    // 3. 서버 연결
    //      3-1. 연결할 서버 주소 셋팅(IP주소, port번호 셋팅)
    memset(&servAddr,0,sizeof(servAddr));
    servAddr.sin_family = AF_INET;


    //***************Mingw32를 사용하지 않는 경우**************************
        //inet_pton() 사용
    // inet_pton(
    //     AF_INET,        //IPv4 버전을 의미
    //     (LPSTR)ip_address,    //ip 주소
    //     &servAddr.sin_addr  //ip주소를 저장할 곳
    //     );
    //**********************************************************************



    //********************Mingw32를 사용하는 경우 ****************************
        //inet_pton() 대신 WSAStringToAddress 사용
    int size_sock_addr_in = sizeof(servAddr);
    WSAStringToAddress(
        (LPSTR)ip_address,  //ip 주소
        AF_INET,    //IPv4 버전을 의미
        NULL, 
        (LPSOCKADDR)&servAddr, 
        (LPINT)&size_sock_addr_in
        );
    //*************************************************************************


    servAddr.sin_port = htons(30002);       //서버의 port 번호 설정 (htons()를 통해 빅엔디안으로 설정)

    //      3-2 서버에 연결 시도
    if(connect(     // (블로킹 함수임)
        hSocket,    //소켓 핸들
        (SOCKADDR*)&servAddr,   //연결할 server주소와 port주소 넣는 구조체의 주소
        sizeof(servAddr)    // sockadder의 구조체 크기
        ) == SOCKET_ERROR)
            printf("Failed connect() \n");

    // 4. 서버로부터 통신 대기
    int recvSize;
    char recvData[255];
    recvSize = recv(    // (블로킹 함수임)
        hSocket,       
        recvData,   //buffer의 크기
        sizeof(recvData),   //buffer의 크기
        0
    );
    if(recvSize == -1)
        printf("recbv() Error \n");
    
    printf("recv %d message : %s \n", recvSize, recvData);
    
    // 5. 소켓 종료 -> 원속 종료
    closesocket(hSocket);
    WSACleanup();

    system("pause");
    return 0;
}
