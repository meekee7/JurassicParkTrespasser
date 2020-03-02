// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__20EAD00E_3303_11D2_A32C_00A0C92A3A07__INCLUDED_)
#define AFX_STDAFX_H__20EAD00E_3303_11D2_A32C_00A0C92A3A07__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#undef DIRECT3D_VERSION //The wrong D3D version comes from somewhere in the MFC headers
#define DIRECT3D_VERSION 0x0700 //We must set the correct version here

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__20EAD00E_3303_11D2_A32C_00A0C92A3A07__INCLUDED_)
