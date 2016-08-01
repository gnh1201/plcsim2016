#include "lib.h"

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
						char cKEY){
	int iOccurCount=0;
	int i,j,k;
	if(sStringBuffer[*iBufferLen] == cKEY)
		return -1;
	for(i = 0;i < *iBufferLen;i++){
		if(sStringBuffer[i] == cKEY)
			iOccurCount++;
	}

	for(i=0,j=0,k=0;i < *iBufferLen;i++){
		if(sStringBuffer[i] == cKEY){
			j++;
			k=0;
			if(j > iArrayDepth)
				return -2;
		}else{
			sRetArray[j].m_String[k++] = sStringBuffer[i];
		}
	}
	return 0;
}

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
int iFindKeyCharacterFromBuffer(char* sDataBuffer,
						   int* iBufferLength,
						   char* sKEY,
						   int* iKEY_Length,
						   int* iOffset){
	/* Check if Job is over Buffer length return immediate */
	if((*iOffset + *iKEY_Length) > *iBufferLength)
		return -1;
	for(int i = *iOffset;i+*iKEY_Length < *iBufferLength;i++){
		if(memcmp(&sDataBuffer[i],sKEY,*iKEY_Length) == 0){
			*iOffset = i;
			return 0;
		}
	}	
	return -1;
}

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

int iFileR(char* sPath,BYTE* pBUF,int* piSize)
{
	struct _stat lfilestat;
	int result;
	FILE* pFile;
	result = _stat( sPath, &lfilestat );
	if( result != 0 )
		return -1;
	if(lfilestat.st_size > *piSize)
		return -2;
	*piSize = lfilestat.st_size;
	pFile = fopen(sPath,"rb");
	if(pFile == NULL)
		return -1;
	result = fread(pBUF,1,*piSize,pFile);
	if(result != *piSize)
	{
		fclose(pFile);
		return -1;
	}
	fclose(pFile);
	return 0;
}

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
int GetPathDialog(HWND hWnd,char* sTitle,char *pBuffer)   
{   
	BROWSEINFO   bf;   
	LPITEMIDLIST   lpitem;   
	memset(&bf,0,sizeof   BROWSEINFO);   
	bf.hwndOwner=hWnd;   
	bf.lpszTitle=sTitle;   
	bf.ulFlags=BIF_RETURNONLYFSDIRS;
	lpitem=SHBrowseForFolder(&bf);   
	if(lpitem==NULL)
		return   0;   
	SHGetPathFromIDList(lpitem,pBuffer);   
	return   lstrlen(pBuffer);   
}   


#ifndef LIB_UNIT_TEST
#define LIB_UNIT_TEST

void main(int argc,char** argv)
{
	int str_len,offset,keylen,iret;
	sString AA[10];
	char tt[]="AAA BBB CCC DDD EEE FFF GGG";

	str_len = strlen(tt);
	iret = sSplitToStringArray(AA,10,tt,&str_len,' ');

	char test[]="1234567890AA123";
	
	keylen=2;
	str_len= strlen(test);
	offset=0;
	iret = iFindKeyCharacterFromBuffer(test,&str_len,"AA",&keylen,&offset);

	/* iFileR Unit Test */
	/*
	int size,iRet;
	BYTE* pBuffer;
	size = 4096000;
	pBuffer = new BYTE[size];
	//iRet = iFileR("C:\\TWMJ_1032.exe",pBuffer,&size);
	GetPathDialog(NULL,"選取記錄檔路?",(char*)pBuffer);

	delete pBuffer;
	printf("Hello world\r\n");
	*/
}

#endif