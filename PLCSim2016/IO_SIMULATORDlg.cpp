// IO_SIMULATORDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IO_SIMULATOR.h"
#include "IO_SIMULATORDlg.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INPUT_FILE "C:\\PLCSim2016\\input.txt"

///////////////////////////////////////////////////

BOOL fnSetInputBreakFlag = FALSE;

BOOL closeSimulationModal() {
	CIO_SIMULATORDlg dlg;
	CIO_SIMULATORDlg* m_pMainWnd;

	m_pMainWnd = &dlg;

	KillTimer(m_pMainWnd->m_hWnd, 500);

	return FALSE;
}

/////////////////////////////////////////////////////

int fnSetInput(int iPos,int iStatus){
	int iInputFile;
	char sFileName[256];
	char sBuffer[3];
	sBuffer[0] = 0x30;
	sBuffer[1] = 0x31;
	sBuffer[2] = 0x00;
	sprintf(sFileName,"%s",INPUT_FILE);
	iInputFile = _open(sFileName, _O_RDWR , _S_IREAD | _S_IWRITE );
	if(iInputFile != -1){
		_lseek( iInputFile, iPos, SEEK_SET );
		if(iStatus == 1)
			_write( iInputFile, &sBuffer[1], 1);
		else
			_write( iInputFile, &sBuffer[0], 1);
		close(iInputFile);
		return 0;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CIO_SIMULATORDlg dialog

CIO_SIMULATORDlg::CIO_SIMULATORDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIO_SIMULATORDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIO_SIMULATORDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for (int i = 0; i < 8; i++) {
		if (fnSetInputBreakFlag == TRUE) {
			break; // Áß´ÜÇØ¾ßÇÒ ½Ã ¸ØÃã 
		}
		
		fnSetInput(i, 0);
	}
}

CIO_SIMULATORDlg::~CIO_SIMULATORDlg() {
	fnSetInputBreakFlag = TRUE;
	closeSimulationModal();
}

void CIO_SIMULATORDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIO_SIMULATORDlg)
	DDX_Control(pDX, IDC_OUT7, m_Out7);
	DDX_Control(pDX, IDC_OUT6, m_Out6);
	DDX_Control(pDX, IDC_OUT5, m_Out5);
	DDX_Control(pDX, IDC_OUT4, m_Out4);
	DDX_Control(pDX, IDC_OUT3, m_Out3);
	DDX_Control(pDX, IDC_OUT2, m_Out2);
	DDX_Control(pDX, IDC_OUT1, m_Out1);
	DDX_Control(pDX, IDC_OUT0, m_Out0);
	DDX_Control(pDX, IDC_IN7, m_In7);
	DDX_Control(pDX, IDC_IN6, m_In6);
	DDX_Control(pDX, IDC_IN5, m_In5);
	DDX_Control(pDX, IDC_IN4, m_In4);
	DDX_Control(pDX, IDC_IN3, m_In3);
	DDX_Control(pDX, IDC_IN2, m_In2);
	DDX_Control(pDX, IDC_IN1, m_In1);
	DDX_Control(pDX, IDC_IN0, m_In0);
	DDX_Control(pDX, IDC_BUTTON8, m_Button8);
	DDX_Control(pDX, IDC_BUTTON7, m_Button7);
	DDX_Control(pDX, IDC_BUTTON6, m_Button6);
	DDX_Control(pDX, IDC_BUTTON5, m_Button5);
	DDX_Control(pDX, IDC_BUTTON4, m_Button4);
	DDX_Control(pDX, IDC_BUTTON3, m_Button3);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIO_SIMULATORDlg, CDialog)
	//{{AFX_MSG_MAP(CIO_SIMULATORDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIO_SIMULATORDlg message handlers

BOOL CIO_SIMULATORDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIO_SIMULATORDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIO_SIMULATORDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIO_SIMULATORDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void CIO_SIMULATORDlg::OnButton1() 
{
	char strBuffer[256];
	m_Button1.GetWindowText(strBuffer,256);
	CString str(strBuffer);
	if(str == "On"){
		str = "Off";
		fnSetInput(0,1);
		m_Button1.SetWindowText(str);
		m_In0.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
	}else{
		fnSetInput(0,0);
		str = "On";
		m_Button1.SetWindowText(str);
		m_In0.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
	}	
}

void CIO_SIMULATORDlg::OnButton2() 
{
	char strBuffer[256];
	m_Button2.GetWindowText(strBuffer,256);
	CString str(strBuffer);
	if(str == "On"){
		str = "Off";
		fnSetInput(1,1);
		m_Button2.SetWindowText(str);
		m_In1.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
	}else{
		str = "On";
		fnSetInput(1,0);
		m_Button2.SetWindowText(str);
		m_In1.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
	}	
}

void CIO_SIMULATORDlg::OnButton3() 
{
	char strBuffer[256];
	m_Button3.GetWindowText(strBuffer,256);
	CString str(strBuffer);
	if(str == "On"){
		str = "Off";
		fnSetInput(2,1);
		m_Button3.SetWindowText(str);
		m_In2.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
	}else{
		str = "On";
		fnSetInput(2,0);
		m_Button3.SetWindowText(str);
		m_In2.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
	}	
}

void CIO_SIMULATORDlg::OnButton4() 
{
	char strBuffer[256];
	m_Button4.GetWindowText(strBuffer,256);
	CString str(strBuffer);
	if(str == "On"){
		str = "Off";
		fnSetInput(3,1);
		m_Button4.SetWindowText(str);
		m_In3.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
	}else{
		str = "On";
		fnSetInput(3,0);
		m_Button4.SetWindowText(str);
		m_In3.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
	}	
}

void CIO_SIMULATORDlg::OnButton5() 
{
	char strBuffer[256];
	m_Button5.GetWindowText(strBuffer,256);
	CString str(strBuffer);
	if(str == "On"){
		str = "Off";
		fnSetInput(4,1);
		m_Button5.SetWindowText(str);
		m_In4.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
	}else{
		str = "On";
		fnSetInput(4,0);
		m_Button5.SetWindowText(str);
		m_In4.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
	}	
}

void CIO_SIMULATORDlg::OnButton6() 
{
	char strBuffer[256];
	m_Button6.GetWindowText(strBuffer,256);
	CString str(strBuffer);
	if(str == "On"){
		str = "Off";
		fnSetInput(5,1);
		m_Button6.SetWindowText(str);
		m_In5.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
	}else{
		str = "On";
		fnSetInput(5,0);
		m_Button6.SetWindowText(str);
		m_In5.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
	}	
}

void CIO_SIMULATORDlg::OnButton7() 
{
	char strBuffer[256];
	m_Button7.GetWindowText(strBuffer,256);
	CString str(strBuffer);
	if(str == "On"){
		str = "Off";
		fnSetInput(6,1);
		m_Button7.SetWindowText(str);
		m_In6.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
	}else{
		str = "On";
		fnSetInput(6,0);
		m_Button7.SetWindowText(str);
		m_In6.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
	}	
}

void CIO_SIMULATORDlg::OnButton8() 
{
	char strBuffer[256];
	m_Button8.GetWindowText(strBuffer,256);
	CString str(strBuffer);
	if(str == "On"){
		str = "Off";
		fnSetInput(7,1);
		m_Button8.SetWindowText(str);
		m_In7.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
	}else{
		str = "On";
		fnSetInput(7,0);
		m_Button8.SetWindowText(str);
		m_In7.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
	}	
}

void CIO_SIMULATORDlg::OnClose() 
{
	KillTimer(iTimerHandle);	
	CDialog::OnClose();
}

