// IO_SIMULATORDlg.h : header file
//

#if !defined(AFX_IO_SIMULATORDLG_H__CB857783_7B69_4AA3_949F_6D9AD6AF8B50__INCLUDED_)
#define AFX_IO_SIMULATORDLG_H__CB857783_7B69_4AA3_949F_6D9AD6AF8B50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIO_SIMULATORDlg dialog

class CIO_SIMULATORDlg : public CDialog
{
// Construction
public:
	CIO_SIMULATORDlg(CWnd* pParent = NULL);	// standard constructor
	~CIO_SIMULATORDlg(); // Destory
	int iTimerHandle;
// Dialog Data
	//{{AFX_DATA(CIO_SIMULATORDlg)
	enum { IDD = IDD_IO_SIMULATOR_DIALOG };
	CStatic	m_Out7;
	CStatic	m_Out6;
	CStatic	m_Out5;
	CStatic	m_Out4;
	CStatic	m_Out3;
	CStatic	m_Out2;
	CStatic	m_Out1;
	CStatic	m_Out0;
	CStatic	m_In7;
	CStatic	m_In6;
	CStatic	m_In5;
	CStatic	m_In4;
	CStatic	m_In3;
	CStatic	m_In2;
	CStatic	m_In1;
	CStatic	m_In0;
	CButton	m_Button8;
	CButton	m_Button7;
	CButton	m_Button6;
	CButton	m_Button5;
	CButton	m_Button4;
	CButton	m_Button3;
	CButton	m_Button2;
	CButton	m_Button1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIO_SIMULATORDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIO_SIMULATORDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IO_SIMULATORDLG_H__CB857783_7B69_4AA3_949F_6D9AD6AF8B50__INCLUDED_)
