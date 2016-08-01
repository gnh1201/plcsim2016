// IO_SIMULATOR.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "IO_SIMULATOR.h"
#include "IO_SIMULATORDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIO_SIMULATORDlg* pdlg;

#define OUTPUT_FILE "C:\\PLCSim2016\\output.txt"

int fnReadOutput(char* pBuffer){
	FILE* pOutFile;
	char sFileName[256];
	sprintf(sFileName,"%s",OUTPUT_FILE);
	pOutFile = fopen(OUTPUT_FILE,"r");
	if(pOutFile != NULL){
		fread(pBuffer,1,256,pOutFile);
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
	memset(sBuffer,0x00,256);

	hWnd = GetDlgItem(pdlg->m_hWnd,IDC_STATUS);
	GetWindowText(hWnd,strBuf,20);
	switch(strBuf[0]){
	case '-':	SetWindowText(hWnd,"\\");	break;
	case '\\':	SetWindowText(hWnd,"|");	break;
	case '|':	SetWindowText(hWnd,"/");	break;
	case '/':	SetWindowText(hWnd,"-");	break;
	default:	SetWindowText(hWnd,"-");	break;
	}
	fnReadOutput(sBuffer);

	/*§PÅª OUTPUT ÀÉ*/
	for(int i = 0;i < 8;i++){
		switch(i){
		case 0:
			if(sBuffer[i] == 0x31)
				pdlg->m_Out0.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out0.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 1:
			if(sBuffer[i] == 0x31)
				pdlg->m_Out1.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out1.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 2:
			if(sBuffer[i] == 0x31)
				pdlg->m_Out2.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out2.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 3:
			if(sBuffer[i] == 0x31)
				pdlg->m_Out3.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out3.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 4:
			if(sBuffer[i] == 0x31)
				pdlg->m_Out4.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out4.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 5:
			if(sBuffer[i] == 0x31)
				pdlg->m_Out5.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out5.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 6:
			if(sBuffer[i] == 0x31)
				pdlg->m_Out6.SetIcon(AfxGetApp()->LoadIcon(IDI_ON));
			else
				pdlg->m_Out6.SetIcon(AfxGetApp()->LoadIcon(IDI_OFF));
			break;
		case 7:
			if(sBuffer[i] == 0x31)
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
	dlg.iTimerHandle = SetTimer(m_pMainWnd->m_hWnd,500,10,MyTimerProcIO);
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
