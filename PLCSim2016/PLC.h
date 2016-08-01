// PLC.h : main header file for the PLC application
//

#if !defined(AFX_PLC_H__6379F7BC_4946_4607_B5C9_F8AB3A407A3A__INCLUDED_)
#define AFX_PLC_H__6379F7BC_4946_4607_B5C9_F8AB3A407A3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPLCApp:
// See PLC.cpp for the implementation of this class
//

class CPLCApp : public CWinApp
{
public:
	CPLCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPLCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLC_H__6379F7BC_4946_4607_B5C9_F8AB3A407A3A__INCLUDED_)
