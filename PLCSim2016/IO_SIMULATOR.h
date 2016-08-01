// IO_SIMULATOR.h : main header file for the IO_SIMULATOR application
//

#if !defined(AFX_IO_SIMULATOR_H__1DAAD9B0_F737_4148_8C8D_9FCEB6AB94BC__INCLUDED_)
#define AFX_IO_SIMULATOR_H__1DAAD9B0_F737_4148_8C8D_9FCEB6AB94BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIO_SIMULATORApp:
// See IO_SIMULATOR.cpp for the implementation of this class
//

class CIO_SIMULATORApp : public CWinApp
{
public:
	CIO_SIMULATORApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIO_SIMULATORApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIO_SIMULATORApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IO_SIMULATOR_H__1DAAD9B0_F737_4148_8C8D_9FCEB6AB94BC__INCLUDED_)
