#include <stdio.h>
#include <Winsock2.h>
#include <WS2tcpip.h>

// 서버 소스
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
    SOCKET listenSocket;
    SOCKADDR_IN servAddr;   //서버의 ip주소, port번호 저장하는 구조체
    char *ip_address="127.0.0.1";   //서버 자신의 ip주소

    // 1. 윈속 초기화
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        printf("Faild WSAStartup() \n");

    // 2. socket 생성
    listenSocket = socket(
        PF_INET,    //IPv4 버전을 의미  (왜 PF?)
        SOCK_STREAM,    //TCP를 의미 (UDP는 SOCK_DGRAM)
        IPPROTO_TCP     //TCP를 의미 (UDP는 IPPROTO_UDP)
        );
    if(listenSocket == INVALID_SOCKET)  
        printf("Faild socket() \n");

    // 3. 서버 자신의 ip주소, port번호 저장
    memset(&servAddr, 0, sizeof(servAddr));

    servAddr.sin_family = AF_INET;          //IP버전 설정. (AF_INET : IPv4) 
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);   //IP 주소 설정(INADDR_ANY : "이 컴퓨터의 랜카드중 사용가능한 랜카드의 ip주소를 사용하라") (htonl :빅 엔디안으로 데이터 변환)
    servAddr.sin_port = htons(30002);       //port 번호 설정 (htonl : 빅 엔디안으로 데이터 변환)

    
    //      3-1. ip 주소를 binary 형태로 변환
    //*************** 방법1. Mingw32를 사용하지 않는 경우 **************************
        //inet_pton() 사용
    // inet_pton(
    //     AF_INET,        //IP버전 설정. (AF_INET == IPv4)
    //     (LPSTR)ip_address,    //서버 자신의 ip 주소
    //     &servAddr.sin_addr  //ip주소를 저장할 곳
    //     );
    //**********************************************************************

    //******************** 방법2. Mingw32를 사용하는 경우 ****************************     
        //inet_pton() 대신 WSAStringToAddress 사용
    int size_sock_addr_in = sizeof(servAddr);

    WSAStringToAddress(                      //? 생략 가능?
        (LPSTR)ip_address,  //ip 주소
        AF_INET,    //IP버전 설정. (AF_INET == IPv4)
        NULL, 
        (LPSOCKADDR)&servAddr, 
        (LPINT)&size_sock_addr_in
        );
    //*************************************************************************


    
    //socket과 local address를 연결시켜주는 함수
    if(bind(    
        listenSocket,    //바인딩할 socket
        (SOCKADDR*)&servAddr,   //바인딩할 sockaddr의 주소
        sizeof(servAddr)    //sockaddr의 크기
        ) == SOCKET_ERROR)
            printf("Binding Error \n");
   
    if(listen(
        listenSocket,    //접속대기할 socket
        5           //보류중인 연결 대기열의 최대 길이
        ) == SOCKET_ERROR)
        printf("listen Error \n");


    SOCKADDR_IN     clientAddr;
    SOCKET      clientSocket;       //소켓이 아닌 우체통 비슷한건가?
    int         sizeClientAddr = sizeof(clientAddr);
    char sendData[255] = "hello";
    char recvByte;
    char recvData[255];
    int recvCount = 0;

    // 4. 접속이 시도된 클라이언트 연결과 통신 처리
    while(1){
        //      4-1 클라이언트 연결 기다리기
        clientSocket = accept(
            listenSocket,
            (SOCKADDR*)&clientAddr,     //client의 ip주소, port번호가 있음
            &sizeClientAddr);
        if(clientSocket == INVALID_SOCKET)
            printf("Failed accept \n");
        recvCount = 0;

        printf("accept client socket : %d   \n", clientSocket);

        //      4-2. 클라이언트에게 send
        send(
            clientSocket,
            sendData,
            strlen(sendData)+1,
            0
        );

        //      4-3. 접속한 client가 접속을 끊을때까지 recv 대기를 반복
        printf("recv count : ");
        while (recvByte = recv(    // (블로킹 함수임)
        clientSocket,       
        recvData,   //buffer의 크기
        sizeof(recvData),   //buffer의 크기
        0
        ))
        {
            // 받은걸 그대로 보낸다.
            printf("[%d]%d", ++recvCount, recvData[0]);
            send(clientSocket, recvData, recvByte, 0);
        }
                
        //      4-4 접속이 끊긴 처리
        printf("\n");
        closesocket(clientSocket);
        printf("close socket : %d \n", clientSocket);

    }
    
    // 5. 소켓 종료 -> 원속 종료
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}