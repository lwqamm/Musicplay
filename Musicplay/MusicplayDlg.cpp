
// MusicplayDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Musicplay.h"
#include "MusicplayDlg.h"
#include "afxdialogex.h"
#include "IniPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMusicplayDlg* g_this = 0;
HWND g_hwnd=0;
HICON g_hIcon;
BOOL   Paint = FALSE;//是否在画图
CDC* g_dc = NULL;

// CMusicplayDlg 对话框

	//normal color
#define COLOR_DEFAULT 0 //默认颜色
#define COLOR_RED 1 //红色
#define COLOR_BLUE 2 //蓝色
#define ID_HOTKEY  3

CMusicplayDlg::CMusicplayDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MUSICPLAY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMusicplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	
}

BEGIN_MESSAGE_MAP(CMusicplayDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON1, &CMusicplayDlg::OnBnClickedButton1)
	ON_WM_QUERYOPEN()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, &CMusicplayDlg::OnBnClickedButton2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMusicplayDlg::OnNMDblclkList1)
	ON_COMMAND(ID_32772, &CMusicplayDlg::OnPaly)
	ON_COMMAND(ID_32773, &CMusicplayDlg::OnDeleteList)
	ON_COMMAND(ID_32774, &CMusicplayDlg::OnClearList)
	ON_COMMAND(ID_32775, &CMusicplayDlg::OnCloselist)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMusicplayDlg::OnNMRClickList1)
	ON_WM_HOTKEY(WM_HOTKEY, OnHotKey)
	//ON_WM_HOTKEY(&CMusicplayDlg::OnHotKey)
	ON_COMMAND(ID_32776, &CMusicplayDlg::On32776)
	ON_COMMAND(ID_32777, &CMusicplayDlg::On32777)
	ON_COMMAND(ID_32778, &CMusicplayDlg::On32778)
	ON_COMMAND(ID_32779, &CMusicplayDlg::On32779)
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CMusicplayDlg 消息处理程序

BOOL CMusicplayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码0xf0, 0xf0, 0xf0
	if (RegisterHotKey(m_hWnd, ID_HOTKEY, NULL, 0x20))
	
	//return TRUE;  // return TRUE unless you
	
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT /*| LVS_EX_GRIDLINES*/);
	m_list.InsertColumn(0, "", LVCFMT_LEFT,450);
	m_list.SetBkColor(RGB(0xf0, 0xf0, 0xf0));
	m_list.SetTextBkColor(RGB(0xf0, 0xf0, 0xf0));
	m_list.SetTextColor(RGB(0x33, 0x33, 0x33));
	RegisterHotKey(m_hWnd, ID_HOTKEY, 32,0);
	CString str;
	AfxSetWindowText(m_hWnd, "本地音乐播放器 2.0");//设置标题
	INIPATH(0);
	Rwlist();
	str.Format("%d", m_hWnd);
	Write_config(str,"窗口句柄");


	str = "";
	g_this = this;
	g_hwnd = m_hWnd;
	g_hIcon = m_hIcon;
	MP3.SetHwnd(m_hWnd);
	g_dc = GetDC();
	m_lider.Create(m_hWnd, GetDC(), RGB(0xf0, 0xf0, 0xf0), 355, 650);
	m_lider.SetBack(RGB(0xCE, 0xCE, 0xCE));
	m_lider.SetSpe(RGB(0xff, 0x4E, 0x4E));//ff4e4e
	m_lider.SetSize(3);
	m_lider.SetLen(400);
	
	CString commandLine = AfxGetApp()->m_lpCmdLine;

	for (size_t i = 0; i < commandLine.GetLength(); i++)
	{
		if (commandLine.Mid(i, 1) == '"')
		{
			continue;
		}
	str = str + commandLine.Mid(i, 1);	
	}
	//AfxMessageBox(str);
	if (str=="")
	{
		m_lider.SetPos(0);
		return TRUE;
	}
	MP3.paly(str.GetBuffer(),0);
	//Rwlist();
	m_play = FALSE;
	SetPaly(GetDC(), 447, 605, 2, 1, 2);
	m_lider.SetPos(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。


void CMusicplayDlg::OnPaint()
{
	Paint = TRUE;
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
		CDC* dc = GetDC();
		
		
	   // m_Verbar.SetPos(100);
		m_lider.SetPos(m_lider.GetPos());
		if (m_play == TRUE)
		{
			SetPaly(dc, 447, 605, 2, 0, 1);
		}
		else
		{
			SetPaly(dc, 447, 605, 2, 0, 2);
		}
		
		CDialogEx::OnPaint();
		
	}
	myNewdraw(GetDC());
	MP3.StretchBlt();
	MP3.Stretchmodel();
	Paint = FALSE;
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMusicplayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMusicplayDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_close&&!m_minimize&& m_movewin&&!m_Next
		&& !m_Last)
	{
		PostMessage(WM_NCLBUTTONDOWN,
			HTCAPTION,
			MAKELPARAM(point.x, point.y));  //或SendMessage(WM_SYSCOMMAND,0xF012,0);   //0xF012 = SC_MOVE | HTCAPTION
	}
	if (m_lider_bnt==TRUE)
	{
		m_lider_bnt_up = TRUE;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMusicplayDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

		int x = point.x, y = point.y;
		POINT brxy = m_lider.GetBraxy();
		if (x>=brxy.x&&x<=m_lider.GetLen()+ brxy.x&&y>=brxy.y-2&&y<=brxy.y+m_lider.GetSize()+2
			 )
		{
			m_lider_bnt = TRUE;
			return ;
		}
		m_lider_bnt = FALSE;
		if (m_lider_bnt_up == TRUE)
		{
			m_lider_bnt = FALSE;
			return ;
		}

		if (x >= 927 && x <= 959 && y >= 15 && y <= 37)//m_close
		{//927,15,959,37,
			if (m_BUTTONDX == 1)
			{
				return;
			}
			myRectangle(GetDC(), 979, 16, 1, 1);
			m_minimize = TRUE;
			m_close = FALSE;
			m_movewin = FALSE;
			m_susplay = FALSE;
			m_Next = FALSE;
			m_Last = FALSE;
			m_Redraw = TRUE;
			m_BUTTONDX = 1;
			return;
		}
		else if (x >= 968 && x <= 1001 && y >= 13 && y <= 33)//m_minimize=2
		{//968,13,1001,33
			if (m_BUTTONDX == 2)
			{
				return;
			}
			myRectangle(GetDC(), 979, 16, 2, 1);
			m_close = TRUE;
			m_minimize = FALSE;//0,0,1020,61
			m_movewin = FALSE;
			m_susplay = FALSE;
			m_Next = FALSE;
			m_Last = FALSE;
			m_Redraw = TRUE;
			m_BUTTONDX = 2;
			return;
		}
		else if (x >= 0 && x <= 1020 && y >= 0 && y <= 60 )//m_movewin =3
		{//968,13,1001,33
			if (m_BUTTONDX == 3)
			{
				return;
			}
			myRectangle(GetDC(), 979, 16, 0, 0);
			m_close = FALSE;
			m_minimize = FALSE;//0,0,1020,61  拖动窗口
			m_movewin = TRUE;
			m_susplay = FALSE;
			m_Next = FALSE;
			m_Last = FALSE;
			m_Redraw = TRUE;
			m_BUTTONDX = 3;
			return;
		}
		else if (x >= 442 && x <= 476 && y >= 600 && y <= 632)//m_Last =4
		{
			//上一首     442,610,476,642 
			if (m_BUTTONDX == 4)
			{
				return;
			}
			if (m_play == TRUE)
			{
				SetPaly(GetDC(), 447, 605, 1, 1, 1);
			}
			else
			{
				SetPaly(GetDC(), 447, 605, 1, 1, 2);

			}

			m_close = FALSE;
			m_minimize = FALSE;//0,0,1020,61
			m_movewin = FALSE;
			m_Next = FALSE;
			m_susplay = FALSE;
			m_Last = TRUE;
			m_Redraw = TRUE;
			m_BUTTONDX = 4;
			return;
		}
		else if (x >= 483 && x <= 525 && y >= 600 && y <= 635)//m_susplay =6
		{
			//播放       483,610,525,645 
			if (m_BUTTONDX == 6)
			{
				return;
			}
			if (m_play == TRUE)
			{
				SetPaly(GetDC(), 447, 605, 2, 1, 1);
			}
			else 
			{
				SetPaly(GetDC(), 447, 605, 2, 1, 2);		
			}
			m_close = FALSE;
			m_minimize = FALSE;//0,0,1020,61
			m_movewin = FALSE;
			m_Last = FALSE;
			m_Next = FALSE;
			m_susplay = TRUE;
			m_Redraw = TRUE;
			m_BUTTONDX = 6;
			return;
		}
		else if (x >= 530 && x <= 573 && y >= 598 && y <= 635)//m_Next=5
		{
			//下一首     530,608,573,645
			if (m_BUTTONDX == 5)
			{
				return;
			}
			if (m_play == TRUE)
			{
				SetPaly(GetDC(), 447, 605, 3, 1, 1);
			}
			else 
			{
				SetPaly(GetDC(), 447, 605, 3, 1, 2);
			}
			m_close = FALSE;
			m_minimize = FALSE;//0,0,1020,61
			m_movewin = FALSE;
			m_susplay = FALSE;
			m_Next = TRUE;
			m_Last = FALSE;
			m_Redraw = TRUE;
			m_BUTTONDX = 6;
			return;
		}
		else if (x >= 795 && x <= 854 && y >= 606 && y <= 646)//模式设置
		{

		  m_modelColor = TRUE;  //795,606,854,646,
		  MP3.SetModelColor(TRUE);

		}
		else
		{
		m_modelColor = FALSE;  //795,606,854,646,
		MP3.SetModelColor(FALSE);
		}
	
	
	if (m_Redraw == TRUE)
	{
		if (m_play == TRUE)
		{
			SetPaly(GetDC(), 447, 605, 0, 0,1);
		}
		else 
		{
			SetPaly(GetDC(), 447, 605, 0, 0,2);
		}
		
		myRectangle(GetDC(), 979, 16, 0, 0);
		m_Redraw = FALSE;
	}

	m_BUTTONDX = 0;
	m_close = FALSE;
	m_minimize = FALSE;
	m_movewin = FALSE;
	m_susplay = FALSE;
	m_Next = FALSE;
	m_Last = FALSE;

	CDialogEx::OnMouseMove(nFlags, point);
}

void CMusicplayDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (m_lider_bnt_up==TRUE)
	{
		m_lider_bnt_up = FALSE;
		m_lider_bnt = FALSE;
		return;
	}

	if (m_close == TRUE)
	{
		if (MP3.Getpause()==2|| MP3.Getpause() == 1)
		{
			UnregisterHotKey(GetSafeHwnd(), ID_HOTKEY);//注销热键
			MP3.close();
			return;
		}
		DeleteFile(INIPATH(2).GetBuffer());
		UnregisterHotKey(GetSafeHwnd(), ID_HOTKEY);//注销热键
		::PostMessage(m_hWnd, WM_QUIT, 0, 0);//关闭程序
		return;
	}
	if (m_minimize == TRUE)
	{
	   //PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); //最小化
		if (MP3.Getpause() == 2 || MP3.Getpause() == 1)
		{
			MP3.Setmini(TRUE);
		}
		
		
		ShowWindow(SW_MINIMIZE);
		return;
	}

	if (m_susplay==TRUE)//播放暂停
	{
		
		CDC* dc = GetDC();
		if (m_play==TRUE)
		{
			
			m_play = FALSE;
			SetPaly(dc, 447, 605, 2, 1, 2);
			if (MP3.Getpause()==2)
			{
				MP3.resume();
			}
			else
			{
				MP3.paly("",0);
			}
			
		}
		else 
		{
			
			m_play = TRUE;
			SetPaly(dc, 447, 605, 2, 1, 1);
			MP3.pause();

		}
		return;
	}

	if (m_Last==TRUE)
	{
		m_Last = FALSE;
		MP3.Previous();
		return;
	}
	if (m_Next == TRUE)
	{
		m_Next = FALSE;
		MP3.Next();
		return;
	}

	if (m_modelColor)
	{
		MP3.SetModel(-1);
		//MP3.SetModelColor();
		return;
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

VOID WINAPI miscres1_1(char* str1)
{
	g_this->miscres1(str1);
}

int mp3strl = 0;
void CMusicplayDlg::miscres1(char* str)
{
	if (strlen(str) <= 6)
	{
		return;
	}

	//if (MP3.Getpause() > 0)
	//{
	//	
	//	HANDLE g_Thread_1 = MP3.GetThread();
	//	
	//	WaitForSingleObject(g_Thread_1, INFINITE);
	//	m_lider.SetPos(0);
	//	//Write_config("0", "进度条进度");
	//}
	MP3.stop();
	MP3.paly(str, mp3strl);
	Rwlist();
	m_play = FALSE;
	SetPaly(GetDC(), 447, 605, 2, 0, 2);
}


LRESULT CMusicplayDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	switch (message)
	{
	case TTMSG_WARNLIST1:
	{

		char* cstr2 = (char*)lParam;
	
		char* str = MP3.GetMp3Path();
		if (strcmp(cstr2,str)==0)
		{
			break;
		}
		mp3strl = wParam;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)miscres1_1, cstr2, 0, 0);
		//VirtualFree((LPVOID)lParam, wParam+100, MEM_COMMIT);
		return CDialogEx::WindowProc(message, wParam, lParam);
	}
	default:
		break;
	}

	if (message==WM_LBUTTONUP|| message == WM_MOUSEMOVE|| message == WM_LBUTTONDOWN)
	{
		m_lider.WindowProc(message, wParam, lParam);
	}
		

	
	return CDialogEx::WindowProc(message, wParam, lParam);
}

void mp3paly(int dx)
{
	g_this->mp3paly_(dx);
}
void CMusicplayDlg::mp3paly_(int dx)
{
	CDC* dc = GetDC();
	
	switch (dx)
	{
	case 1:
	{
		m_play = TRUE;
		break;
	}
	case 2:
	{
		m_play = FALSE;
		break;
	}
	default:
		break;
	}
	
	SetPaly(dc, 447, 605, 2, 0, dx);
}

int Mp3Pos(int dx,int pos)
{
	switch (dx)
	{
	case 0:
	{
		g_this->SetMp3Pos(pos);
		return 0;
	}
	case 1:
	{
		return	g_this->GetMp3Pos();
	}
	default:
		break;
	}
	return 0;
}

void CMusicplayDlg::SetMp3Pos(int pos)
{
	m_lider.SetPos(pos);
}

int CMusicplayDlg::GetMp3Pos()
{
	return m_lider.GetPos();
}




BOOL CMusicplayDlg::OnQueryOpen()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MP3.Setmini(FALSE);
	//myNewdraw(GetDC());
	return CDialogEx::OnQueryOpen();
}

void Rwlist1()
{
	g_this->Rwlist();
	
}
void CMusicplayDlg::Rwlist()
{
	m_list.DeleteAllItems();
	CString str = "";
	int cons = MP3.GetListSize();
	for (int i = 0; i < cons; i++)
	{
		/*	if (i==15)
			{
				i = i;
			}*/
		str = MP3.Getdxliststr(i);
		if (str != "未知歌曲")
		{
			m_list.InsertItem(i, str);
		}

	}


	
	
}

void CMusicplayDlg::SetListColor(int dx)
{

}

void SetListColor1(int dx,int ret)
{
	g_this->m_list.SetListdxColor(dx,ret);
}

void OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	//CLinkItem* pLink = NULL;
	*pResult = 0;
	int nItem = -111;
	int nSubItem = -111;
	COLORREF clrNewTextColor, clrNewBkColor;
	//Switch between different draw stages
	switch (pLVCD->nmcd.dwDrawStage) {

	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:

		nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);  //获取当前重绘行索引
		nSubItem = static_cast<int>(pLVCD->iSubItem);  //获取当前重绘列索引


		if (nItem == 0)   //如果重绘行就是选中的单元格所在的行
		{
			clrNewTextColor = RGB(0, 0, 0);		//设置文本颜色 
			clrNewBkColor = RGB(100, 149, 237); //设置背景颜色
			pLVCD->clrText = clrNewTextColor;
			pLVCD->clrTextBk = clrNewBkColor;
		}
		else if (nSubItem == 1)   //如果重绘列是选中的单元格所在的列
		{
			clrNewTextColor = RGB(0, 0, 0);
			clrNewBkColor = RGB(230, 230, 250);
			pLVCD->clrText = clrNewTextColor;
			pLVCD->clrTextBk = clrNewBkColor;
		}
		else if (nSubItem > 2)    //只显示一列，如果没有这个判断就会显示后面所有列
		{
			clrNewTextColor = RGB(0, 0, 0);
			clrNewBkColor = RGB(255, 255, 255);
			pLVCD->clrText = clrNewTextColor;
			pLVCD->clrTextBk = clrNewBkColor;
		}
		*pResult = CDRF_DODEFAULT;
		break;

	default:
		break;

	}

	//原文链接：https ://blog.csdn.net/qq_28114615/article/details/93889138
  //原文链接：https ://blog.csdn.net/langxm2006/article/details/76736153
}

void CMusicplayDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	int count;
	TCHAR filePath[MAX_PATH] = {0};//必须用宽字符
	count = DragQueryFile(hDropInfo, -1, NULL, 0);//获取拖拽文件总数
		for (int i = 0; i < count; i++)//每个文件都显示文件路径
		{
			memset(filePath, 0, sizeof(filePath));
			DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));
			if (MP3.myGetExtension(filePath))
			{
				MP3.AddList(filePath);
			}
			
		}
	
	m_list.DeleteAllItems();
	CString str = "";
	int cons = MP3.GetListSize();
	for (int i = 0; i < cons; i++)
	{
		str = MP3.Getdxliststr(i);
		m_list.InsertItem(i, str);
	}
   //原文链接：https ://blog.csdn.net/weixin_42845306/article/details/113248911
	CDialogEx::OnDropFiles(hDropInfo);
}

void CMusicplayDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_list.SetItemData(0, COLOR_DEFAULT);
	//m_list.SetListdxColor(3);
	//m_list.SetItemData(3, 2);

	MP3.Stretchmodel();

}

void CMusicplayDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_list.SetListdxColor(3,FALSE);
	
}


void CMusicplayDlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CListCtrl* p = (CListCtrl*)GetDlgItem(IDC_LIST1);
	CString str;

	int row;
	//首先得到点击的位置
	POSITION pos = p->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		//MessageBox("请至少选择一项", "提示", MB_ICONEXCLAMATION);
		return;
	}
	//得到行号，通过POSITION转化
	row = (int)p->GetNextSelectedItem(pos);
	if (MP3.GetMp3dx() == row || row == -1)//鼠标双击事件
	{
		return;
	}
	str = MP3.Getdxlist(row);
	if (MP3.GetMp3dx() >= 0)
	{
		MP3.stop();
	}
	MP3.paly(str.GetBuffer(), str.GetLength());

	*pResult = 0;
}


void CMusicplayDlg::OnPaly()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("播放");
	CListCtrl* p = (CListCtrl*)GetDlgItem(IDC_LIST1);
	CString str;

	int row;
	//首先得到点击的位置
	POSITION pos = p->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		//MessageBox("请至少选择一项", "提示", MB_ICONEXCLAMATION);
		return;
	}
	//得到行号，通过POSITION转化
	row = (int)p->GetNextSelectedItem(pos);
	if (MP3.GetMp3dx() == row || row == -1)//鼠标双击事件
	{
		return;
	}
    str = MP3.Getdxlist(row);
	if (MP3.GetMp3dx()>=0)
	{
		MP3.stop();
	}
	
	MP3.paly(str.GetBuffer(), str.GetLength());

	return;
}


void CMusicplayDlg::OnDeleteList()
{
	// TODO: 在此添加命令处理程序代码
	CListCtrl* p = (CListCtrl*)GetDlgItem(IDC_LIST1);


	int row;
	//首先得到点击的位置
	POSITION pos = p->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		//MessageBox("请至少选择一项", "提示", MB_ICONEXCLAMATION);
		return;
	}
	//得到行号，通过POSITION转化
	row = (int)p->GetNextSelectedItem(pos);
	if (MP3.GetMp3dx() == row || row == -1)//鼠标双击事件
	{
		return;
	}
	MP3.DeleteList(row);
}


void CMusicplayDlg::OnClearList()
{
	// TODO: 在此添加命令处理程序代码
  MP3.ClearList();

}


void CMusicplayDlg::OnCloselist()
{
	// TODO: 在此添加命令处理程序代码
	if (MP3.GetMp3dx() != -1)
	{
		MP3.close();
	}
	exit(0);
}


void CMusicplayDlg::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int dx = pNMItemActivate->iItem;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//添加线程操作
		VERIFY(menu.LoadMenu(IDR_MENU1));			//这里是我们在1中定义的MENU的文件名称
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	*pResult = 0;
}





void CMusicplayDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nHotKeyId == ID_HOTKEY)
		//MessageBox("快捷键F2");
		if (nKey2==0x20)
		{
			if ((m_hWnd == ::GetForegroundWindow()))
			{
				//MessageBox("窗口在最顶层");//... // The window is topmost.
				CDC* dc = GetDC();
				if (m_play == TRUE)
				{

					m_play = FALSE;
					SetPaly(dc, 447, 605, 2, 1, 2);
					if (MP3.Getpause() == 2)
					{
						MP3.resume();
					}
					else
					{
						MP3.paly("", 0);
					}

				}
				else
				{

					m_play = TRUE;
					SetPaly(dc, 447, 605, 2, 1, 1);
					MP3.pause();

				}
			}
		}



	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CMusicplayDlg::On32776()//顺序播放
{
	// TODO: 在此添加命令处理程序代码
	MP3.SetModel(0);
}


void CMusicplayDlg::On32777()//随机播放
{
	// TODO: 在此添加命令处理程序代码
	MP3.SetModel(1);
}


void CMusicplayDlg::On32778()//单曲循环
{
	// TODO: 在此添加命令处理程序代码
	MP3.SetModel(2);
}


void CMusicplayDlg::On32779()//单曲播放
{
	// TODO: 在此添加命令处理程序代码
	MP3.SetModel(3);
}


void CMusicplayDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_modelColor)
	{
		CMenu menu;
		//添加线程操作
		VERIFY(menu.LoadMenu(IDR_MENU2));			//这里是我们在1中定义的MENU的文件名称
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x+470, point.y+75, this);
	}

	CDialogEx::OnRButtonUp(nFlags, point);
}
