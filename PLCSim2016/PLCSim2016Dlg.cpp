
// PLCSim2016Dlg.cpp : ���� ����
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

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CPLCSim2016Dlg ��ȭ ����



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


// CPLCSim2016Dlg �޽��� ó����

BOOL CPLCSim2016Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CPLCSim2016Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CPLCSim2016Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPLCSim2016Dlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
