#include <stdio.h>
#include <stdlib.h>

#ifndef CLASS_PLC_CORE
#define CLASS_PLC_CORE
/* define all components */

#include <Afx.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "..\\lib\\lib.h"

#define MAX_LADDER_DEPTH	5
#define MAX_NUM_INPUT	8
#define MAX_NUM_OUTPUT	8
#define MAX_NUM_AUX		500
#define MAX_NUM_STATUS	500
#define MAX_NUM_TIMER	200
#define MAX_NUM_COUNTER 100
#define MAX_NUM_REGISTER	200
#define MAX_PROG_SIZE	4096000

#define TRACE_PATH "C:\\PLCSim2016\\plccore.log"
#define LADDER_PATH "C:\\PLCSim2016\\test.lad"

#define IS_INPUT	1
#define IS_OUTPUT	2
#define IS_AUX		3
#define IS_STATUS	4
#define IS_TIMER	5
#define IS_COUNTER	6
#define IS_REGISTER	7

class stTimer;
class stCounter;
class stRegister;
class sString;

class stPLC_OBJ{
public: 
	stPLC_OBJ(){
		iClassType = IS_AUX;
		status = 1;
	}
	short status;
	void operator=(const stPLC_OBJ&); 
	stPLC_OBJ operator!()const; 
	stPLC_OBJ operator+(const stPLC_OBJ&);
	stPLC_OBJ operator*(const stPLC_OBJ&); 
public:
	int iClassType;
	int TestIsKindOf(stPLC_OBJ& a){
		return a.iClassType;
	}
};

/* 0:off,1:on */
class stInPut : public stPLC_OBJ
{
public:
	stInPut(){status = 0;iClassType = IS_INPUT;}
};
class stOutPut : public stPLC_OBJ
{
public:
	stOutPut(){status = 0;iClassType = IS_OUTPUT;}
};
class stAUX : public stPLC_OBJ
{
public:
	stAUX(){status = 0;iClassType = IS_AUX;}
};
class stSTATUS : public stPLC_OBJ
{
public:
	stSTATUS(){status = 0;iClassType = IS_STATUS;}
};

class stTimer : public stPLC_OBJ
{
public:
	stTimer(){status = 0;limit=0;clock=0;timeup=0;iClassType = IS_TIMER;}
	int limit;
	int clock;
	short timeup; 
};

class stCounter : public stPLC_OBJ
{
public:
	stCounter(){status = 0;limit=0;count=0;fire=0;/*filpflop=0;*/iClassType = IS_COUNTER;}
	int limit;
	int count;
	short fire;
	short filpflop;
};

class stRegister : public stPLC_OBJ
{
public:
	stRegister(){status = 0;iClassType = IS_REGISTER;}
	int number;
};


class cPLCCore{
public :
	stInPut* pInPut;
	stOutPut* pOutPut;
	stAUX* pAUX;
	stSTATUS* pSTATUS;
	stTimer* pTimer;
	stCounter* pCounter;
	BYTE* pMEMBUF;

	cPLCCore();
	~cPLCCore();

	int iProgSize;
	int iTimerHandle;
	UINT m_timerID;
	HWND m_Hwnd;
	HANDLE hFlowThread;
	DWORD  dwFlowThreadID;

	HANDLE hClockThread;
	DWORD  dwClockThreadID;

	BOOL bRUN;
	FILE* pTRACE;
	BOOL bEnableTrace;
	char* sLogPath;
	char* sLadderFilePath;
public:
	int fnSETAndCompute(sString *pCOMMAND, stPLC_OBJ *pPLC_OBJ, int *piStackDepth);
	int fnRSTAndCompute(sString *pCOMMAND, stPLC_OBJ *pPLC_OBJ, int *piStackDepth);
	int fnORBAndCompute(sString *pCOMMAND, stPLC_OBJ *pPLC_OBJ, int *piStackDepth);
	int fnANBAndCompute(sString* pCOMMAND, stPLC_OBJ *pPLC_OBJ,int* piStackDepth);
	void fnRESETAll();
	void fnWriteOutput();
	int fnOUTAndCompute(sString* pCOMMAND,stPLC_OBJ* pPLC_OBJ,int* piStackDepth);
	int fnLDAndCompute(sString* pCOMMAND,stPLC_OBJ* pPLC_OBJ,int* piStackDepth);
	int fnLDNotAndCompute(sString* pCOMMAND,stPLC_OBJ* pPLC_OBJ,int* piStackDepth);
	int fnANDAndCompute(sString* pCOMMAND,stPLC_OBJ* pPLC_OBJ,int* piStackDepth);
	int fnANDNotAndCompute(sString* pCOMMAND,stPLC_OBJ* pPLC_OBJ,int* piStackDepth);
	int fnORAndCompute(sString* pCOMMAND,stPLC_OBJ* pPLC_OBJ,int* piStackDepth);
	int fnORNotAndCompute(sString* pCOMMAND,stPLC_OBJ* pPLC_OBJ,int* piStackDepth);
	BOOL GetProgToken(int* iParseProgOffset);
	void fnReadInput();
	friend VOID CALLBACK MyTimerProc(HWND hwnd, UINT iMsg, UINT iTimerID, DWORD dwTime);
	friend DWORD WINAPI fnFlowCtrlThread( LPVOID pParam);
	friend DWORD WINAPI fnClockThread( LPVOID pParam);
	friend void CALLBACK timerRoutine(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	int fRUN();
	int fSTOP();
	int cReadProgIntoMEM(char* sPath);
	int PLCTRACE(char* pMsg);
};

#endif