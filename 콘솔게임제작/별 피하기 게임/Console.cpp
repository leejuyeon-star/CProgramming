// 코드 참고 : https://cafe.naver.com/gamepromaster

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>		//WBCS 한글 사용시 필요
#include "Console.h"

void Clear(void)
{
	system("cls");
}

void GotoXY(SHORT _x, SHORT _y)
{
	COORD pos = { _x, _y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetTitle(LPCTSTR _szConsoleName)
{
	SetConsoleTitle(_szConsoleName);
}

void SetColor(unsigned char _BgColor, unsigned char _TextColor)
{
	if (_BgColor > 15 || _TextColor > 15) return;

	unsigned short ColorNum = (_BgColor << 4) | _TextColor;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ColorNum);
}

void SetCursor(BOOL _bShow)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = _bShow;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void SetConsoleSize(int _col, int _lines)
{
	TCHAR setText[100];
	_stprintf(setText, _T("mode con cols=%d lines=%d"), _col, _lines);
	_tsystem(setText);
}

void PrintPlayer(DWORD PosX, DWORD PosY){
	GotoXY(PosX, PosY);
	_tprintf(_T("★"));
	BusyWaiting();
	return;
}

void BusyWaiting(void){
	for(DWORD i=0; i<1000; i++);
		for(DWORD i=0; i<1000; i++);
	return;
}

// void MovePlayer()
