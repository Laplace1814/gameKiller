#include "stdafx.h"
//#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

void TakeupSelf(char* exePath) {
	FILE* hFile = fopen(exePath, "r");
}

int main(int argc, char *argv[]) { 

	ProcessMgr process;
	WindowMgr window;

	TakeupSelf(argv[0]);

	do {
		process.getProcessSnap();
		process.killProcess();
		// 降压
		Sleep(500);
		window.shutWindow();
		Sleep(500);
	} while (TRUE);

	return 0;
}	
