#include "stdafx.h"

WindowMgr* pEnumWindowMgr = NULL;

WindowMgr::WindowMgr() {
	BlackWindowList.push_back("4399");
	BlackWindowList.push_back("7k7k");
	BlackWindowList.push_back("三国杀");
	BlackWindowList.push_back("小游戏");
	pEnumWindowMgr = this;
}

WindowMgr::~WindowMgr() { 
	pEnumWindowMgr = NULL;
}	

bool WindowMgr::isBlackWindows(std::string str) { 

	int pos;
	int result = 0;

	for (auto Iter = BlackWindowList.begin();
		Iter != BlackWindowList.end();
		Iter++) { 
		transform(str.begin(), str.end(), str.begin(), (int(*)(int))tolower);
		pos = str.find(Iter->c_str());
		if (pos != str.npos) { 
			return TRUE;
		}	
	}	
	return FALSE;
}	

BOOL CALLBACK WindowMgr::EnumChildProc(HWND hwnd, LPARAM lParam) { 

	char szTitle[MAX_PATH] = { 0 };
	char szClass[MAX_PATH] = { 0 };

	if (!pEnumWindowMgr) {
		return FALSE;
	}
	GetWindowTextA(hwnd, szTitle, MAX_PATH);
	GetClassNameA(hwnd, szClass, MAX_PATH);
	if (pEnumWindowMgr->isBlackWindows(szTitle)) {
		SendMessage(hwnd, WM_CLOSE, 0, 0);
	}

	return TRUE;
}

BOOL CALLBACK WindowMgr::EnumWindowsProc(HWND hwnd, LPARAM lParam) {

	char szTitle[MAX_PATH] = { 0 };		// 窗口标题
	char szClass[MAX_PATH] = { 0 };		// 窗口类名
	
	if (!pEnumWindowMgr) {
		return FALSE;
	}

	GetWindowTextA(hwnd, szTitle, MAX_PATH);
	GetClassNameA(hwnd, szClass, MAX_PATH);
	if (pEnumWindowMgr->isBlackWindows(szTitle)) {
		// 发送win32窗口消息
		SendMessage(hwnd, WM_CLOSE, 0, 0);
	}

	EnumChildWindows(hwnd, EnumChildProc, lParam);
	return TRUE;
}	

void WindowMgr::shutWindow() {
	EnumWindows(EnumWindowsProc, 0);
}	