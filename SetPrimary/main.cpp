/*
Set Primary Monitor Main

Copyright (c) 2016 Kobchaipuk Kemapirom
*/

#include "main.h"

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFOEX monitor = {};
	monitor.cbSize = sizeof(monitor);
	GetMonitorInfo(hMonitor, &monitor);

	DEVMODE devmode = {};
	devmode.dmSize = sizeof(devmode);
	EnumDisplaySettings(monitor.szDevice, ENUM_CURRENT_SETTINGS, &devmode);
	devmode.dmFields = DM_POSITION;
	devmode.dmPosition.x = devmode.dmPosition.x - primary_devmode.dmPosition.x;
	devmode.dmPosition.y = devmode.dmPosition.y - primary_devmode.dmPosition.y;
	
	int flags = CDS_UPDATEREGISTRY | CDS_NORESET;
	if (hMonitor == primary_hmonitor)
		flags |= CDS_SET_PRIMARY;

	LRESULT res = ChangeDisplaySettingsEx(
		monitor.szDevice, &devmode, NULL,
		flags,
		NULL);

	return res == 0;
}



int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	LPCTSTR title = TEXT("Make Primary Monitor");
	LRESULT res = 0;
	POINT current_pos;
	GetCursorPos(&current_pos);

	HMONITOR hmonitor = MonitorFromPoint(current_pos, MONITOR_DEFAULTTONEAREST);
	if (hmonitor == 0)
	{
		MessageBox(NULL, TEXT("Unable to determine current monitor."), title, MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	MONITORINFOEX current_monitor = {};
	current_monitor.cbSize = sizeof(current_monitor);
	GetMonitorInfo(hmonitor, &current_monitor);


	primary_hmonitor = hmonitor;
	primary_devmode.dmSize = sizeof(primary_devmode);
	res = EnumDisplaySettings(current_monitor.szDevice, ENUM_CURRENT_SETTINGS, &primary_devmode);
	
	if (current_monitor.dwFlags & MONITORINFOF_PRIMARY > 0)
	{
		MessageBox(NULL, TEXT("This monitor is already Primary."), title, MB_OK | MB_ICONINFORMATION);
		return 0;
	}
	
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (DWORD)hmonitor);

	res = ChangeDisplaySettingsEx(NULL, NULL, NULL, 0, NULL);

	DWORD error = GetLastError();

	if (res == DISP_CHANGE_SUCCESSFUL)
	{
		MessageBox(NULL, TEXT("Success!"), title, MB_OK | MB_ICONINFORMATION);

		//ChangeDisplaySettingsEx(monitor_info.szDevice, &devmode, NULL, NULL, NULL);
	}
	else
	{
		TCHAR message[255];
		swprintf_s(message, TEXT("Make Primary Monitor error is %d"), 1);
		MessageBox(NULL, message, title, MB_OK | MB_ICONERROR);
	}
	
	return 0;
}