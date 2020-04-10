#pragma once



class WindowMgr
{

public:
	WindowMgr();
	~WindowMgr();



	void shutWindow();
	bool isBlackWindows(std::string str);
	static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

private:
	std::vector<std::string> BlackWindowList;
};