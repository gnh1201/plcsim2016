
// PLCSim2016.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPLCSim2016App:
// �� Ŭ������ ������ ���ؼ��� PLCSim2016.cpp�� �����Ͻʽÿ�.
//

class CPLCSim2016App : public CWinApp
{
public:
	CPLCSim2016App();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CPLCSim2016App theApp;