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
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        printf("Faild WSAStartup() \n");
        return 1;
    }

    // 2. socket 생성
    listenSocket = socket(
        PF_INET,    //IPv4 버전을 의미 
        SOCK_STREAM,    //TCP를 의미 (UDP는 SOCK_DGRAM)
        IPPROTO_TCP     //TCP를 의미 (UDP는 IPPROTO_UDP)
        );
    if(listenSocket == INVALID_SOCKET){
        WSACleanup();
        printf("Faild socket() \n");
        return 1;
    }

    // 3. 서버 자신의 ip주소, port번호 지정
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;          //IP버전 설정. (AF_INET : IPv4) 
    servAddr.sin_port = htons(30002);       //port 번호 설정 (htons() : 호스트의 엔디안으로 데이터 변환)

    
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
    // int size_sock_addr_in = sizeof(servAddr);

    // WSAStringToAddress(                      //? 서버는 왜 생략 가능?
    //     (LPSTR)ip_address,  //ip 주소
    //     AF_INET,    //IP버전 설정. (AF_INET == IPv4)
    //     NULL, 
    //     (LPSOCKADDR)&servAddr, 
    //     (LPINT)&size_sock_addr_in
    //     );
    //*************************************************************************


    
    //socket과 local address를 연결시켜주는 함수
    if(bind(    
        listenSocket,    //바인딩할 socket
        (SOCKADDR*)&servAddr,   //바인딩할 sockaddr의 주소
        sizeof(servAddr)    //sockaddr의 크기
        ) == SOCKET_ERROR){
            printf("Binding Error \n");
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }
   
    if(listen(
        listenSocket,    //접속대기할 socket
        5           //보류중인 연결 대기열의 최대 길이
        ) == SOCKET_ERROR){
            printf("listen Error \n");
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

    printf("server open\n");

    SOCKADDR_IN     clientAddr;
    SOCKET      clientSocket;       
    int         sizeClientAddr = sizeof(clientAddr);
    char sendData[255] = "hi Client";
    char recvByte;
    char recvData[255];
    int recvCount = 0;


    // 4. 접속이 시도된 클라이언트의 통신 처리
    //한 클라이언트와 통신이 끊겨면 다른 클라이언트의 연결을 기다리기 위해 무한반복
    while(1){
        //      4-1 클라이언트 연결 기다리기
        clientSocket = accept(
            listenSocket,      //클라이언트에게서 받는 역할을 할 소켓 
            (SOCKADDR*)&clientAddr,     //client의 ip주소, port번호가 있음
            &sizeClientAddr);
        if(clientSocket == SOCKET_ERROR)
            printf("Failed accept \n");
        recvCount = 0;

        printf("accept client socket : %d   \n", clientSocket);
        printf("client IP, port : %d.%d.%d.%d : %d \n", clientAddr.sin_addr.S_un.S_un_b.s_b1,
            clientAddr.sin_addr.S_un.S_un_b.s_b2,
            clientAddr.sin_addr.S_un.S_un_b.s_b3,
            clientAddr.sin_addr.S_un.S_un_b.s_b4,
            clientAddr.sin_port);

        //      4-2. 클라이언트에게 send
        send(
            clientSocket,       //클라이언트가 접속 시도시 보낸 소켓의 정보
            sendData,           //보낼 데이터
            strlen(sendData)+1,
            0
        );

        //      4-3. 접속한 client가 접속을 끊을때까지 대기를 반복
        while (recvByte = recv(    // (블로킹 함수임)
                clientSocket,       //클라이언트가 접속 시도시 보낸 소켓의 정보
                recvData,           //buffer
                sizeof(recvData),   //buffer의 크기
                0
        )){
            printf("recv message : %s \n", recvData);
            //클라이언트에게 send

            // 서버에게서 데이터 입력받아 전송
            fputs("Input String : ", stdout);
            scanf("%s", sendData);
            send(clientSocket, sendData, recvByte, 0);
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
