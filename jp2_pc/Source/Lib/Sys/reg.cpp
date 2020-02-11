
#include <windows.h>
#include "Reg.h"
#include "RegInit.hpp"
#include <string>


/*
#define REGKEYPARENT HKEY_LOCAL_MACHINE
#ifndef DEMO_BUILD
#define REGLOCATION "Software\\DreamWorks Interactive\\Trespasser"
#else
#define REGLOCATION "Software\\DreamWorks Interactive\\Trespasser Demo"
#endif
*/

//
// Module specific variables.
//


BOOL bSafeModeReg = FALSE;

void DisableSafemode()
{
	bSafeModeReg = FALSE;
}
/*
HKEY    g_hKey = NULL;


void CloseKey(BOOL b_change_safemode)
{
	if (g_hKey && b_change_safemode)
	{
		SetRegValue(REG_KEY_SAFEMODE, FALSE);
		RegCloseKey(g_hKey);
	}
}

void OpenKey ()
{
	if (RegOpenKeyEx (REGKEYPARENT,
				REGLOCATION,
				0,
				KEY_ALL_ACCESS,
				&g_hKey)!=ERROR_SUCCESS)
	{
		RegCreateKey (REGKEYPARENT,
				REGLOCATION,
				&g_hKey);
	}

	// Get ready for safe mode.
	if (g_hKey)
	{
		bSafeModeReg = GetRegValue(REG_KEY_SAFEMODE, FALSE);
		SetRegValue(REG_KEY_SAFEMODE, TRUE);
	}
}

void SetRegValue (LPCSTR lpszVal, int nVal)
{
	RegSetValueEx (g_hKey,
		lpszVal,
		NULL,
		REG_DWORD,
		(LPBYTE)&nVal,
		sizeof(nVal));
}


int GetRegValue (LPCSTR lpszVal, int nDefault)
{
	DWORD dwType,dwSize;
	DWORD nRes;
	dwSize = sizeof(nRes);
	if (RegQueryValueEx (g_hKey,
		lpszVal,
		NULL,
		&dwType,
		(LPBYTE)&nRes,
		&dwSize)==ERROR_SUCCESS)
	{
		if (dwType==REG_DWORD)
			return (int)nRes;
	}
	return nDefault;
}

void SetRegString (LPCSTR lpszVal, LPCSTR lpszString)
{
	RegSetValueEx (g_hKey,
		lpszVal,
		NULL,
		REG_SZ,
		(LPBYTE)lpszString,
		lstrlen(lpszString)+1);
}

// returns length
int GetRegString (LPCSTR lpszVal, LPSTR lpszString, int nSize, LPCSTR lpszDefault)
{
	DWORD dwType,dwSize;
	dwSize = nSize;
	if (RegQueryValueEx (g_hKey,
		lpszVal,
		NULL,
		&dwType,
		(LPBYTE)lpszString,
		&dwSize)!=ERROR_SUCCESS)
	{
		if (lpszDefault)
		{
			lstrcpy (lpszString, lpszDefault);
			dwSize = lstrlen(lpszDefault);
		}
		else
			dwSize = 0;
	}
	return dwSize;
}

void SetRegData (LPCSTR lpszVal, LPBYTE lpszData, int nSize)
{
	RegSetValueEx (g_hKey,
		lpszVal,
		NULL,
		REG_BINARY,
		lpszData,
		nSize);
}

// returns length
int GetRegData (LPCSTR lpszVal, LPBYTE lpszData, int nSize)
{
	DWORD dwType,dwSize;
	dwSize = nSize;
	if (RegQueryValueEx (g_hKey,
		lpszVal,
		NULL,
		&dwType,
		lpszData,
		&dwSize)!=ERROR_SUCCESS)
		dwSize = 0;
	return dwSize;
}

void DeleteValue(LPCSTR lpszVal)
{
	RegDeleteValue (g_hKey, lpszVal);
}

void SetRegFloat(LPCSTR lpszVal, float fVal)
{
	SetRegData(lpszVal, ((unsigned char*)(&fVal)), sizeof(fVal));
}

float GetRegFloat(LPCSTR lpszVal, float fDefault)
{
	float f_retval;
	int i_size = GetRegData(lpszVal, ((unsigned char*)(&f_retval)), sizeof(f_retval));
	if (i_size != sizeof(f_retval))
		return f_retval = fDefault;
	return f_retval;
}
*/

static constexpr char iniappname[] = "OpenTrespasser";
static constexpr char inifilename[] = ".\\opentrespasser.ini"; //Path is necessary, otherwise the Windows directory is searched for the ini file


void OpenKey()
{
}

void CloseKey(BOOL b_change_safemode)
{
}

int GetRegValue(LPCSTR lpszVal, int nDefault)
{
	return GetPrivateProfileInt(iniappname, lpszVal, nDefault, inifilename);
}

int GetRegData(LPCSTR lpszVal, LPBYTE lpszData, int nSize)
{
	if (!GetPrivateProfileStruct(iniappname, lpszVal, lpszData, nSize, inifilename))		
		return 0;
	return nSize;
}

int GetRegString(LPCSTR lpszVal, LPSTR lpszString, int nSize, LPCSTR lpszDefault)
{
	return GetPrivateProfileString(iniappname, lpszVal, lpszDefault, lpszString, nSize, inifilename);
}

void SetRegValue(LPCSTR lpszVal, int nVal)
{
	std::string string = std::to_string(nVal);
	WritePrivateProfileString(iniappname, lpszVal, string.c_str(), inifilename);
}

void SetRegString(LPCSTR lpszVal, LPCSTR lpszString)
{
	WritePrivateProfileString(iniappname, lpszVal, lpszString, inifilename);
}

void SetRegData(LPCSTR lpszVal, LPBYTE lpszData, int nSize)
{
	WritePrivateProfileStruct(iniappname, lpszVal, lpszData, nSize, inifilename);
}

void DeleteValue(LPCSTR lpszVal)
{
	WritePrivateProfileString(iniappname, lpszVal, "", inifilename);
}


