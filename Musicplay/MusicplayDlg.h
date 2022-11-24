
// MusicplayDlg.h: 头文件
//

#pragma once
#include "myDialog.h"
#include "MySlider.h"
#include "MicPalay.h"
#include "MyListCtrl.h"

// CMusicplayDlg 对话框
class CMusicplayDlg : public CDialogEx
{
// 构造

public:
	CMusicplayDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CMusicplayDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUSICPLAY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	BOOL m_close = FALSE;  //关闭退出 1
	BOOL m_minimize = FALSE;//最小化 2
	BOOL m_movewin = FALSE;// 拖动窗口3
	BOOL m_Last = FALSE;   // 上一首4
	BOOL m_Next = FALSE;	//下一首5
	BOOL m_play = TRUE;	// 播放按钮是否按下
	BOOL m_susplay= FALSE;	//播放暂停按钮开关 6
	BOOL m_Redraw = FALSE;
	BOOL m_lider_bnt = FALSE;
	BOOL m_lider_bnt_up = FALSE;
	int m_BUTTONDX = 0;
	MicPalay MP3;
	MySlider   m_lider;
	int        m_listdx = 0;
	BOOL        m_modelColor =FALSE;  //795,606,854,646,
	void ToTray();//托盘图标
	DWORD m_std;  //隐藏窗口任务栏
	NOTIFYICONDATA m_nid;//是用于设置托盘图标的的结构体
	CRect   m_winrc = {0};
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void mp3paly_(int dx);
	void SetMp3Pos(int pos);
	int GetMp3Pos();
	void miscres1(char* str);

	afx_msg void OnBnClickedButton1();
	afx_msg BOOL OnQueryOpen();
	CMyListCtrl m_list;
	void Rwlist();
	void SetListColor(int dx);
	
	afx_msg void OnDropFiles(HDROP hDropInfo);
	
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);

	
	afx_msg void OnPaly();
	afx_msg void OnDeleteList();
	afx_msg void OnClearList();
	afx_msg void OnCloselist();
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void On32776();
	afx_msg void On32777();
	afx_msg void On32778();
	afx_msg void On32779();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void On32780();
	afx_msg void On32784();
	afx_msg void On32781();
	afx_msg void On32782();
	afx_msg void On32783();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
