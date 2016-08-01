// PLCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PLC.h"
#include "PLCDlg.h"

#include ".\\core\\cCore.h"
#include ".\\lib\\lib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPLCDlg dialog

CPLCDlg::CPLCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPLCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPLCDlg)
	m_RunString = _T("");
	m_EnableTrace = 1;
	m_DspLadderFilePath = _T("");
	m_DspLogPath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	pPLC = NULL;
	RESETALL();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPLCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPLCDlg)
	DDX_Control(pDX, IDC_RUNSTATE, m_RunState);
	DDX_Control(pDX, IDC_STOP, m_STOP);
	DDX_Control(pDX, IDC_RUN, m_RUN);
	DDX_Text(pDX, IDC_STATUS, m_RunString);
//	DDX_Radio(pDX, IDC_RADIO1, m_EnableTrace);
	DDX_Text(pDX, IDC_EDIT1, m_DspLadderFilePath);
//	DDX_Text(pDX, IDC_EDIT2, m_DspLogPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPLCDlg, CDialog)
	//{{AFX_MSG_MAP(CPLCDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RUN, OnRun)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_SELLAD, OnSellad)
//	ON_BN_CLICKED(IDC_SETLOG, OnSetlog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPLCDlg message handlers

BOOL CPLCDlg::OnInitDialog()
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

void CPLCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPLCDlg::OnPaint() 
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
HCURSOR CPLCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPLCDlg::OnRun() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	if(sLadderFilePath.GetLength() > 0){
		m_DspLadderFilePath = sLadderFilePath;
		if(m_EnableTrace == 0 && sLogPath.GetLength() == 0){
			MessageBox("TRACE?", "OK?", MB_OK);
		}
		else{
			m_DspLogPath = sLogPath;
			pPLC = new cPLCCore();
			pPLC->m_Hwnd = this->GetSafeHwnd();
			if(m_EnableTrace == 0)
				pPLC->bEnableTrace = TRUE;
			
			sprintf(pPLC->sLadderFilePath,"%s",sLadderFilePath);
			sprintf(pPLC->sLogPath,"%s",sLogPath);

			pPLC->fRUN();
			GetDlgItem(IDC_RUN)->EnableWindow(FALSE);
			GetDlgItem(IDC_SELLAD)->EnableWindow(FALSE);
			/*
			GetDlgItem(IDC_SETLOG)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
			*/
			m_RunState.SetIcon(AfxGetApp()->LoadIcon(IDI_RUN));
		}
	}
	else{
		MessageBox("Ladder?!!","OK?",MB_OK);
	}
}

void CPLCDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_RUN)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELLAD)->EnableWindow(TRUE);
	/*
	GetDlgItem(IDC_SETLOG)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
	*/
	if(pPLC != NULL){
		pPLC->fSTOP();
		delete pPLC;
		m_RunState.SetIcon(AfxGetApp()->LoadIcon(IDI_STOP));
		pPLC = NULL;
	}
}

void CPLCDlg::OnSellad() 
{
	OPENFILENAME   ofn;   
	char   filename[MAX_PATH];   
	memset(&ofn,   0,   sizeof(ofn));   
	filename[0]   =   0;   
	ofn.lStructSize   =   sizeof(ofn);   
	ofn.hwndOwner   =   this->GetSafeHwnd();   
	ofn.lpstrFilter   =  "Ladder   file\0*.lad\0";   
	ofn.lpstrFile   =   (LPSTR)filename;   
	ofn.nMaxFile   =   MAX_PATH;   
	if(GetOpenFileName(&ofn))   
	{   
		sLadderFilePath = filename;
		m_DspLadderFilePath = sLadderFilePath;
		this->UpdateData(FALSE);
	}   
}

void CPLCDlg::OnSetlog() 
{
	char   filename[MAX_PATH];   
	filename[0]   =   0;   
	if(GetPathDialog(this->GetSafeHwnd(),"OnSetlog?",filename))   
	{   
		sLogPath=filename;
		m_DspLogPath = sLogPath;
		this->UpdateData(FALSE);
	}   
}

void CPLCDlg::RESETALL()
{
	int iOutputFile,iPos,iStatus;
	char sFileName[256];
	char sBuffer[3];
	sBuffer[0] = 0x30;
	sBuffer[1] = 0x31;
	sBuffer[2] = 0x00;
	sprintf(sFileName,"%s","output.txt");
	iOutputFile = _open(sFileName,_O_RDWR , _S_IREAD | _S_IWRITE );
	if(iOutputFile != -1){
		for(int i = 0;i < MAX_NUM_OUTPUT;i++){
			iPos = i;
			_lseek( iOutputFile, iPos, SEEK_SET );
			iStatus = 0;
			if(iStatus == 1)
				_write( iOutputFile, &sBuffer[1], 1);
			else
				_write( iOutputFile, &sBuffer[0], 1);
		}
		close(iOutputFile);
	}
}
