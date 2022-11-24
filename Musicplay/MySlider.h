#pragma once
#include <afxcmn.h>
#include <afx.h>
class MySlider
{
public:
	MySlider();
	~MySlider();
public:
	//获取滑块位置返回百分比
	int GetPos() const;
	//设置滑块位置百分比
	void SetPos( int nPos);

	//设置滑块背景颜色
	void SetBack(COLORREF color);
	//设置进度条颜色
	void SetSpe(COLORREF color);
	//设置进度条宽窄
	void SetSize(int size);
	//返回进度条宽窄
	int GetSize();
	//返回进度条宽窄
	POINT GetBraxy();
	//设置进度条长度必须是100的倍数
	void SetLen(int len);
	//返回进度条长度
	int GetLen();
	//创建进度条对象必须指定CDC设备和窗口背景颜色
	BOOL Create(HWND hwnd, CDC* dc, COLORREF color,int x,int y );
	void WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	typedef struct _MYRECT
	{
		int x = 0;
		int y = 0;
		int x1 = 0;
		int y1 = 0;
	}MYRECT, * PMyRECT;
	//画进度条
	BOOL myPolygon(CDC* dc,const POINT* lpPoints, int nCount, COLORREF crColor);
     //画滑块
	BOOL ellipse(CDC* dc);
	void OnPaint(CDC* dc);
	void OnLButtonUp(CPoint point);
	void OnLButtonDown(CPoint point);
	void OnMouseMove(CPoint point);
	//重画
	void myRedraw(CDC* dc);
	//计算进度坐标
	void SetPosxy();
	//获取桌面宽度
	void GetWindowRect();
private:	
	int m_Slider_len; //进度条长度必须是100的倍数
	int m_Slider_size ; //进度条大小 宽度相数
	BOOL m_size = FALSE;//是否增加了
	int m_Slider_pos ;   //进度条位置
	COLORREF m_Slider_back;//进度条背景颜色
	COLORREF m_windows_back;//窗口背景颜色
	COLORREF m_Slider_spe;  //进度条颜色
	CDC* m_dc;    //绘制进度条的设备
	CRect rc;
	MYRECT m_pos_rect = { 0 }; //进度条pos
	POINT  m_Slider_xy;        //进度条起始位置
	HWND m_hWnd;              //主窗口句柄
	BOOL m_Slider = FALSE; //是否刷新背景
	BOOL m_back = FALSE;   //是否画出了滑块
	BOOL m_Slider_btnDwn = FALSE; //鼠标左键按下
	BOOL m_Slider_braDwn = FALSE; //按下的是否为进度条区域
	BOOL m_Slider_posbtn = FALSE;
	int   m_width = 0;
	int   m_height = 0;
	BOOL  m_bar = FALSE;         //第一次
};


