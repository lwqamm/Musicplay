
// Musicplay.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号
#define TTMSG_WARNLIST1 5000         //字符串
#define TTMSG_WARNLIST2 5001         //结构体
#define TTMSG_WARNLIST3 5003         //数字

// CMusicplayApp:
// 有关此类的实现，请参阅 Musicplay.cpp
//

class CMusicplayApp : public CWinApp
{
public:
	CMusicplayApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	
};

extern CMusicplayApp theApp;
