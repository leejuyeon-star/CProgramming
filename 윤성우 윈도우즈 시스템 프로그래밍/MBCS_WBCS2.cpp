/*
	예제 2-10
	<MBCS와 WBCS(유니코드)를 동시지원하는 방법>
	* 자료형 사용시 동시지원하는 자료형 매크로를 사용해야 함. windows.h를 참조.
	* 문자열 사용시 _T("문자열") 형식으로 작성해야 함
	* 함수 사용시 동시지원하는 함수 매크로를 사용해야 함. tchar.h를 참조.
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
// * 이 경우 _tmain 불가. main만 가능
// #define UNICODE
// #define _UNICODE

// * MBCS 기반으로 컴파일하고자 하는 경우 아래 코드 삽입
// #undef UNICODE
// #undef _UNICODE
//**************************************************



#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>		//동시지원하는 함수를 MBCS, WBCS 함수로 변환해줌. 함수 사용시 이 파일에서 함수 검색.
#include <locale.h>		//WBCS 한글 사용시 필요	

// int _tmain(int argc, TCHAR* argv[])	//이유는 모르겠으나 WBCS인 경우 _tmain 사용시 오류발생
int main(void)		//WBCS인 경우 main 사용(MBCS도 사용해도 무관함)
{	

#ifdef UNICODE		// WBCS 한글 사용시 적용
	_wsetlocale(LC_ALL, L"korean");	
#endif

	//문자열의 sizeof, len 확인	
	TCHAR str1[] = _T("abcde");
	TCHAR str2[] = _T("가나다라마");

	_tprintf(_T("%s \n"), str2);

	_tprintf(_T("string size: %d \n"), sizeof(str1));		//메모리 할당 크기 출력
	_tprintf(_T("string length: %d \n"), _tcslen(str1));	//문자열의 길이 출력(문자의 개수 출력)

	_tprintf(_T("string size: %d \n"), sizeof(str2));		//메모리 할당 크기 출력
	_tprintf(_T("string length: %d \n"), _tcslen(str2));	//문자열의 길이 출력(문자의 개수 출력)



	// //********입력한 두 문자열 결합하기************
	TCHAR str3[100];
	TCHAR str4[50];
	_fputts(_T("Input String 1 : "), stdout);	
	_tscanf(_T("%s"), str3);
	_fputts(_T("Input String 2 : "), stdout);	
	_tscanf(_T("%s"), str4);

	_tcscat(str3, str4);
	_tprintf(_T("String1 + String2 : %s \n"), str3);
	// //************************************

	return 0;
}
