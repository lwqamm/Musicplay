
// Music play.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "Music play.h"
#include "Music playDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMusicplayApp

BEGIN_MESSAGE_MAP(CMusicplayApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMusicplayApp 构造

CMusicplayApp::CMusicplayApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CMusicplayApp 对象

CMusicplayApp theApp;


// CMusicplayApp 初始化

BOOL CMusicplayApp::InitInstance()
{
	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	WNDCLASS wc;
	// #32770 is the default class name for dialogs boxes.
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);

	// Change the name of the class.
	wc.lpszClassName = _T("#QQ519400285"); //这里请再次注意，一定要保证和rc资源文件里保存的类名相同！

	// Register this class so that MFC can use it.
	BOOL bRet = AfxRegisterClass(&wc);
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	HANDLE MutexHandle = CreateMutex(NULL, FALSE, TEXT("这份即将关闭"));  //创建互斥体. 信号量为0. 有信号的状态.wait可以等待
	DWORD ErrorCode = 0;
	ErrorCode = GetLastError();
	if (ERROR_ALREADY_EXISTS == ErrorCode|| NULL == MutexHandle)
	{
		//printf("对不起,程序已经启动一份了.这份即将关闭\r\n");
		HWND hwnd=FindWindow("#QQ519400285","音乐播放器1.0");
		if (hwnd == 0)
		{
			CloseHandle(MutexHandle);
			exit(0);
		}
		CString commandLine = AfxGetApp()->m_lpCmdLine;
		CString str;
		for (size_t i = 0; i < commandLine.GetLength(); i++)
		{
			if (commandLine.Mid(i, 1) != '"')
			{
				str += commandLine.Mid(i, 1);
			}
		}
		if (str != "")
		{

			DWORD dwPid = 0;
			::GetWindowThreadProcessId(hwnd, &dwPid);
			if (dwPid==0)
			{
				CloseHandle(MutexHandle);
				exit(0);
			}
			HANDLE ha = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
			if (ha==0)
			{
				CloseHandle(MutexHandle);
				exit(0);
			}
			int strl = strlen(str.GetBuffer()) + 1;
			void* pRemoteAdrr = VirtualAllocEx(ha, NULL, strl , MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			str.ReleaseBuffer();
			if (pRemoteAdrr==NULL)
			{
				CloseHandle(MutexHandle);
				exit(0);
			}
			WriteProcessMemory(ha, pRemoteAdrr, str.GetBuffer(), strl-1, NULL);
			::SendMessage(hwnd, TTMSG_WARNLIST1, (WPARAM)strl-1, (LPARAM)(LPCTSTR)pRemoteAdrr);
			CloseHandle(ha);
		}

		CloseHandle(MutexHandle);
		exit(0);
		
	}





	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CMusicplayDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

