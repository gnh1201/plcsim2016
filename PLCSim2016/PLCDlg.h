// PLCDlg.h : header file
//
#include ".\\core\\cCore.h"

#if !defined(AFX_PLCDLG_H__AC844B41_7C71_4052_B7D5_9F86E36944CF__INCLUDED_)
#define AFX_PLCDLG_H__AC844B41_7C71_4052_B7D5_9F86E36944CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPLCDlg dialog

class CPLCDlg : public CDialog
{
// Construction
public:
	void RESETALL();
	CPLCDlg(CWnd* pParent = NULL);	// standard constructor
	~CPLCDlg(){RESETALL();};

// Dialog Data
	//{{AFX_DATA(CPLCDlg)
	enum { IDD = IDD_PLC_DIALOG };
	CStatic	m_RunState;
	CButton	m_STOP;
	CButton	m_RUN;
	CString	m_RunString;
	int		m_EnableTrace;
	CString sLadderFilePath;
	CString sLogPath;
	CString	m_DspLadderFilePath;
	CString	m_DspLogPath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	cPLCCore* pPLC;
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPLCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRun();
	afx_msg void OnStop();
	afx_msg void OnSellad();
	afx_msg void OnSetlog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLCDLG_H__AC844B41_7C71_4052_B7D5_9F86E36944CF__INCLUDED_)
