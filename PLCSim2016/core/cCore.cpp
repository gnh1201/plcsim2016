#include "cCore.h"
#include "..\\lib\\lib.h"
#include "..\Resource.h"
#include <memory.h>

//#define ENABLE_TRECE

cPLCCore* thePLCCore;
int iTimerCount;

cPLCCore::cPLCCore(){
	pInPut = new stInPut[MAX_NUM_INPUT];
	pOutPut = new stOutPut[MAX_NUM_OUTPUT];
	pAUX = new stAUX[MAX_NUM_AUX];
	pSTATUS = new stSTATUS[MAX_NUM_STATUS];
	pTimer = new stTimer[MAX_NUM_TIMER];
	pCounter = new stCounter[MAX_NUM_COUNTER];
	pMEMBUF = new BYTE[MAX_PROG_SIZE];

	sLogPath = new char[256];
	sLadderFilePath = new char[256];
	memset(pMEMBUF,0x00,MAX_PROG_SIZE);
	iProgSize = MAX_PROG_SIZE;
	bRUN = FALSE;
	hFlowThread = NULL;
	hClockThread = NULL;
	iTimerHandle = 0;
	bEnableTrace = FALSE;

	thePLCCore = this; /* 這是為了讓 Call Back  Function ?夠存取 PLC Core */

	::iTimerCount = 0;
	thePLCCore->fnRESETAll();
	thePLCCore->fnWriteOutput();
	pTRACE = fopen(TRACE_PATH,"ab");
	PLCTRACE("PLCCore Log open");
}

cPLCCore::~cPLCCore(){
	thePLCCore->fnRESETAll();
	thePLCCore->fnWriteOutput();
	delete pInPut;
	delete pOutPut;
	delete pAUX;
	delete pSTATUS;
	delete pTimer;
	delete pCounter;
	delete pMEMBUF;
	delete sLogPath;
	delete sLadderFilePath;
	PLCTRACE("PLCCore Log close");
	fclose(pTRACE);
}

void cPLCCore::fnRESETAll()
{
	int i=0;
	for(i = 0;i < MAX_NUM_INPUT;i++)pInPut[i].status = 0;
	for(i = 0;i < MAX_NUM_OUTPUT;i++)pOutPut[i].status = 0;
	for(i = 0;i < MAX_NUM_AUX;i++)pAUX[i].status = 0;
	for(i = 0;i < MAX_NUM_STATUS;i++)pSTATUS[i].status = 0;
	for(i = 0;i < MAX_NUM_TIMER;i++){
		pTimer[i].status = 0;
		pTimer[i].timeup = 0;
		pTimer[i].clock = 0;
		pTimer[i].limit = 0;
	}
	for(i = 0;i < MAX_NUM_COUNTER;i++){
		pCounter[i].count = 0;
		pCounter[i].fire = 0;
		pCounter[i].limit = 0;
		pCounter[i].status = 0;
		pCounter[i].filpflop = 0;
	}
}

int cPLCCore::cReadProgIntoMEM(char* sPath)
{
	return iFileR(sPath,pMEMBUF,&iProgSize);
}

VOID CALLBACK MyTimerProc(HWND hwnd, UINT iMsg, UINT iTimerID, DWORD dwTime)
{
	/* timeup do timer proc */	
	HWND hWnd;
	char strBuf[20];
#ifdef ENABLE_TRECE
	thePLCCore->PLCTRACE("timer msg");
#endif
	hWnd = GetDlgItem(thePLCCore->m_Hwnd,IDC_STATUS);
	GetWindowText(hWnd,strBuf,20);
	switch(strBuf[0])
	{
	case '-':
		SetWindowText(hWnd,"\\");
		break;
	case '\\':
		SetWindowText(hWnd,"|");
		break;
	case '|':
		SetWindowText(hWnd,"/");
		break;
	case '/':
		SetWindowText(hWnd,"-");
		break;
	default:
		SetWindowText(hWnd,"-");
		break;
	}
}

DWORD WINAPI fnClockThread( LPVOID pParam)
{
	/* Multimedia Timer */
	while(thePLCCore->bRUN)
	{
#ifdef ENABLE_TRECE
		thePLCCore->PLCTRACE("clock thread msg");
#endif
		Sleep(100);
	}
	return 0;
}

void CALLBACK timerRoutine(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
#ifdef ENABLE_TRECE
	thePLCCore->PLCTRACE("timerRoutine ...");
#endif
	int i;
	/* 處理 PLC Timer */
	for(i = 0;i < MAX_NUM_TIMER;i++){
		if(thePLCCore->pTimer[i].status == 1){
			if(thePLCCore->pTimer[i].clock == thePLCCore->pTimer[i].limit)
				thePLCCore->pTimer[i].timeup = 1;
			else
				thePLCCore->pTimer[i].clock+=100;
		}else{
			thePLCCore->pTimer[i].timeup = 0;
			thePLCCore->pTimer[i].clock = 0;
		}
	}
}

int cPLCCore::fnLDAndCompute(sString* pCOMMAND, stPLC_OBJ *pPLC_OBJ,int* piStackDepth)
{
	int iObjIndex = 0;
	iObjIndex = atoi(&pCOMMAND[1].m_String[1]);
	switch(pCOMMAND[1].m_String[0]){
	case 'X':
		pPLC_OBJ[*piStackDepth]=thePLCCore->pInPut[iObjIndex];
		break;
	case 'Y':
		pPLC_OBJ[*piStackDepth]=thePLCCore->pOutPut[iObjIndex];
		break;
	case 'M':
		pPLC_OBJ[*piStackDepth]=thePLCCore->pAUX[iObjIndex];
		break;
	case 'S':
		pPLC_OBJ[*piStackDepth]=thePLCCore->pSTATUS[iObjIndex];
		break;
	case 'T':
		pPLC_OBJ[*piStackDepth]=thePLCCore->pTimer[iObjIndex];
		break;
	case 'C':
		pPLC_OBJ[*piStackDepth]=thePLCCore->pCounter[iObjIndex];
		break;
	default:
		return -1;
	}
	return 0;
}

int cPLCCore::fnLDNotAndCompute(sString* pCOMMAND, stPLC_OBJ *pPLC_OBJ,int* piStackDepth)
{
	int iObjIndex = 0;
	iObjIndex = atoi(&pCOMMAND[1].m_String[1]);
	switch(pCOMMAND[1].m_String[0]){
	case 'X':
		pPLC_OBJ[*piStackDepth]=!thePLCCore->pInPut[iObjIndex];
		break;
	case 'Y':
		pPLC_OBJ[*piStackDepth]=!thePLCCore->pOutPut[iObjIndex];
		break;
	case 'M':
		pPLC_OBJ[*piStackDepth]=!thePLCCore->pAUX[iObjIndex];
		break;
	case 'S':
		pPLC_OBJ[*piStackDepth]=!thePLCCore->pSTATUS[iObjIndex];
		break;
	case 'T':
		pPLC_OBJ[*piStackDepth]=!thePLCCore->pTimer[iObjIndex];
		break;
	case 'C':
		pPLC_OBJ[*piStackDepth]=!thePLCCore->pCounter[iObjIndex];
		break;
	default:
		return -1;
	}
	return 0;
}

int cPLCCore::fnANDAndCompute(sString* pCOMMAND, stPLC_OBJ *pPLC_OBJ,int* piStackDepth)
{
	int iObjIndex = 0;
	iObjIndex = atoi(&pCOMMAND[1].m_String[1]);
	switch(pCOMMAND[1].m_String[0]){
	case 'X':
		pPLC_OBJ[*piStackDepth-1]=pPLC_OBJ[*piStackDepth-1]*thePLCCore->pInPut[iObjIndex];
		break;
	case 'Y':
		pPLC_OBJ[*piStackDepth-1]=pPLC_OBJ[*piStackDepth-1]*thePLCCore->pOutPut[iObjIndex];
		break;
	case 'M':
		pPLC_OBJ[*piStackDepth-1]=pPLC_OBJ[*piStackDepth-1]*thePLCCore->pAUX[iObjIndex];
		break;
	case 'S':
		pPLC_OBJ[*piStackDepth-1]=pPLC_OBJ[*piStackDepth-1]*thePLCCore->pSTATUS[iObjIndex];
		break;
	case 'T':
		pPLC_OBJ[*piStackDepth-1]=pPLC_OBJ[*piStackDepth-1]*thePLCCore->pTimer[iObjIndex];
		break;
	case 'C':
		pPLC_OBJ[*piStackDepth-1]=pPLC_OBJ[*piStackDepth-1]*thePLCCore->pCounter[iObjIndex];
		break;
	default:
		return -1;
	}
	return 0;
}

int cPLCCore::fnANDNotAndCompute(sString* pCOMMAND, stPLC_OBJ *pPLC_OBJ,int* piStackDepth)
{
	int iObjIndex = 0;
	iObjIndex = atoi(&pCOMMAND[1].m_String[1]);
	switch(pCOMMAND[1].m_String[0]){
	case 'X':
		pPLC_OBJ[*piStackDepth-1]=(pPLC_OBJ[*piStackDepth-1]*!thePLCCore->pInPut[iObjIndex]);
		break;
	case 'Y':
		pPLC_OBJ[*piStackDepth-1]=(pPLC_OBJ[*piStackDepth-1]*!thePLCCore->pOutPut[iObjIndex]);
		break;
	case 'M':
		pPLC_OBJ[*piStackDepth-1]=(pPLC_OBJ[*piStackDepth-1]*!thePLCCore->pAUX[iObjIndex]);
		break;
	case 'S':
		pPLC_OBJ[*piStackDepth-1]=(pPLC_OBJ[*piStackDepth-1]*!thePLCCore->pSTATUS[iObjIndex]);
		break;
	case 'T':
		pPLC_OBJ[*piStackDepth-1]=(pPLC_OBJ[*piStackDepth-1]*!thePLCCore->pTimer[iObjIndex]);
		break;
	case 'C':
		pPLC_OBJ[*piStackDepth-1]=(pPLC_OBJ[*piStackDepth-1]*!thePLCCore->pCounter[iObjIndex]);
		break;
	default:
		return -1;
	}
	return 0;
}

int cPLCCore::fnORAndCompute(sString* pCOMMAND, stPLC_OBJ *pPLC_OBJ,int* piStackDepth)
{
	int iObjIndex = 0;
	iObjIndex = atoi(&pCOMMAND[1].m_String[1]);
	switch(pCOMMAND[1].m_String[0]){
	case 'X':
		pPLC_OBJ[*piStackDepth-1]=pPLC_OBJ[*piStackDepth-1]+thePLCCore->pInPut[iObjIndex];
		break;
	case 'Y':
		pPLC_OBJ[*piStackDepth-1]=pPLC_OBJ[*piStackDepth-1]+thePLCCore->pOutPut[iObjIndex];
		break;
	case 'M':
		pPLC_OBJ[*piStackDepth-1]=pPLC_OBJ[*piStackDepth-1]+thePLCCore->pAUX[iObjIndex];
		break;
	case 'S':
		pPLC_OBJ[*piStackDepth-1]=pPLC_OBJ[*piStackDepth-1]+thePLCCore->pSTATUS[iObjIndex];
		break;
	case 'T':
		pPLC_OBJ[*piStackDepth-1]=pPLC_OBJ[*piStackDepth-1]+thePLCCore->pTimer[iObjIndex];
		break;
	case 'C':
		pPLC_OBJ[*piStackDepth-1]=pPLC_OBJ[*piStackDepth-1]+thePLCCore->pCounter[iObjIndex];
		break;
	default:
		return -1;
	}
	return 0;
}

int cPLCCore::fnORNotAndCompute(sString* pCOMMAND, stPLC_OBJ *pPLC_OBJ,int* piStackDepth)
{
	int iObjIndex = 0;
	iObjIndex = atoi(&pCOMMAND[1].m_String[1]);
	switch(pCOMMAND[1].m_String[0]){
	case 'X':
		pPLC_OBJ[*piStackDepth-1]=(pPLC_OBJ[*piStackDepth-1]+!thePLCCore->pInPut[iObjIndex]);
		break;
	case 'Y':
		pPLC_OBJ[*piStackDepth-1]=(pPLC_OBJ[*piStackDepth-1]+!thePLCCore->pOutPut[iObjIndex]);
		break;
	case 'M':
		pPLC_OBJ[*piStackDepth-1]=(pPLC_OBJ[*piStackDepth-1]+!thePLCCore->pAUX[iObjIndex]);
		break;
	case 'S':
		pPLC_OBJ[*piStackDepth-1]=(pPLC_OBJ[*piStackDepth-1]+!thePLCCore->pSTATUS[iObjIndex]);
		break;
	case 'T':
		pPLC_OBJ[*piStackDepth-1]=(pPLC_OBJ[*piStackDepth-1]+!thePLCCore->pTimer[iObjIndex]);
		break;
	case 'C':
		pPLC_OBJ[*piStackDepth-1]=(pPLC_OBJ[*piStackDepth-1]+!thePLCCore->pCounter[iObjIndex]);
		break;
	default:
		return -1;
	}
	return 0;
}

int cPLCCore::fnOUTAndCompute(sString *pCOMMAND, stPLC_OBJ *pPLC_OBJ, int *piStackDepth)
{
	char sDebugBuffer[256];
	int iObjIndex = 0;
	int iK_Value = 0;
	iObjIndex = atoi(&pCOMMAND[1].m_String[1]);
	switch(pCOMMAND[1].m_String[0]){
	case 'Y':
		thePLCCore->pOutPut[iObjIndex].status = pPLC_OBJ[*piStackDepth-1].status;
		break;
	case 'M':
		thePLCCore->pAUX[iObjIndex].status=pPLC_OBJ[*piStackDepth-1].status;
		break;
	case 'T':
		thePLCCore->pTimer[iObjIndex].status=pPLC_OBJ[*piStackDepth-1].status;
		iK_Value = atoi(&pCOMMAND[2].m_String[1]);
		thePLCCore->pTimer[iObjIndex].limit = iK_Value;
		break;
	case 'C':
		if(pPLC_OBJ[*piStackDepth-1].status == 1){
			iK_Value = atoi(&pCOMMAND[2].m_String[1]);
			thePLCCore->pCounter[iObjIndex].limit = iK_Value;
			if(thePLCCore->pCounter[iObjIndex].filpflop==0 && thePLCCore->pCounter[iObjIndex].fire==0){
				thePLCCore->pCounter[iObjIndex].count++;
				thePLCCore->pCounter[iObjIndex].status=1;
				thePLCCore->pCounter[iObjIndex].filpflop=1;
#ifdef ENABLE_TRECE
				sprintf(sDebugBuffer,"count msg ok : %d ,%d, %d",
					iObjIndex,
					thePLCCore->pCounter[iObjIndex].count,
					thePLCCore->pCounter[iObjIndex].status);
				thePLCCore->PLCTRACE(sDebugBuffer);
#endif
			}
		}else{
			thePLCCore->pCounter[iObjIndex].filpflop=0;
#ifdef ENABLE_TRECE
				thePLCCore->PLCTRACE("count msg off");
#endif
		}

		if((thePLCCore->pCounter[iObjIndex].count >= thePLCCore->pCounter[iObjIndex].limit)&&(thePLCCore->pCounter[iObjIndex].status==1))
			thePLCCore->pCounter[iObjIndex].fire=1;
		break;
	default:
		return -1;
	}
	return 0;
}

int cPLCCore::fnANBAndCompute(sString *pCOMMAND, stPLC_OBJ *pPLC_OBJ, int *piStackDepth)
{
	pPLC_OBJ[*piStackDepth-2]=pPLC_OBJ[*piStackDepth-2]*pPLC_OBJ[*piStackDepth-1];
	return 0;
}

int cPLCCore::fnORBAndCompute(sString *pCOMMAND, stPLC_OBJ *pPLC_OBJ, int *piStackDepth)
{
	pPLC_OBJ[*piStackDepth-2]=pPLC_OBJ[*piStackDepth-2]+pPLC_OBJ[*piStackDepth-1];
	return 0;
}

int cPLCCore::fnSETAndCompute(sString *pCOMMAND, stPLC_OBJ *pPLC_OBJ, int *piStackDepth)
{
	int iObjIndex = 0;
	iObjIndex = atoi(&pCOMMAND[1].m_String[1]);
	switch(pCOMMAND[1].m_String[0]){
	case 'Y':
		if(pPLC_OBJ[*piStackDepth-1].status == 1)
			thePLCCore->pOutPut[iObjIndex].status = 1;
		break;
	case 'M':
		if(pPLC_OBJ[*piStackDepth-1].status == 1)
			thePLCCore->pAUX[iObjIndex].status=1;
		break;
	case 'S':
		if(pPLC_OBJ[*piStackDepth-1].status == 1)
			thePLCCore->pSTATUS[iObjIndex].status=1;
		break;
	case 'T':
		if(pPLC_OBJ[*piStackDepth-1].status == 1)
			thePLCCore->pTimer[iObjIndex].status=1;
		break;
	case 'C':
		if(pPLC_OBJ[*piStackDepth-1].status == 1){
			thePLCCore->pCounter[iObjIndex].status=1;
			thePLCCore->pCounter[iObjIndex].fire=1;
		}
		break;
	default:
		return -1;
	}
	return 0;
}

int cPLCCore::fnRSTAndCompute(sString *pCOMMAND, stPLC_OBJ *pPLC_OBJ, int *piStackDepth)
{
	int iObjIndex = 0;
	iObjIndex = atoi(&pCOMMAND[1].m_String[1]);
	switch(pCOMMAND[1].m_String[0]){
	case 'Y':
		if(pPLC_OBJ[*piStackDepth-1].status == 1)
			thePLCCore->pOutPut[iObjIndex].status = 0;
		break;
	case 'M':
		if(pPLC_OBJ[*piStackDepth-1].status == 1)
			thePLCCore->pAUX[iObjIndex].status=0;
		break;
	case 'S':
		if(pPLC_OBJ[*piStackDepth-1].status == 1)
			thePLCCore->pSTATUS[iObjIndex].status=0;
		break;
	case 'T':
		if(pPLC_OBJ[*piStackDepth-1].status == 1)
			thePLCCore->pTimer[iObjIndex].status=0;
		break;
	case 'C':
		if(pPLC_OBJ[*piStackDepth-1].status == 1){
			thePLCCore->pCounter[iObjIndex].status=0;
			thePLCCore->pCounter[iObjIndex].count=0;
			thePLCCore->pCounter[iObjIndex].fire=0;
			thePLCCore->pCounter[iObjIndex].filpflop=0;
		}
		break;
	default:
		return -1;
	}
	return 0;
}

DWORD WINAPI fnFlowCtrlThread( LPVOID pParam)
{
	char cClearStack;
	int iTokenStartPos;
	int iStackDepth;
	int iParseProgOffset;
	int iTempBufferLength;	
	int iRet;
	BOOL bProgEnd;
	sString sCOMMAND[5];
	char* pTempBuffer = new char[256];
	stPLC_OBJ*	pPLC_OBJ = new stPLC_OBJ[MAX_LADDER_DEPTH];

	/* Ladder Prog */
	thePLCCore->cReadProgIntoMEM(LADDER_PATH);
	iTokenStartPos = 0;
	iStackDepth = 0;
	iParseProgOffset = 0;
	thePLCCore->fnRESETAll();
	thePLCCore->fnWriteOutput();
	// HANDLE m_hEvent=CreateEvent(NULL,TRUE,TRUE,"XXXEvent");   
	while(thePLCCore->bRUN)
	{
		iStackDepth = 0;
		iParseProgOffset = 0;
		cClearStack = 0x00;
#ifdef ENABLE_TRECE
		thePLCCore->PLCTRACE("flow thread msg");
#endif
		/* 讀取 Input 狀態讀至記憶體中 */
		thePLCCore->fnReadInput();
		do{
			memset(sCOMMAND,0x00,sizeof(sString)*5);
			iTokenStartPos = iParseProgOffset;
			/* 取得 Ladder ?式的?小段 such as LD X000，分段的方式是以 0x0d,0x0a ?為識別符號 */
			bProgEnd = thePLCCore->GetProgToken(&iParseProgOffset);
			if(bProgEnd == TRUE){
				memset(pTempBuffer,0x00,256);
				memcpy(pTempBuffer,&thePLCCore->pMEMBUF[iTokenStartPos],iParseProgOffset - iTokenStartPos);
				iParseProgOffset += 2;
				iTempBufferLength = strlen(pTempBuffer);
				/* LD X000  aa[0] = LD,aa[1] = X000 */
				iRet = sSplitToStringArray(sCOMMAND,5,pTempBuffer,&iTempBufferLength,' ');
				/* Ladder Prog Command */
				// LD
				if((sCOMMAND[0].m_String[0] == 'L') && (sCOMMAND[0].m_String[1] == 'D') && (sCOMMAND[0].m_String[2] == 0x00)){
					if(cClearStack == 0x01){
						iStackDepth--;
						cClearStack = 0x00;
					}
					iRet = thePLCCore->fnLDAndCompute(sCOMMAND,pPLC_OBJ,&iStackDepth);
					iStackDepth++;
				}// LDI
				else if((sCOMMAND[0].m_String[0] == 'L') && (sCOMMAND[0].m_String[1] == 'D') && (sCOMMAND[0].m_String[2] == 'I')){
					iRet = thePLCCore->fnLDNotAndCompute(sCOMMAND,pPLC_OBJ,&iStackDepth);
					iStackDepth++;
				}// AND
				else if((sCOMMAND[0].m_String[0] == 'A') && (sCOMMAND[0].m_String[1] == 'N') && (sCOMMAND[0].m_String[2] == 'D')){
					iRet = thePLCCore->fnANDAndCompute(sCOMMAND,pPLC_OBJ,&iStackDepth);
				}// ANI
				else if((sCOMMAND[0].m_String[0] == 'A') && (sCOMMAND[0].m_String[1] == 'N') && (sCOMMAND[0].m_String[2] == 'I')){
					iRet = thePLCCore->fnANDNotAndCompute(sCOMMAND,pPLC_OBJ,&iStackDepth);
				}// OR
				else if((sCOMMAND[0].m_String[0] == 'O') && (sCOMMAND[0].m_String[1] == 'R') && (sCOMMAND[0].m_String[2] == 0x00)){
					iRet = thePLCCore->fnORAndCompute(sCOMMAND,pPLC_OBJ,&iStackDepth);
				}// ORI
				else if((sCOMMAND[0].m_String[0] == 'O') && (sCOMMAND[0].m_String[1] == 'R') && (sCOMMAND[0].m_String[2] == 'I')){
					iRet = thePLCCore->fnORNotAndCompute(sCOMMAND,pPLC_OBJ,&iStackDepth);
				}// ANB
				else if((sCOMMAND[0].m_String[0] == 'A') && (sCOMMAND[0].m_String[1] == 'N') && (sCOMMAND[0].m_String[2] == 'B')){
					iRet = thePLCCore->fnANBAndCompute(sCOMMAND,pPLC_OBJ,&iStackDepth);
					iStackDepth--;
				}// ORB
				else if((sCOMMAND[0].m_String[0] == 'O') && (sCOMMAND[0].m_String[1] == 'R') && (sCOMMAND[0].m_String[2] == 'B')){
					iRet = thePLCCore->fnORBAndCompute(sCOMMAND,pPLC_OBJ,&iStackDepth);
					iStackDepth--;
				}// SET
				else if((sCOMMAND[0].m_String[0] == 'S') && (sCOMMAND[0].m_String[1] == 'E') && (sCOMMAND[0].m_String[2] == 'T')){
					iRet = thePLCCore->fnSETAndCompute(sCOMMAND,pPLC_OBJ,&iStackDepth);
					if(iStackDepth > 0)cClearStack=0x01;
				}// RST
				else if((sCOMMAND[0].m_String[0] == 'R') && (sCOMMAND[0].m_String[1] == 'S') && (sCOMMAND[0].m_String[2] == 'T')){
					iRet = thePLCCore->fnRSTAndCompute(sCOMMAND,pPLC_OBJ,&iStackDepth);
					if(iStackDepth > 0)cClearStack=0x01;
				}// OUT
				else if((sCOMMAND[0].m_String[0] == 'O') && (sCOMMAND[0].m_String[1] == 'U') && (sCOMMAND[0].m_String[2] == 'T')){
					iRet = thePLCCore->fnOUTAndCompute(sCOMMAND,pPLC_OBJ,&iStackDepth);
					if(iStackDepth > 0)cClearStack=0x01;
					//ResetEvent(m_hEvent);   使用 EVENT 並不??低 CPU 的 Loading,所以還是用 Sleep(1)
					//WaitForSingleObject(m_hEvent,1);
					//Sleep(1);
				}else if((sCOMMAND[0].m_String[0] == 'E') && (sCOMMAND[0].m_String[1] == 'N') && (sCOMMAND[0].m_String[2] == 'D')){
					;
				}else{
					;
				}
				
			}
			
		}while(bProgEnd);
		thePLCCore->fnWriteOutput();
		Sleep(100);
	}
	thePLCCore->fnRESETAll();
	thePLCCore->fnWriteOutput();
	// CloseHandle(m_hEvent);
	delete pTempBuffer;
	delete pPLC_OBJ;
	return 0;
}

BOOL cPLCCore::GetProgToken(int* iParseProgOffset)
{
	int iStringLen,iBufferLen;
	char sCMD_Buffer[]={0x0d,0x0a};
	iStringLen = MAX_PROG_SIZE;
	iBufferLen = 2;
	if(iFindKeyCharacterFromBuffer((char*)pMEMBUF,&iStringLen,sCMD_Buffer,&iBufferLen,iParseProgOffset) == 0)
		return TRUE;
	return FALSE;
}

void cPLCCore::fnReadInput()
{
	FILE* pFile;
	char* pBuffer;
	pBuffer = new char[256];
	pFile = fopen("input.txt","r");
	fread(pBuffer,1,256,pFile);
	fclose(pFile);
	for(int i = 0;i < MAX_NUM_INPUT;i++){
		pInPut[i].status = (pBuffer[i] - 0x30);
	}
	delete pBuffer;
}

void cPLCCore::fnWriteOutput()
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
			iStatus = pOutPut[i].status;
			if(iStatus == 1)
				_write( iOutputFile, &sBuffer[1], 1);
			else
				_write( iOutputFile, &sBuffer[0], 1);
		}
		close(iOutputFile);
	}
}

int cPLCCore::fRUN()
{
	if(bRUN == FALSE){
		bRUN = TRUE;
		PLCTRACE("plc core running");
		iTimerHandle = SetTimer(m_Hwnd,500,10,MyTimerProc);
		if(iTimerHandle == 0)
			return -1;
		hFlowThread = CreateThread(NULL,(4096*1024),fnFlowCtrlThread,NULL,0,&dwFlowThreadID);
		// 嘗試使用 Multimedia Timer 取代
		//hClockThread = CreateThread(NULL,(4096*1024),fnClockThread,NULL,0,&dwClockThreadID);
		m_timerID = timeSetEvent(100,5,timerRoutine, NULL,TIME_PERIODIC);
	}
	else{
		return -2;
	}
	return 0;
}

int cPLCCore::fSTOP()
{
	DWORD dwThread;
	if(bRUN == TRUE){
		bRUN = FALSE;
		KillTimer(m_Hwnd,iTimerHandle);
		dwThread = WaitForSingleObject(hFlowThread,5000);
		if(dwThread == WAIT_TIMEOUT)
		{
			TerminateThread(hFlowThread,0);
		}
		/* 嘗試使用 Multimedia Timer 取代
		dwThread = WaitForSingleObject(hClockThread,5000);
		if(dwThread == WAIT_TIMEOUT)
		{
			TerminateThread(hClockThread,0);
		}
		*/
		PLCTRACE("plc core stoped");
		// CloseHandle(hClockThread);
		CloseHandle(hFlowThread);
		ASSERT(m_timerID != 0);
		timeKillEvent(m_timerID);
	}
	else
	{
		return -2;
	}
	return 0;
}

int cPLCCore::PLCTRACE(char* pMsg)
{
	SYSTEMTIME dt;
#ifdef ENABLE_TRECE
	GetSystemTime(&dt);
	fprintf(pTRACE,"[%04d/%02d/%02d %02d:%02d:%02d %03d] %s\r\n",
		dt.wYear,dt.wMonth,dt.wDay,dt.wHour,dt.wMinute,dt.wSecond,dt.wMilliseconds,pMsg);
	fflush(pTRACE);
#endif
	return 0;
}

stPLC_OBJ stPLC_OBJ::operator +(const stPLC_OBJ& a)
{
	stPLC_OBJ _stPLC_OBJ;
	stTimer* b = (stTimer*)&a;
	stCounter* c = (stCounter*)&a;
	if((this->iClassType != IS_TIMER) && (this->iClassType != IS_COUNTER))
	{
		switch(a.iClassType){
		case IS_INPUT:
		case IS_OUTPUT:
		case IS_AUX:
		case IS_STATUS:
			if((this->status == 1) || (a.status == 1))	_stPLC_OBJ.status = 1;
			else _stPLC_OBJ.status = 0;
			break;
		case IS_TIMER:
			if((this->status == 1) || (a.status == 1 && b->timeup == 1)) _stPLC_OBJ.status = 1;
			else _stPLC_OBJ.status = 0;
			break;
		case IS_COUNTER:
			if((this->status == 1) || (a.status == 1 && c->fire == 1)) _stPLC_OBJ.status = 1;
			else _stPLC_OBJ.status = 0;
			break;
		}
	}else{
		if(this->iClassType == IS_TIMER){
			stTimer* d = (stTimer*)this;
			if(a.iClassType == IS_TIMER){
				if((d->status == 1 && d->timeup==1) || (a.status == 1 && b->timeup == 1)) _stPLC_OBJ.status = 1;
				else _stPLC_OBJ.status = 0;
			}else{
				if((d->status == 1 && d->timeup==1) || (a.status == 1 && c->fire == 1)) _stPLC_OBJ.status = 1;
				else _stPLC_OBJ.status = 0;
			}
		}
		if(this->iClassType == IS_COUNTER){
			stCounter* d = (stCounter*)this;
			if(a.iClassType == IS_TIMER){
				if((d->status == 1 && d->fire==1) || (a.status == 1 && b->timeup == 1)) _stPLC_OBJ.status = 1;
				else _stPLC_OBJ.status = 0;
			}else{
				if((d->status == 1 && d->fire==1) || (a.status == 1 && c->fire == 1)) _stPLC_OBJ.status = 1;
				else _stPLC_OBJ.status = 0;
			}
		}
	}
	return _stPLC_OBJ;
}

stPLC_OBJ stPLC_OBJ::operator *(const stPLC_OBJ& a)
{
	stPLC_OBJ _stPLC_OBJ;
	stTimer* b = (stTimer*)&a;
	stCounter* c = (stCounter*)&a;
	if((this->iClassType != IS_TIMER) && (this->iClassType != IS_COUNTER))
	{
		switch(a.iClassType){
		case IS_INPUT:
		case IS_OUTPUT:
		case IS_AUX:
		case IS_STATUS:
			if((this->status == 1) && (a.status == 1)) _stPLC_OBJ.status = 1;
			else _stPLC_OBJ.status = 0;
			break;
		case IS_TIMER:
			if((this->status == 1) && (a.status == 1 && b->timeup == 1)) _stPLC_OBJ.status = 1;
			else _stPLC_OBJ.status = 0;
			break;
		case IS_COUNTER:
			if((this->status == 1) && (a.status == 1 && c->fire == 1)) _stPLC_OBJ.status = 1;
			else _stPLC_OBJ.status = 0;
			break;
		}
	}else{
		if(this->iClassType == IS_TIMER){
			stTimer* d = (stTimer*)this;
			if(a.iClassType == IS_TIMER){
				if((d->status == 1 && d->timeup==1) && (a.status == 1 && b->timeup == 1)) _stPLC_OBJ.status = 1;
				else _stPLC_OBJ.status = 0;
			}else{
				if((d->status == 1 && d->timeup==1) && (a.status == 1 && c->fire == 1)) _stPLC_OBJ.status = 1;
				else _stPLC_OBJ.status = 0;
			}
		}
		if(this->iClassType == IS_COUNTER){
			stCounter* d = (stCounter*)this;
			if(a.iClassType == IS_TIMER){
				if((d->status == 1 && d->fire==1) && (a.status == 1 && b->timeup == 1)) _stPLC_OBJ.status = 1;
				else _stPLC_OBJ.status = 0;
			}else{
				if((d->status == 1 && d->fire==1) && (a.status == 1 && c->fire == 1)) _stPLC_OBJ.status = 1;
				else _stPLC_OBJ.status = 0;
			}
		}
	}
	return _stPLC_OBJ;
}

void stPLC_OBJ::operator =(const stPLC_OBJ& a)
{
	stTimer* b = (stTimer*)&a;
	stCounter* c = (stCounter*)&a;
	if((a.iClassType != IS_TIMER) && (a.iClassType != IS_COUNTER))
		this->status = a.status;
	if(a.iClassType == IS_TIMER)
		this->status = (a.status == 1 & b->timeup == 1);
	if(a.iClassType == IS_COUNTER)
		this->status = (a.status == 1 & c->fire == 1);
}

stPLC_OBJ stPLC_OBJ::operator!() const
{
	stPLC_OBJ _stPLC_OBJ;
	stTimer* d = (stTimer*)this;
	stCounter* e = (stCounter*)this;
	switch(this->iClassType){
		case IS_INPUT:
		case IS_OUTPUT:
		case IS_AUX:
		case IS_STATUS: 
			if(this->status == 1)
				_stPLC_OBJ.status = 0;
			else
				_stPLC_OBJ.status = 1;
			break;
		case IS_TIMER:
			if((this->status == 1)&&(d->timeup == 1))
				_stPLC_OBJ.status = 0;
			else
				_stPLC_OBJ.status = 1;
			break;
		case IS_COUNTER:
			if((this->status == 1)&&(e->fire == 1))
				_stPLC_OBJ.status = 0;
			else
				_stPLC_OBJ.status = 1;
			break;
	}
	return _stPLC_OBJ;
}

