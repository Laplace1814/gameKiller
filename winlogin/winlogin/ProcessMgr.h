#pragma once


struct ProcessInformation {
	int ProcessPid;
	std::string ProcessName;
	std::vector<std::string> ModuleArray;
};


class ProcessMgr
{

public:
	ProcessMgr();
	~ProcessMgr();

	std::vector<ProcessInformation> ProcessInfoList;
	void getModuleSnap(ProcessInformation* pProcessInformation);
	void getProcessSnap();
	void killProcess();

	bool isBlackModule(std::string& str);
	bool isWhiteProcess(std::string& str);
private:
	std::vector<std::string> WhiteProcessList;
	std::vector<std::string> BlackModuleList;
};




