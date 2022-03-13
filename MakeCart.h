// MakeCart.h : main header file for the MAKECART application
//

#if !defined(AFX_MAKECART_H__F3A93162_7460_4FC2_9BE7_A220D53C8CB7__INCLUDED_)
#define AFX_MAKECART_H__F3A93162_7460_4FC2_9BE7_A220D53C8CB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMakeCartApp:
// See MakeCart.cpp for the implementation of this class
//

class CMakeCartApp : public CWinApp
{
public:
	CMakeCartApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMakeCartApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMakeCartApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAKECART_H__F3A93162_7460_4FC2_9BE7_A220D53C8CB7__INCLUDED_)
