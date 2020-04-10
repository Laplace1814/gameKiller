#include "stdafx.h"

ProcessMgr::ProcessMgr() { 
	BlackModuleList.push_back("mono");
	BlackModuleList.push_back("d3d");
	BlackModuleList.push_back("taskmgr");
	
	WhiteProcessList.push_back("explorer.exe");
	WhiteProcessList.push_back("devenv.exe");
	WhiteProcessList.push_back("servicehub");
	WhiteProcessList.push_back("searchui");
	WhiteProcessList.push_back("windowsinternal");
	WhiteProcessList.push_back("edge");
	WhiteProcessList.push_back("framehost");


}	
ProcessMgr::~ProcessMgr() {
}


bool ProcessMgr::isWhiteProcess(std::string str) {

	int pos;
	int result = 0;

	for (auto Iter = WhiteProcessList.begin();
		Iter != WhiteProcessList.end();
		Iter++) {
		transform(str.begin(), str.end(), str.begin(), (int(*)(int))tolower);
		pos = str.find(Iter->c_str());
		if (pos != str.npos) {
			return TRUE;
		}
	}
	return FALSE;
}


bool ProcessMgr::isWhiteProcess(std::string& str) {

	int pos;
	int result = 0;

	for (auto Iter = WhiteProcessList.begin();
		Iter != WhiteProcessList.end();
		Iter++) {
		transform(str.begin(), str.end(), str.begin(), (int(*)(int))tolower);
		pos = str.find(Iter->c_str());
		if (pos != str.npos) {
			return TRUE;
		}
	}
	return FALSE;
}

bool ProcessMgr::isBlackModule(std::string& str) {
	
	int pos;
	int result = 0;

	for (auto Iter = BlackModuleList.begin();
		Iter != BlackModuleList.end();
		Iter++) {
		transform(str.begin(), str.end(), str.begin(), (int(*)(int))tolower);
		pos = str.find(Iter->c_str());
		if (pos != str.npos) {
			return TRUE;
		}
	}
	return FALSE;
}

void ProcessMgr::getModuleSnap(ProcessInformation* pProcessInformation) {
	HANDLE hModuleSnap = NULL;
	MODULEENTRY32 moduleEntry = { 0 };
	std::string szModuleName;
	BOOL bRet = FALSE;
	int pid = pProcessInformation->ProcessPid;

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (INVALID_HANDLE_VALUE == hModuleSnap) { 
		return;
	}	
	// 从快照中遍历所有模块
	moduleEntry.dwSize = sizeof(MODULEENTRY32); 
	bRet = Module32First(hModuleSnap, &moduleEntry);
	while (bRet) { 
		szModuleName = moduleEntry.szModule;
		pProcessInformation->ModuleArray.push_back(szModuleName);
		bRet = Module32Next(hModuleSnap, &moduleEntry);
	}	
	//关闭快照句柄
	CloseHandle(hModuleSnap);
}	

void ProcessMgr::getProcessSnap() {
	HANDLE hProcessSnap = NULL;
	ProcessInformation pProcessInformation = { 0 };
	PROCESSENTRY32 processEntry = { 0 };
	BOOL bRet = FALSE;

	std::vector<ProcessInformation>().swap(ProcessInfoList);

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) { 
		// CreateToolhelp32Snapshot failed
		return;
	}	
	processEntry.dwSize = sizeof(PROCESSENTRY32);
	// 从快照中遍历所有进程
	bRet = Process32First(hProcessSnap, &processEntry);
	while (bRet) { 

		if (!isWhiteProcess(processEntry.szExeFile)) {
			std::vector<std::string>().swap(pProcessInformation.ModuleArray);

			pProcessInformation.ProcessPid = processEntry.th32ProcessID;
			pProcessInformation.ProcessName = processEntry.szExeFile;

			//printf("processName %s\n", processEntry.szExeFile);

			getModuleSnap(&pProcessInformation);
			ProcessInfoList.push_back(pProcessInformation);
		}

		bRet = Process32Next(hProcessSnap, &processEntry);
	}	
	//关闭快照句柄
	CloseHandle(hProcessSnap);
}	

void ProcessMgr::TerminateProcess(int pid) { 

	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if (hProcess) {
		::TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
	}
}


void ProcessMgr::killProcess() { 

	std::string szModuleName;
	int result = 0;
	unsigned int i = 0;

	for (auto ProcessIter = ProcessInfoList.begin(); 
		ProcessIter != ProcessInfoList.end(); 
		ProcessIter++) {

		if (isBlackModule(ProcessIter->ProcessName)) {
			this->TerminateProcess(ProcessIter->ProcessPid);
		}
		
		for (i = 0; i < ProcessIter->ModuleArray.size(); i++) { 
			if (isBlackModule(ProcessIter->ModuleArray[i])) { 
				this->TerminateProcess(ProcessIter->ProcessPid);
			}
		}
	}
}





