
#include <windows.h>
#include "Reg.h"
#include "RegInit.hpp"
#include <string>
#include <fstream>
#include <filesystem>
#include <atlbase.h>


#define REGKEYPARENT HKEY_LOCAL_MACHINE
#ifndef DEMO_BUILD
#define REGLOCATION "Software\\DreamWorks Interactive\\Trespasser"
#else
#define REGLOCATION "Software\\DreamWorks Interactive\\Trespasser Demo"
#endif


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


void transferDWORD(CRegKey& key, LPCSTR name)
{
	DWORD value = 0;
	key.QueryDWORDValue(name, value);
	SetRegValue(name, value);
}

void transferString(CRegKey& key, LPCSTR name, ULONG size)
{
	std::string string;
	ULONG allocated = size + 1;
	string.resize(allocated);
	key.QueryStringValue(name, string.data(), &allocated);
	SetRegString(name, string.c_str());
}

void transferBinary(CRegKey& key, LPCSTR name, ULONG size)
{
	std::vector<uint8_t> data;
	ULONG allocated = size;
	data.resize(size);
	key.QueryBinaryValue(name, data.data(), &allocated);
	SetRegData(name, data.data(), size);
}

bool CreateIniIfNotExists()
{
	if (std::filesystem::exists(inifilename))
		return false;
	std::fstream file(inifilename, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::_Noreplace );
	//file << "[" << iniappname << std::endl;
	return file.is_open();
}

void ConvertRegistryToINI()
{
	const char* keys[] = {
		strDDDEVICE_NAME			,
		strDDDEVICE_DESCRIPTION		,
		strDDDEVICE_GUID			,
		strD3DDEVICE_NAME			,
		strD3DDEVICE_DESCRIPTION	,
		strD3DDEVICE_GUID			,
		strFLAG_FULLSCREEN          ,
		strFLAG_D3D                 ,
		strFLAG_SYSTEMMEM           ,
		strFLAG_REGINIT				,
		strSIZE_WIDTH               ,
		strSIZE_HEIGHT              ,
		strPARTITION_SUBDIVISION    ,
		strPARTITION_STUFFCHILDREN  ,
		strAUTOSETTINGS             ,
		REG_KEY_PID                 ,
		REG_KEY_DATA_DRIVE          ,
		REG_KEY_INSTALLED           ,
		REG_KEY_INSTALLED_DIR       ,
		REG_KEY_NOVIDEO             ,
		REG_KEY_AUDIO_LEVEL         ,
		REG_KEY_AUDIO_EFFECT        ,
		REG_KEY_AUDIO_AMBIENT       ,
		REG_KEY_AUDIO_VOICEOVER     ,
		REG_KEY_AUDIO_MUSIC         ,
		REG_KEY_AUDIO_SUBTITLES     ,
		REG_KEY_AUDIO_ENABLE		,
		REG_KEY_AUDIO_ENABLE3D		,
		REG_KEY_GAMMA               ,
		REG_KEY_DSOUND_IGNORE       ,
		REG_KEY_DDRAW_CERT_IGNORE   ,
		REG_KEY_DDRAW_HARD_IGNORE   ,
		REG_KEY_VIEWPORT_X          ,
		REG_KEY_VIEWPORT_Y          ,
		REG_KEY_RENDERING_QUALITY   ,
		REG_KEY_AUTOLOAD            ,
		REG_KEY_SAFEMODE            ,
		REG_KEY_KEYMAP              ,
		REG_KEY_GORE                ,
		REG_KEY_INVERTMOUSE         ,
		REG_KEY_AUTOSAVE            ,
		strD3D_FILTERCACHES         ,
		strD3D_DITHER               ,
		strVIDEOCARD_TYPE			,
		strVIDEOCARD_NAME			,
		strRECOMMENDEDTEXMAX        ,
		strTRIPLEBUFFER             ,
		strRESTORE_NVIDIA       	,
		strRESTORE_NVIDIAMIPMAPS	,
		strRESTORE_NVIDIASQUARE     ,
		strPAGEMANAGED              ,
		strD3D_TITLE                ,
		strZBUFFER_BITDEPTH			,
		strHARDWARE_WATER           
	};

	CRegKey regkey;
	regkey.Open(REGKEYPARENT, REGLOCATION, KEY_READ);

	for (auto entry : keys)
	{
		DWORD type = 0;
		ULONG size = 0;
		auto status = regkey.QueryValue(entry, &type, nullptr, &size);
		if (status == ERROR_FILE_NOT_FOUND) //key does not exit
			continue;
		if (status != ERROR_SUCCESS)
		{
			_RPT1(0, "Error when reading registry key %s\n", entry);
			continue;
		}

		if (type == REG_BINARY)
			transferBinary(regkey, entry, size);
		else if (type == REG_DWORD)
			transferDWORD(regkey, entry);
		else if (type == REG_SZ)
			transferString(regkey, entry, size);
		else
		{
			_RPT1(0, "Error: could not recognize type of registry key %s\n", entry);
		}
	}
	
}

void OpenKey()
{
	if (CreateIniIfNotExists())
		ConvertRegistryToINI();
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


