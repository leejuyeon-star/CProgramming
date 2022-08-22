/*
    - char, char* char[]에 관하여
    - 한국어 깨지지 않게하는 방법 (MBCS와 WBCS(유니코드)를 동시지원하는 방법)
    - 각종 입출력 함수 정리
*/

/*
	*<MBCS와 WBCS(유니코드)를 동시지원하는 방법>
	* 자료형 사용시 동시지원하는 자료형 매크로를 사용해야 함. windows.h를 참조.
	* 문자열 사용시 _T("문자열") 형식으로 작성해야 함
	* 함수 사용시 동시지원하는 함수 매크로를 사용해야 함. tchar.h를 참조
	******************************* windows.h에서 제공하는 동시지원 자료형 ***********************************
	typedef char	CHAR;
	typedef wchar_t	WCHAR;
	#define CONST const
	typedef CHAR *		LPSTR
	typedef CONST CHAR *	LPCSTR;
	typedef WCHAR *		LPWSTR;
	typedef CONST WCHAR *	LPCWSTR;	
	******************************** windows.h에서 제공하는 자료형 관련 매크로 ************************************
	* UNICODE, _UNICODE의 정의여부를 확인 후 동시지원 자료형과 형식을 그에 맞는 자료형과 형식으로 바꿔준다
	#ifdef UNICODE
		typedef WCHAR	TCHAR;
		typedet LPWSTR	LPTSTR;
		typedef LPCWSTR	LPCTSTR;
	#else
		typedef CHAR	TCHAR;
		typedef LPSTR	LPTSTR;
		typedef LPCSTR	LPCTSTR;
	* 문자열 사용시 _T("문자열") 형식으로 작성해야 하는 이유
	#ifdef _UNICODE
		#define __T(x)	L ## x		//WBCS인 경우 문자열 앞에 L붙여야 하기 때문
	#else
		#define __T(x)	x
	#define _T(x)	__T(x)			//_T(x) == __T(x) == _TEXT(x)
	#define _TEXT(x)	__T(x)	
	//*******************************************************************************	
*/

//***********************문자 셋 변환하는 방법***************************
// * WBCS(UNICODE) 기반으로 컴파일하고자 하는 경우 "#include보다 위에" 해당 코드 삽입
// #define UNICODE
// #define _UNICODE

// * MBCS 기반으로 컴파일하고자 하는 경우 아래 코드 삽입
#undef UNICODE
#undef _UNICODE
//***********************************************************************

//**************명령 프롬프트의 인코딩 방식 변환방법******************
// UTF-8 로 변환할 경우     명령 프롬프트에 chcp 65001 입력
// EUC-KR 로 변환할 경우    명령 프롬프트에 chcp 949 입력
//*******************************************************************

//*******내 컴퓨터 한글 사용을 위한 세팅법*********
// VSC 편집기 사용, 
// 문자 셋 : MBCS인 경우, VSC의 인코딩방식 : EUC-KR, 명령 프롬프트의 인코딩 방식 : CP-949
// 문자 셋 : WBCS인 경우, VSC의 인코딩방식 : UTF-8,  명령 프롬프트의 인코딩 방식 : CP-949
//***********************************************

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>		//*WBCS 한글 사용시 필요

void ClearLineFromReadBuffer(void){
    while(getchar() != '\n');
}

int main(void){
#ifdef UNICODE		//*WBCS 한글 사용시 적용
	_wsetlocale(LC_ALL, L"korean");	
#endif

    // ******** char [TCHAR]************
    //* 1byte만 저장 가능
    _tprintf(_T("******* char ******* \n"));
    TCHAR ch1 = 'a';
    _tprintf(_T("%c \n"), ch1);
    //*********************************



    //********* (const) char* [LPCTSTR]**********
    //* 특징 : - 저장할 문자열은 static data의 rodata(read only data)에 저장되고 (const) char*는 문자열의 시작주소만 저장됨
    //*        - 상수 문자열을 저장하기 때문에 char* 대신 const char*로 표현하는것이 적절함 
    //*        - 모든 상수 문자열은 저장되면 char* 형태로 주소값을 반환함. (따라서 상수 문자열을 인자로 받을 시 매개변수 선언을 char* 형태로 해야 함)
    //* 장점 : 여러byte 저장가능, 배열 크기 정하지 않아도 됨
    //* 단점 : 문자열 수정 불가(동적할당하면 수정 가능)
    _tprintf(_T("****** char* ****** \n"));
    LPCTSTR chp1     = _T("abcd");     //대체 (CONST이므로 한번만 정의 가능)
    // LPTSTR chp2   = _T("abcd");     //경고뜸. 상수문자열을 const가 아닌 변수에 넣었기 때문 (warning: ISO C++ forbids converting a string constant to 'LPTSTR {aka char*}')
    LPTSTR chp3      = (LPTSTR)("abcd");     //?동적 할당
    LPTSTR chp4      = (LPTSTR)malloc(sizeof(TCHAR)*30);     //동적 할당 (heap에 저장) (사용예 https://zoosso.tistory.com/801)
    //한국어 한글자씩 출력하는 방법 : MBCS인 경우 한글자당 2칸 or 3칸 연속으로, WBCS인 경우 한글자당 1칸 출력해야 한다 
    _tprintf(_T("%c%c \n"), chp1[0], chp1[1]);  //== *(chp1), *(chp1+1)
    // 문자열 출력하는 방법
    _tprintf(_T("%s \n"), chp1);
    _tprintf(_T("%s \n"), chp1+1);  //두번째 문자부터 끝까지 출력
    _tprintf(_T("\n"));
    //*********************************************




    //************** char [] ***********************
    _tprintf(_T("****** char [] : **********\n"));
    //* 특징 : stack에 저장됨
    //* 장점 : 여러byte 저장 가능, 1byte단위로 수정 가능
    //* 단점 : 배열 크기 선언해야 함
    //* 선언, 정의하는 경우 : 배열 크기 생략가능
    TCHAR str1[] = _T("가나다라마 \n");
    _tprintf(_T("%s \n"), str1);

    //* 선언과 정의 따로 하는 경우 : 배열 크기 필수로 지정해야 함, 배열크기는 (저장할 문자열의 byte)+1
    // *    - 문자열 입력받아 저장하는 경우 
    TCHAR str2[4];
    _fgetts(str2, sizeof(str2), stdin);
    _tprintf(_T("%s \n"), str2);

    //*     - 상수 문자열을 저장하는 경우 : memcpy() 사용해야 함
    //***********************************************





    //********************* char* [] ****************************
    _tprintf(_T("****** char* [] : **********\n"));
    //* 형식 : 자료형 선언시 저장할 변수의 자료형에 맞는 자료형으로 선언해야 함
    //* 기능 : 
    // 예시1 - 숫자 저장시
    int arrays = 12;
    int arrayss = 1;
    int * dy [] = {&arrays, &arrayss};
    _tprintf(_T("%d \n"), *dy[0]);

    //예시2 - 문자열 저장시  (모든 상수 문자열은 저장되면 char* 형태로 주소값을 반환하는 것을 이용)
    char * strArr[3] = {"abcsd", "defdf", "ghigkl"};
    _tprintf(_T("%s \n"), strArr[1]);
    






    //************************************************************




    //************************출력버퍼에 입력하는 함수*************************
    _tprintf(_T("*********출력함수***********\n"));     

    //*출력함수1 : _tprintf();
    _tprintf(_T("출력함수1 : _tprintf(); \n"));     
    TCHAR pch1 = 'a';
    _tprintf(_T("%c \n"), pch1);     

    //*출력함수2 : int _puttchar(int c);
    //*1byte만 출력, 인자가 숫자여도 문자로 출력, 한글 불가
    _tprintf(_T("출력함수2 : int _puttchar(int c); \n"));     
    _puttchar('P');      
    _tprintf(_T("\n"));
    _puttchar(80);
    _tprintf(_T("\n"));

    //*출력함수3 : int _fputtc(int c, FILE * stream);
    //*1byte만 출력, 출력의 대상 직접 지정가능, 한글 불가
    //(stdout : 모니터 버퍼에 출력)(파일글에 쓰기 가능)
    _tprintf(_T("출력함수3 : int _fputtc(int c, FILE * stream); \n"));     
    _fputtc('P', stdout);     
    _tprintf(_T("\n"));

    //*출력함수4 : int _putts(const char * s);
    //*여러byte 출력, 자동으로 \n 됨 
    _tprintf(_T("출력함수4 : int _putts(const char * s); \n"));     
    TCHAR pch2[] = _T("abcde");
    _putts(pch2);
    TCHAR pch3[] = _T("가나다");
    _putts(pch3);

    //*출력함수5 : int _fputts(const char * s, FILE * stream);
    //?여러byte 출력, 한글불가?
    //(stdout : 모니터 버퍼에 출력)(파일글에 쓰기 가능)
    _tprintf(_T("출력함수5 : int _fputts(const char * s, FILE * stream);\n"));     
    TCHAR pch4[] = _T("abcde");
    _fputts(pch4, stdout);
    _tprintf(_T("\n"));
    TCHAR pch5[] =_T("가나다");
    _fputts(pch5, stdout);
    _tprintf(_T("\n"));

    //***************************출력버퍼를 비우는 함수*****************************
    //* 한번 입력받으면 항상 바로 버퍼를 비워 오류 가능성을 없애지
    _tprintf(_T("*************출력버퍼를 비우는 함수**************\n"));
    //* int fflush(FILE * stream);
    //* 출력버퍼를 비움으로써 버퍼에 남아있던 것을 모두 출력함
    fflush(stdout);



    //***************************입력버퍼에 입력하는 함수****************************
    _tprintf(_T("************입력함수**************** \n"));     
    //*입력함수1 : _tscanf
    //* 공백, tab, \n 이전까지 입력함
    TCHAR ggch[3];
    _tscanf(_T("%s"), ggch);
    _putts(ggch);

    
    //*입력함수2 : int getchar(void);
    //*1byte만 입력받음, 한글 불가
    _tprintf(_T("입력함수2 : int getchar(void); \n"));     
    TCHAR gch1;
    gch1 = getchar();
    ClearLineFromReadBuffer();           //(엔터 남으므로 마저 지워야 함)
    putchar(gch1);
    _tprintf(_T("\n"));


    //*입력함수3 : int _fgettc(FILE * stream);
    //*1byte만 입력받음, 문자를 입력받을 대상 지정가능, 한글 불가 
    //(stdin : 키보드 버퍼에서 받아옴) (파일글 받아오기 가능)
    _tprintf(_T("입력함수3 : int _fgettc(FILE * stream); \n"));     
    TCHAR gch2;
    gch2 = _fgettc(stdin);   
    ClearLineFromReadBuffer();             //(엔터 남으므로 마저 지워야 함)
    putchar(gch2);
    _tprintf(_T("\n"));

    // //*입력함수4 : char * _getts(char * s);
    // //?여러byte 입력받음, char[지정수]가능, char* 불가 
    // //!저장 가능한 byte수를 한정하지 않아 접근 허용하지 않은 메모리에 침범 가능. 가급적 사용x
    // _tprintf(_T("입력함수4 : char * _getts(char * s); \n"));
    // TCHAR gch3[10];
    // _getts(gch3);
    // _putts(gch3);
    

    //*입력함수5 : char * _fgetts(char * s, int n, FILE * stream);
    //최대 (n-1)byte만 저장 (\0d도 저장하므로 -1), char[지정수]가능, char* 불가
    //*공백 입력해도 계속 저장, \n입력한 경우 \n까지 저장 (ClearLineFromReadBuffer(); 넣으면 안된다!!)
    //(stdin : 키보드 버퍼에서 받아옴) (파일글 받아오기 가능)
    _tprintf(_T("입력함수5 : char * _fgetts(char * s, int n, FILE * stream); \n"));
    TCHAR gch4[100];
    _fgetts(gch4, sizeof(gch4), stdin);
    _putts(gch4);

    //***************************입력버퍼를 비우는 함수*****************************
    _tprintf(_T("*************입력버퍼를 비우는 함수**************\n"));
    //* 입력버퍼에 남은 문자열을 모두 지우는 함수
    //* void ClearLineFromReadBuffer(void){
    //*     while(getchar() != '\n');
    //* }
    TCHAR cleanch[2];
    _fgetts(cleanch, sizeof(cleanch), stdin);
    ClearLineFromReadBuffer();
    _fgetts(cleanch, sizeof(cleanch), stdin);
    ClearLineFromReadBuffer();
    _fputts(cleanch, stdout);
    //********************************************************************************


    //*************************** 기타 함수 ***************************************
    //* 메모리 할당한 byte 크기 확인하는 연산자 : sizeof


    //* '\0' 이전까지의 문자열 길이 : strlen()
    //! 주의, '\0'이 없는 문자열을 인자에 넣으면 이상한 값이 나온다
    // MBCS의 경우 : 영어 1개, 한글 3개로 간주 
    // WBCS의 경우 : 모든 문자 1개로 간주
    //****************************************************************************

    system("pause");
}
