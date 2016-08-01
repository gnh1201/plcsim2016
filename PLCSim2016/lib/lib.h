#ifndef CLASS_PLC_CORE
//#include <windows.h>
#endif

#include <memory.h>
#include <io.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shlobj.h>

#ifndef SSTRING
#define SSTRING
class sString{
public:
	sString(){
		memset(m_String,0x00,256);
	}
	~sString(){}
	char m_String[256];
};
#endif

/*
*	limit : 
*		1. can't use this object in heap area
*		2. last character can't be cKEY
*	type : int
*	argument :
*		1. (sString*) sString array for store data
*		2. int sString array depth
*		3. (char*) Data Buffer to Parse
*		4. int* Buffer Length
*		5. (char) KEY character you want to split
*	return :
*		-1. not support
*		-2. out of array index
*		0. ok
*/
int sSplitToStringArray(sString* sRetArray,
						int iArrayDepth,
						char* sStringBuffer,
						int* iBufferLen,
						char cKEY);

/*
*	type : int
*	argument :
*		1. (char*) file path want to read
*		2. (BYTE*) buffer pointer for store file
*		3. (int*)  buffer size and file size return
*	return :
*		0. success
*		-1. file read failure
*		-2. buffer size not enougth
*/
int iFileR(char* sPath,BYTE* pBUF,int* piSize);
/*
*	type : int
*	argument :
*		1. Windows Handle
*		2. (char*) Title for display
*		3. (char*) buffer for return string
*	return :
*		0. failure
*		n. buffer length
*/
int GetPathDialog(HWND hWnd,char* sTitle,char *pBuffer);

/*
*	type : int
*	argument : 
*		1. (char*) Data Buffer to Parse
*		2. int* Buffer Length
*		3. (char*) KEY character you want to find
*		4. int* key length
*		5. int* offset begin from data buffer / first key character occur in buffer
*	return :
*		0. success
*		-1. failure
*/
int iFindKeyCharacterFromBuffer(char* sDataBuffer,int* iBufferLength,char* sKEY,int* iKEY_Length,int* iOffset);
