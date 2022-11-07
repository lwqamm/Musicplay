
// Music playDlg.h: 头文件
//

#pragma once
#include "mySlider.h"
#include "IniPath.h"
#include "Stream.h"


// CMusicplayDlg 对话框
class CMusicplayDlg : public CDialogEx
{
// 构造
public:
	CMusicplayDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUSIC_PLAY_DIALOG };
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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	
	MySlider m_proc1;
	CBrush m_brush;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton5();
	void miscres(int lx);
	void miscres1(char* str);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	
	afx_msg void OnClose();

	BOOL ShowImage(CDC* pDC, CString strPath, int x, int y);
	CListCtrl m_list1;
	CListCtrl m_list2;
	//CImage m_image;
	myimage m_image;

	
};
