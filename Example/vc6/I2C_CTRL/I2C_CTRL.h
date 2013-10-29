// I2C_CTRL.h : main header file for the I2C_CTRL application
//

#if !defined(AFX_I2C_CTRL_H__8348C34B_C596_46F5_9D97_09BFF1553B46__INCLUDED_)
#define AFX_I2C_CTRL_H__8348C34B_C596_46F5_9D97_09BFF1553B46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CI2C_CTRLApp:
// See I2C_CTRL.cpp for the implementation of this class
//

class CI2C_CTRLApp : public CWinApp
{
public:
	CI2C_CTRLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CI2C_CTRLApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CI2C_CTRLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_I2C_CTRL_H__8348C34B_C596_46F5_9D97_09BFF1553B46__INCLUDED_)
