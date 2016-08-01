
// PLCSim2016Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "PLCSim2016.h"
#include "PLCSim2016Dlg.h"
#include "afxdialogex.h"

#include "PLCDlg.h"
#include "IO_SIMULATORDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CIO_SIMULATORDlg* pdlg;

////////////////////////////////////////////////////////////////////

#define OUTPUT_FILE "C:\\PLCSim2016\\output.txt"

int fnReadOutput(char* pBuffer) {
	FILE* pOutFile;
	char sFileName[256];
	sprintf(sFileName, "%s", OUTPUT_FILE);
	pOutFile = fopen(OUTPUT_FILE, "r");
	if (pOutFile != NULL) {
		fread(pBuffer, 1, 256, pOutFile);
		fclose(pOutFile);
		return 0;
	}
	return -1;
}

VOID CALLBACK MyTimerProcIO(HWND hwnd, UINT iMsg, UINT iTimerID, DWORD dwTime)
{
	/* timeup do timer proc */
	HWND hWnd;
	char strBuf[20];
	char  sBuffer[256];
	memset(sBuffer, 0x00, 256);

	hWnd = GetDlgItem(pdlg->m_hWnd, IDC_STATUS);
	GetWindowText(hWnd, strBuf, 20);
	switch (strBuf[0]) {
	case '-':	SetWindowText(hWnd, "\\");	break;
	case '\\':	SetWindowText(hWnd, "|");	break;
	case '|':	SetWindowText(hWnd, "/");	break;
	case '/':	SetWindowText(hWnd, "-");	break;
	default:	SetWindowText(hWnd, "-");	break;
	}
	fnReadOutput(sBuffer);

	/* OUTPUT */
	for (int i = 0; i < 8; i++) {
		switch (i) {
		case 0:
			if (sBuffer[i] == 0x31)
				pdlg->m_Out0.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out0.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 1:
			if (sBuffer[i] == 0x31)
				pdlg->m_Out1.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out1.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 2:
			if (sBuffer[i] == 0x31)
				pdlg->m_Out2.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out2.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 3:
			if (sBuffer[i] == 0x31)
				pdlg->m_Out3.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out3.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 4:
			if (sBuffer[i] == 0x31)
				pdlg->m_Out4.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out4.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 5:
			if (sBuffer[i] == 0x31)
				pdlg->m_Out5.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out5.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 6:
			if (sBuffer[i] == 0x31)
				pdlg->m_Out6.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out6.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 7:
			if (sBuffer[i] == 0x31)
				pdlg->m_Out7.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out7.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		}
	}
}

BOOL openSimulationModal() {
	CIO_SIMULATORDlg dlg;
	CIO_SIMULATORDlg* m_pMainWnd;

	pdlg = &dlg;
	m_pMainWnd = &dlg;
	dlg.iTimerHandle = SetTimer(m_pMainWnd->m_hWnd, 500, 10, MyTimerProcIO);
	int nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

////////////////////////////////////////////////////////////////////

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPLCSim2016Dlg 대화 상자



CPLCSim2016Dlg::CPLCSim2016Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PLCSIM2016_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPLCSim2016Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPLCSim2016Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CPLCSim2016Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CPLCSim2016Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPLCSim2016Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPLCSim2016Dlg 메시지 처리기

BOOL CPLCSim2016Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CPLCSim2016Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPLCSim2016Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPLCSim2016Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPLCSim2016Dlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CPLCSim2016Dlg::OnBnClickedButton1()
{
	CPLCDlg* dlg = new CPLCDlg();
	dlg->Create(IDD_PLC_DIALOG, this);
	dlg->ShowWindow(SW_NORMAL);
}


void CPLCSim2016Dlg::OnBnClickedButton2()
{

	//CIO_SIMULATORDlg Dlg;
	//Dlg.DoModal();
	openSimulationModal();
}
