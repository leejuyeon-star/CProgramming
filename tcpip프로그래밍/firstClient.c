#include <stdio.h>
#include <Winsock2.h>
#include <WS2tcpip.h>

// 클라이언트 소스 - 서버와 채팅 주고받기
// 참고 : https://cafe.naver.com/gamepromaster


//**************@@@ GCC(MinGW)로 컴파일 문제 발생하는 경우 *******************
// MinGW에는 "ws2_32.lib"이 없으므로 문제 발생.
// 따라서 아래 명령어로 컴파일해야 함
//> gcc 컴파일할 파일명 -o 생성될 파일명 -lws2_32
// 참고 : https://stackoverflow.com/questions/34384803/c-undefined-reference-to-wsastartup8
//***************************************************************************

#pragma comment (lib, "ws2_32.lib")

int main(void){
    WSADATA wsaData;
    SOCKET hSocket;
    SOCKADDR_IN servAddr;

    // 1. 윈속 초기화
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        printf("Faild WSAStartup() \n");
        return 1;
    }

    // 2. socket 생성
    hSocket = socket(
        AF_INET,        //@@@ IP 버전 설정 (IPv4 버전은 AF_INET, IPv6은 AF_INET6)
        SOCK_STREAM,    //@@@ (TCP는 SOCK_STREAM, UDP는 SOCK_DGRAM)
        IPPROTO_TCP     //@@@ (TCP는 IPPROTO_TCP, UDP는 IPPROTO_UDP)
        );
    if(hSocket == INVALID_SOCKET) {
        printf("Faild socket() \n");
        WSACleanup();
    }
       

    // 3. 서버 연결
    //      3-1. 연결할 서버의 IP주소, port번호 지정
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;  //@@@ IP 버전 설정 (IPv4 버전은 AF_INET, IPv6은 AF_INET6)
    char *ip_address="127.0.0.1";   //@@@ 서버의 ip주소 설정
    servAddr.sin_port = htons(30002);       //@@@ 서버의 port 번호 설정 (htons() : 호스트의 엔디안으로 데이터 변환)

    //      3-1. @@@ ip 주소를 binary 형태로 변환 (아래 두가지 방법 중 한가지 선택)
    //***************Mingw32를 사용하지 않는 경우**************************
        //inet_pton() 사용
    // inet_pton(
    //     AF_INET,        //@@@ IP 버전 설정 (IPv4 버전은 AF_INET, IPv6은 AF_INET6)
    //     (LPSTR)ip_address,    //ip 주소
    //     &servAddr.sin_addr  //ip주소를 저장할 곳
    //     );
    //**********************************************************************

    //********************Mingw32를 사용하는 경우 ****************************
        //inet_pton() 대신 WSAStringToAddress 사용
    int size_sock_addr_in = sizeof(servAddr);
    WSAStringToAddress(
        (LPSTR)ip_address,  //ip 주소
        AF_INET,    //@@@ IP 버전 설정 (IPv4 버전은 AF_INET, IPv6은 AF_INET6)
        NULL, 
        (LPSOCKADDR)&servAddr, 
        (LPINT)&size_sock_addr_in
        );
    //*************************************************************************


    //      3-2 서버에 연결 시도
    if(connect(     // (블로킹 함수임)
        hSocket,    // 서버에게 보낼 소켓
        (SOCKADDR*)&servAddr,   //연결할 server의 ip주소와 port주소
        sizeof(servAddr)   
        ) == SOCKET_ERROR){
            perror("The following error occurred");
            printf("Failed connect() \n");
            closesocket(hSocket);
            WSACleanup();
            return 1;
        }



    // 4. 서버와 통신처리
    int recvSize;
    char recvData[255];
    char sendData[255];
    while(1){
        
        //      4-1. 서버로부터 통신 대기, 받기
        recvSize = recv(    // (블로킹 함수임)
            hSocket,       //서버에게 보낼 소켓
            recvData,   //buffer
            sizeof(recvData),   //buffer의 크기
            0
        );
        if(recvSize == SOCKET_ERROR){
            printf("recbv() Error \n");
            break;
        }
        printf("recv message : %s \n", recvData);
        

        //      4-2. 서버에게 send
        fputs("Input String : ", stdout);
        scanf("%s", sendData);
        if(!strcmp(sendData, "exit")){      // exit 입력 시 연결 종료
            break;
        }
        else{
            send(hSocket, sendData, strlen(sendData)+1,0);
        }
    }

    // 5. 소켓 종료 -> 원속 종료
    closesocket(hSocket);
    WSACleanup();
    
    system("pause");    // 실행되자마자 곧바로 꺼지지 않도록 일시정지
    return 0;
}
