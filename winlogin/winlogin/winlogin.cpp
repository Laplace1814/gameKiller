#include "stdafx.h"
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

void TakeupSelf(char* exePath) {
	FILE* hFile = fopen(exePath, "r");
}

DWORD WINAPI ProcessThreadProc(LPVOID lpParam) { 

	ProcessMgr process;
	do {
		process.getProcessSnap();
		process.killProcess();
		Sleep(10);
	} while (TRUE);
	return 0;
}

DWORD WINAPI WindowThreadProc(LPVOID lpParam) {

	WindowMgr window;
	do {
		window.shutWindow();
		Sleep(10);
	} while (TRUE);
	return 0;
}	


int main(int argc, char *argv[]) { 

	DWORD threadId = 0;
	TakeupSelf(argv[0]);

	CreateThread(NULL,0, ProcessThreadProc,NULL,0, &threadId);
	CreateThread(NULL, 0, WindowThreadProc, NULL, 0, &threadId);
	
	do 
	{
		Sleep(10000);
	} while (TRUE);

	return 0;
}	
