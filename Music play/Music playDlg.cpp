
// Music playDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Music play.h"
#include "Music playDlg.h"
#include "afxdialogex.h"
#include <iostream>
//#include <mciapi.h>
#include"mmsystem.h" 
#pragma comment(lib, "Winmm.lib")    // 引用 Windows Multimedia API

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMusicplayDlg 对话框
CMusicplayDlg* g_this = NULL; 
extern BOOL ButtDown;
extern BOOL ButtUp;

CMusicplayDlg::CMusicplayDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MUSIC_PLAY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMusicplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SLIDER1, m_proc1);
	
}

BEGIN_MESSAGE_MAP(CMusicplayDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMusicplayDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMusicplayDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMusicplayDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMusicplayDlg::OnBnClickedButton4)
	ON_WM_DROPFILES()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON5, &CMusicplayDlg::OnBnClickedButton5)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//判断歌曲是否有效
void GetJd(char* fileName)
{
	//char fileName[] = "G:\\KuGou\\梁红 - 爱随浪花漂去远方 (Live).mp3";
	char shortName[MAX_PATH];
	GetShortPathName(fileName, shortName, sizeof(shortName) / sizeof(char));
	char cmd[MAX_PATH + 10];
	char st[MAX_PATH] = { 0 };
	wsprintf(cmd, "open %s alias mymusic", shortName);
	mciSendString(cmd, "", NULL, NULL);

	mciSendString("status mymusic length", st, sizeof(st), 0);
	int len = StrToInt(st);
	len = len / 100;
	if (len == 0)
	{
		Write_config("", "歌曲路径");

		Write_config("", "歌曲播放进度");
		return;
	}
	SetDlgItemText(g_this->m_hWnd,EDIT1, fileName);
	//"进度条进度"
	CString str=Read_config("进度条进度");
	len = StrToInt(str.GetBuffer());
	g_this->m_proc1.SetPos(len);
}

BOOL text0 = FALSE;//不带参数打开
// CMusicplayDlg 消息处理程序
BOOL CMusicplayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	AfxSetWindowText(m_hWnd,"音乐播放器1.0");
	INIPATH(0);
	
	CString str= "";
	m_brush.CreateSolidBrush(RGB(0xff, 255, 255));
	m_proc1.SetPos(0);
	CString commandLine = AfxGetApp()->m_lpCmdLine;
	
	for (size_t i = 0; i < commandLine.GetLength(); i++)
	{
		if (commandLine.Mid(i,1)!='"')
		{
			str += commandLine.Mid(i, 1);
		}
	}
	if (str=="")
	{
		text0 = TRUE;
		g_this = this;
		str = Read_config("歌曲路径");
		if (str=="")
		{
			text0 = FALSE;
			g_this = this;
			return TRUE;
		}
		GetJd(str.GetBuffer());
		
		return TRUE;
	}
	SetDlgItemText(EDIT1, str);
	//SendMessage(句柄, 194, 1, “发送的文本”)
	OnBnClickedButton1();
	g_this = this;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMusicplayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMusicplayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL g_lock = FALSE;
HANDLE g_Thread = 0;
BOOL g_pause = FALSE;
BOOL g_resume = FALSE;
BOOL g_stop = FALSE;
BOOL g_Close = FALSE;
CString g_micsPath = "";

void SetJd(char* fileName)
{
	g_micsPath = fileName;
	//char fileName[] = "G:\\KuGou\\梁红 - 爱随浪花漂去远方 (Live).mp3";
	char shortName[MAX_PATH];
	GetShortPathName(fileName, shortName, sizeof(shortName) / sizeof(char));
	char cmd[MAX_PATH + 10];
	char st[MAX_PATH] = { 0 };
	wsprintf(cmd, "open %s alias mymusic", shortName);
	mciSendString(cmd, "", NULL, NULL);
	if (text0)
	{
		CString sstr = Read_config("歌曲播放进度");
		if (sstr!="")
		{
			char file2[MAX_PATH] = { 0 };
			wsprintf(file2, "seek mymusic to %s", sstr);
			mciSendString(file2, st, sizeof(st), NULL);
			SetDlgItemText(g_this->m_hWnd,EDIT1, g_micsPath);
		}
		text0 = FALSE;
	}
	mciSendString("play mymusic", "", NULL, NULL);

	
	mciSendString("status mymusic length", st, sizeof(st), 0);
	int len=StrToInt(st);
	len = len / 100;

	Write_config("", "歌曲路径");
	Write_config("", "歌曲播放进度");
	Write_config("","进度条进度");
	if (len==0)
	{

		return;
	}
	g_lock = TRUE;
	int a1 = 0;
	int a2 = 0;//记录上次滑块位置
	Write_config(g_micsPath, "歌曲路径");
	while (g_lock)
	{
		memset(st, 0, MAX_PATH);
		mciSendString("status mymusic position", st, sizeof(st), 0);
		int tlen= StrToInt(st);int tlen1 = tlen / len;int tlen2= tlen %  len;
		if (tlen2>=len/2){tlen = tlen1 + 1;}else{tlen = tlen1;}
		if (tlen>100){tlen = 100;}
		if (!ButtDown && a1 == 0)
		{
			g_this->m_proc1.SetPos(tlen);
		}
		if (ButtDown&& a1==0)
		{
			a1= g_this->m_proc1.GetPos();
			ButtDown = FALSE;
		}
		if (ButtUp)
		{
			tlen= g_this->m_proc1.GetPos();
			if (tlen>a1|| tlen<a1)
			{
				
				g_this->m_proc1.SetPos(tlen);
				char file2[100] = { 0 };
				wsprintf(file2,"seek mymusic to %d", tlen * len);
				mciSendString(file2, st, sizeof(st), NULL);
				mciSendString("play mymusic", st, sizeof(st), 0);
				//Sleep(500);
			}
			ButtUp = FALSE;
			ButtDown = FALSE;
			a1 = 0;
		}
		//mode
		memset(st, 0, MAX_PATH);
		mciSendString("status mymusic mode", st, sizeof(st), 0);
		if (strcmp(st,"stopped")==0&& tlen==100)
		{
			g_stop = TRUE;
		}
		//暂停
		if (g_pause)
		{
			mciSendString(_T("pause mymusic"), NULL, 0, NULL);
			while (g_pause)
			{
				if (g_resume)
				{					
					mciSendString(_T("resume mymusic"), NULL, 0, NULL);
					g_pause = FALSE;
					g_resume = FALSE;
					break;
				}
				if (g_stop)
				{
					mciSendString(_T("stop mymusic"), NULL, 0, NULL);
					mciSendString(_T("close mymusic"), NULL, 0, NULL);
					g_pause = FALSE;
					g_resume = FALSE;
					g_stop = FALSE;
					g_this->miscres(3);
					g_Thread = 0;
					break;
				}
				Sleep(1);
			}
		}
		if (g_stop) //停止
		{
			mciSendString(_T("stop mymusic"), NULL, 0, NULL);
			mciSendString(_T("close mymusic"), NULL, 0, NULL);
			g_this->miscres(3);
			g_pause = FALSE;
			g_resume = FALSE;
			g_stop = FALSE;
			g_lock = FALSE;
			g_Thread = 0;
			if (g_Close)
			{
				g_Close = FALSE;
				exit(0);
			}
			break;
		}
		//保存歌曲进度
		if (tlen> a2|| tlen < a2)
		{
			a2 = tlen;
			CString str;
			str.Format("%d", tlen * len);
			Write_config(str, "歌曲播放进度");
			str.Format("%d", tlen);
			Write_config(str, "进度条进度");
		}
		//退出
		if (g_Close)
		{
			g_stop = TRUE;
		}
		Sleep(10);
	}
	return;
}

//播放
void CMusicplayDlg::OnBnClickedButton1()//
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_Thread>0)
	{
		return;
	}
	CString str;
	if (text0)
	{
		str = Read_config("歌曲路径");
	}
	else
	{
		GetDlgItemText(EDIT1, str);
	}
	
	if (str.GetLength()<6)
	{
		text0 = FALSE;
		return;
	}
	//char fileName[] = "G:\\KuGou\\梁红 - 爱随浪花漂去远方 (Live).mp3";
	g_Thread=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SetJd, str.GetBuffer(), 0, 0);
	str.ReleaseBuffer();
	miscres(0);
}

//停止
void CMusicplayDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	    // 停止播放并关闭音乐
	if (g_stop)
	{
		return;
	}
	
	miscres(3);
	g_stop = TRUE;
}

//暂停
void CMusicplayDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_pause)
	{
		return;
	}
	g_pause = TRUE;
	miscres(1);
}

//继续
void CMusicplayDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_resume)
	{
		return;
	}
	g_resume = TRUE;
	miscres(2);
}


HBRUSH CMusicplayDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG)

	{
		return m_brush;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CMusicplayDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CMusicplayDlg::miscres(int lx)
{
	if (lx<0||lx>3)
	{
		return;
	}
	switch (lx)
	{
	case 0:
	{
		CButton* p = (CButton*)GetDlgItem(IDC_BUTTON2);//停止
		p->EnableWindow(1);
		p = (CButton*)GetDlgItem(IDC_BUTTON3);//暂停
		p->EnableWindow(1);
		p = (CButton*)GetDlgItem(IDC_BUTTON4);//继续
		p->EnableWindow(0);
		p = (CButton*)GetDlgItem(IDC_BUTTON1);//播放
		p->EnableWindow(0);
		break;
	}
	case 1:
	{
		CButton* p = (CButton*)GetDlgItem(IDC_BUTTON2);//停止
		p->EnableWindow(1);
		p = (CButton*)GetDlgItem(IDC_BUTTON3);//暂停
		p->EnableWindow(0);
		p = (CButton*)GetDlgItem(IDC_BUTTON4);//继续
		p->EnableWindow(1);
		p = (CButton*)GetDlgItem(IDC_BUTTON1);//播放
		p->EnableWindow(0);
		break;
	}
	case 2:
	{
		CButton* p = (CButton*)GetDlgItem(IDC_BUTTON2);//停止
		p->EnableWindow(1);
		p = (CButton*)GetDlgItem(IDC_BUTTON3);//暂停
		p->EnableWindow(1);
		p = (CButton*)GetDlgItem(IDC_BUTTON4);//继续
		p->EnableWindow(0);
		p = (CButton*)GetDlgItem(IDC_BUTTON1);//播放
		p->EnableWindow(0);
		break;
	}
	case 3:
	{
		CButton* p = (CButton*)GetDlgItem(IDC_BUTTON2);//停止
		p->EnableWindow(0);
		p = (CButton*)GetDlgItem(IDC_BUTTON3);//暂停
		p->EnableWindow(0);
		p = (CButton*)GetDlgItem(IDC_BUTTON4);//继续
		p->EnableWindow(0);
		p = (CButton*)GetDlgItem(IDC_BUTTON1);//播放
		p->EnableWindow(1);
		m_proc1.SetPos(0);
		break;
	}
	default:
		break;
	}

}

void CMusicplayDlg::miscres1( char* str)
{
	if (strlen(str)<=6)
	{
		return;
	}
	SetDlgItemText(EDIT1, str);
	if (g_Thread >0)
	{
		OnBnClickedButton2();
		HANDLE g_Thread_1 = g_Thread;
		WaitForSingleObject(g_Thread_1, INFINITE);
	}
	OnBnClickedButton1();
}

VOID WINAPI miscres1_1(char* str1)
{
	g_this->miscres1(str1);
}

LRESULT CMusicplayDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case TTMSG_WARNLIST1:
	{
		
		char* cstr2 = (char*)lParam;
		CString str;
		GetDlgItemText(EDIT1, str);
		if (str== cstr2&&g_Thread>0)
		{
			break;
		}
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)miscres1_1, cstr2, 0, 0);
		//VirtualFree((LPVOID)lParam, wParam+100, MEM_COMMIT);
		break;
	}
	default:
		break;
	}



	return CDialogEx::WindowProc(message, wParam, lParam);
}

char szPath[MAX_PATH] = { 0 };
/*拖放歌曲*/
void CMusicplayDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//获取文件路径
	
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);
	//显示到控件上

	int a = 0;
	a= DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);//从成功的拖放操作中检索文件的名称，获取拖拽文件的数目
	
	CString str;
	GetDlgItemText(EDIT1, str);
	if (str == szPath && g_Thread > 0)
	{
		return;
	}
	
	if (a == 1)
	{
		SetDlgItemText(EDIT1, szPath);
		
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)miscres1_1, szPath, 0, 0);
		return;
	}
	else if (a > 1)
	{
		//MessageBox(_T("不支持多个文件！"), _T("提示"));
		SetDlgItemText(EDIT1, szPath);
		
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)miscres1_1, szPath, 0, 0);
		return;
	}
	
	//原文链接：https ://blog.csdn.net/qq_41490873/article/details/108302055
	CDialogEx::OnDropFiles(hDropInfo);
}


void CMusicplayDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (g_Thread>0)
	{
		g_Close = TRUE;
		return;
	}
	CDialogEx::OnClose();
}
