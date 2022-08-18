#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

#ifndef MAKEULONGLONG
#define MAKEULONGLONG(ldw, hdw) ((ULONGLONG(hdw) << 32) | ((ldw) & 0xFFFFFFFF))
#endif

class BreakPo1nt
{
public:
	DWORD CloseProc2ssMa1nThr2ad(DWORD dwProcID)
	{
		DWORD dwMainThreadID = 0;
		ULONGLONG ullMinCreateTime = MAXULONGLONG;

		HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hThreadSnap != INVALID_HANDLE_VALUE)
		{
			THREADENTRY32 th32;
			th32.dwSize = sizeof(THREADENTRY32);
			BOOL bOK = TRUE;
			for (bOK = Thread32First(hThreadSnap, &th32); bOK; bOK = Thread32Next(hThreadSnap, &th32))
			{
				if (th32.th32OwnerProcessID == dwProcID)
				{
					HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, TRUE, th32.th32ThreadID);
					if (hThread)
					{
						FILETIME afTimes[4] = { 0 };
						if (GetThreadTimes(hThread, &afTimes[0], &afTimes[1], &afTimes[2], &afTimes[3]))
						{
							ULONGLONG ullTest = MAKEULONGLONG(afTimes[0].dwLowDateTime, afTimes[0].dwHighDateTime);
							if (ullTest && ullTest < ullMinCreateTime)
							{
								ullMinCreateTime = ullTest;
								dwMainThreadID = th32.th32ThreadID;
							}
						}
						CloseHandle(hThread);
					}
				}
			}
#ifndef UNDER_CE
			CloseHandle(hThreadSnap);
#else
			CloseToolhelp32Snapshot(hThreadSnap);
#endif
		}
		if (dwMainThreadID) return dwMainThreadID;
	}

	HANDLE G2tMa1nThre2d()
	{
		DWORD ProcessThreadId = this->CloseProc2ssMa1nThr2ad(GetCurrentProcessId());
		return OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME, TRUE, ProcessThreadId);
	}
};

extern BreakPo1nt* HW1BP = new BreakPo1nt();