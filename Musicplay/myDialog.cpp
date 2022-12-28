#include "pch.h"
#include "myDialog.h"
#include <windef.h>
void myRedraw(CDC* dc);//刷新播放按钮
#define  MPALY       1
#define  MSUSPEND     0  
int    g_playsus = MPALY;
extern HICON g_hIcon;
extern HWND g_hwnd ;
int   g_width = 0;   //电脑屏幕总宽度
int   g_height = 0;  //电脑屏幕总高度
extern BOOL   Paint;          //是否在画图
void Getposwh()
{
	int no_menu_bar_width = GetSystemMetrics(SM_CXFULLSCREEN);
	int no_menu_bar__height = GetSystemMetrics(SM_CYFULLSCREEN);
	g_width = no_menu_bar_width;
	g_height = no_menu_bar__height;

}

int GetDcColor(int* x1, int* y1, int w, int h)
{
	int x = *(int*)x1,  y = *(int*)y1;
	x = x + (w / 2);
	y = y + (h / 2);
	*(int*)x1 = x;
	*(int*)y1 = y;
	return 1;
}


BOOL myPolygon(CDC* dc, const POINT* lpPoints, int nCount, COLORREF crColor,POINT* pxy)
{
	if (nCount <= 0)
	{
		return FALSE;
	}

	for (int i = 0; i < nCount; i++)
	{
		if (lpPoints[i].x<0|| lpPoints[i].y<0)
		{
			return FALSE;
		}
	}
	int x = 0, y = 0;
	POINT pt = { 0 };
	CBrush brush;
	CPen   pen;
	CBrush* obbrush = 0;
	CPen* obpen = 0;

		pen.CreatePen(PS_SOLID, 1, crColor);
		brush.CreateSolidBrush(crColor);//红色画刷35c1ff
		obbrush = dc->SelectObject(&brush);
		obpen = dc->SelectObject(&pen);

	for (int i = 0; i < nCount; i++)
	{
		if (i == 0)
		{
			pt = { lpPoints[i].x,lpPoints[i].y };
			dc->MoveTo(pt);
		}
		dc->LineTo(lpPoints[i].x, lpPoints[i].y);
	}
	dc->LineTo(lpPoints[0].x, lpPoints[0].y);

	if (pxy==0)
	{
		x = lpPoints[0].x;
		y = lpPoints[0].y;
		if (GetDcColor(&x, &y, lpPoints[1].x - lpPoints[0].x, lpPoints[2].y - lpPoints[1].y) == -1)
		{
			return FALSE;
		}
	}
	else
	{
		x = pxy->x;
		y = pxy->y;
	}
	if (crColor > 0)
	{

		dc->FloodFill(x, y, crColor); //填充区域
		dc->SelectObject(obbrush);
		dc->SelectObject(obpen);
		::DeleteObject(obpen);
		::DeleteObject(obbrush);
		return TRUE;
	}
	dc->FloodFill(x, y, dc->GetBkColor()); //填充区域
	dc->SelectObject(obbrush);
	dc->SelectObject(obpen);
	::DeleteObject(brush);
	::DeleteObject(pen);
	return TRUE;
}

BOOL myPolygon_1(CDC* dc, const POINT* lpPoints, int nCount, COLORREF crColor, POINT* pxy)
{
	if (nCount <= 0)
	{
		return FALSE;
	}
	for (int i = 0; i < nCount; i++)
	{
		if (lpPoints[i].x < 0 || lpPoints[i].y < 0)
		{
			return FALSE;
		}
	}
	int x = 0, y = 0;
	POINT pt = { 0 };
	CBrush brush;
	CPen   pen;
	CBrush* obbrush = 0;
	CPen* obpen = 0;

	pen.CreatePen(PS_SOLID, 1, crColor);
	brush.CreateSolidBrush(crColor);//红色画刷35c1ff
	obbrush = dc->SelectObject(&brush);
	obpen = dc->SelectObject(&pen);

	for (int i = 0; i < nCount; i++)
	{
		if (i == 0)
		{
			pt = { lpPoints[i].x,lpPoints[i].y };
			dc->MoveTo(pt);
		}
		dc->LineTo(lpPoints[i].x, lpPoints[i].y);
	}
	dc->LineTo(lpPoints[0].x, lpPoints[0].y);

	if (pxy == 0)
	{
		x = lpPoints[0].x;
		y = lpPoints[0].y;
		if (GetDcColor(&x, &y, lpPoints[1].x - lpPoints[0].x, lpPoints[2].y - lpPoints[1].y) == -1)
		{
			return FALSE;
		}
	}
	else
	{
		x = pxy->x;
		y = pxy->y;
	}
	if (crColor > 0)
	{

		dc->FloodFill(x, y, crColor); //填充区域
		dc->SelectObject(obbrush);
		dc->SelectObject(obpen);
		::DeleteObject(obpen);
		::DeleteObject(obbrush);
		return TRUE;
	}
	dc->FloodFill(x, y, dc->GetBkColor()); //填充区域
	dc->SelectObject(obbrush);
	dc->SelectObject(obpen);
	::DeleteObject(brush);
	::DeleteObject(pen);
	return TRUE;
}
extern  HWND g_hwnd;
void SetPaly(CDC* pDC, int x, int y, int btn, int dx,int paly)
{
	Paint = TRUE;
	CRect rc;
	CDC dcMem;
	GetClientRect(g_hwnd,&rc);
	CBitmap bmp;  //内存中承载临时图象的位图


	dcMem.CreateCompatibleDC(pDC);  //依附窗口DC创建兼容内存DC
	//创建兼容位图(必须用pDC创建，否则画出的图形变成黑色)
	bmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	CBitmap* pOldBit = dcMem.SelectObject(&bmp);
	//按原来背景填充客户区，不然会是黑色
	dcMem.FillSolidRect(rc, RGB(240, 240, 240));


	COLORREF cor1 = RGB(0x35, 0xc1, 0xff),cor0=RGB(0x10, 0x86, 255);
	CPoint pt[8] = { 0 };

	POINT pin = {0};
	/// 上一曲
	pt[0] = { x, y }; //x91, y90    
	pt[1] = { x + 3,y };
	pt[2] = { x + 3,y + 21 };
	pt[3] = { x,y + 21 };
	pt[4] = { x + 5, y + 9 }; //100, 100
	pt[5] = { x + 21,y };   //120,90
	pt[6] = { x + 21,y + 18 };  //120,113
	if (btn == 1 && dx == 1)
	{
		pin.x = 448;
		pin.y = 612;
	if (!myPolygon_1(&dcMem, pt, 4, cor1, &pin))
	{
		return;
	}
		;//0x35, 0xC1, 255
		pin.x = 462;
		pin.y = 612;
		if(!myPolygon_1(&dcMem, pt+4, 3, cor1, &pin))
		{
			return;
		}
	}
	else
	{
		if(!myPolygon_1(&dcMem, pt, 4, cor0,0))//0x35, 0xC1, 255
		{
			return;
		}
		pin.x = 462;
		pin.y = 612;
		if (!myPolygon_1(&dcMem, pt + 4, 3, cor0, &pin))
		{
			return;
		}
	}
	////////////////////
	pin.x = 499;
	pin.y = 613;
	//////////////////////////////////
	//播放 暂停
	//myRedraw(dc);
	if (paly==1)
	{
		if (g_playsus==MSUSPEND)
		{
			myRedraw(&dcMem);
		}
		pt[0] = { x + 68, y + 10 }; //35  
		pt[1] = { x + 49, y - 3 }; //54
		pt[2] = { x + 45, y - 3 }; //58
		pt[3] = { x + 45, y + 24 }; //58
		pt[4] = { x + 50, y + 24 }; //55
		pt[5] = { x + 68,y + 13 };  //35		
		if (btn == 2 && dx == 1)
		{

			if(!myPolygon_1(&dcMem, pt, 6, cor1, &pin)) //e5f5fb
			{return; }
			
		}
		else
		{
			if (!myPolygon_1(&dcMem, pt, 6, cor0, &pin)) //e5f5fb
			{return; }
		}
		g_playsus = MPALY;
	}
	else if (paly == 2)
	{
		if (g_playsus == MPALY)
		{
			myRedraw(&dcMem);
		}
		pt[0] = { x+48, y }; //x91, y90    
		pt[1] = { x + 3+48,y };
		pt[2] = { x + 3+48,y + 21 };
		pt[3] = { x+48,y + 21 };
		pt[4] = { x+58, y }; //x91, y90    
		pt[5] = { x + 3+58,y };
		pt[6] = { x + 3+58,y + 21 };
		pt[7] = { x+58,y + 21 };
		if (btn == 2 && dx == 1)
		{
			pin.x = 497;
			pin.y = 614;
			if (!myPolygon_1(&dcMem, pt, 4, cor1, &pin))//0x35, 0xC1, 255
			{return; }
			pin.x = 507;
			pin.y = 615;
			if (!myPolygon_1(&dcMem, pt + 4, 4, cor1, &pin))
			{return; }
		}
		else
		{
			pin.x = 497;
			pin.y = 614;
			if (!myPolygon_1(&dcMem, pt, 4, cor0, &pin))//0x35, 0xC1, 255
			{return; }
			pin.x = 507;
			pin.y = 615;
			if (!myPolygon_1(&dcMem, pt + 4, 4, cor0,&pin))
			{return; }
		}
		g_playsus = MSUSPEND;
	}

	/// ////////////////////////////////

	//////////////////////////////////
	//下一曲
	pt[0] = { x + 112, y }; //x91, y90    
	pt[1] = { x + 3 + 112,y };
	pt[3] = { x + 112,y + 21 };
	pt[2] = { x + 3 + 112,y + 21 };
	pt[4] = { x + 90 + 20, y + 9 }; //100, 100
	pt[5] = { x + 90,y };   //120,90
	pt[6] = { x + 90,y + 18 };  //120,113
	if (btn == 3 && dx == 1)
	{
		pin.x = 543;
		pin.y = 614;
		if (!myPolygon_1(&dcMem, pt+4, 3, cor1, &pin))
		{return; }
		pin.x = 561;
		pin.y = 612;
		if (!myPolygon_1(&dcMem, pt, 4, cor1, &pin))//0x35, 0xC1, 255
		{return; }
	}
	else
	{
		pin.x = 543;
		pin.y = 614;
		if (!myPolygon_1(&dcMem, pt+4, 3, cor0,&pin))
		{return; }
		pin.x = 561;
		pin.y = 612;
		if(!myPolygon_1(&dcMem, pt, 4, cor0, &pin))//0x35, 0xC1, 255
		{return; }
	}
	///////////////////////////////

	pDC->StretchBlt(441, 596, 136, 39, &dcMem, 441, 596, 136, 39, SRCCOPY);
	//绘图完成后的清理
	dcMem.DeleteDC();      //删除DC
	bmp.DeleteObject();  //删除位图
	Paint = FALSE;
	return;
}

void myRectangle(CDC* pDC, int x, int y, int btn, int dx)
{
	COLORREF  co = 0;
	if (btn == 2 && dx == 2)
	{
		co = pDC->SetTextColor(RGB(0x7E, 0x7E, 0x7E));
	}
	else 	if (btn == 2 && dx == 1)
	{
		co = pDC->SetTextColor(RGB(0x48, 0xc6, 0xff));//48c6ff
	}
	else
	{
		co = pDC->SetTextColor(RGB(0x7E, 0x7E, 0x7E));
	}
	pDC->SetBkMode(0);
	pDC->TextOut(x, y, "X", 1); //1022, 11
	pDC->SetTextColor(co);

	COLORREF  rmin = 0;
	if (btn == 1 && dx == 2)
	{
		rmin = RGB(0x7E, 0x7E, 0x7E);
	}
	else if (btn == 1 && dx == 1)
	{
		rmin = RGB(0x48, 0xc6, 0xff);
	}
	else
	{
		rmin = RGB(0x7E, 0x7E, 0x7E);
	}

	CPen pen(PS_SOLID, 2, rmin);
	CPen* op = pDC->SelectObject(&pen);
	pDC->MoveTo(x-46, y+9); //1022, 11
	pDC->LineTo(x-32, y+9);
	pDC->LineTo(x - 32, y + 10);
	pDC->MoveTo(x - 46, y +10);
	pDC->SelectObject(op);


	::DeleteObject(&pen);
	

	//原文链接：https ://blog.csdn.net/myruo/article/details/86622994
}

void myRedraw(CDC* dc)
{
	//          X    Y      W        H
	CRect rect(477, 590, 477 + 57, 819 + 71);
	CPen NewPen, * pOldPen;
	NewPen.CreatePen(PS_SOLID, 1, RGB(0xf0, 0xf0, 0xf0));
	pOldPen = dc->SelectObject(&NewPen);
	CBrush NewBrush, * pOldBrush;
	NewBrush.CreateSolidBrush(0xF0F0F0);//创建画刷
	pOldBrush = dc->SelectObject(&NewBrush);//选择画刷，保存原来画刷
	dc->Rectangle(rect);
	dc->SelectObject(&pOldPen);//恢复设备上下文
	dc->SelectObject(&pOldBrush);
	::DeleteObject(&NewPen);
	::DeleteObject(&NewBrush);
}

void myNewdraw(CDC* pDC)
{
	if (!g_width|| !g_height)
	{
		Getposwh();
	}
	CRect rc;
	CDC dcMem;
	GetClientRect(g_hwnd, &rc);
	CBitmap bmp;  //内存中承载临时图象的位图


	dcMem.CreateCompatibleDC(pDC);  //依附窗口DC创建兼容内存DC
	//创建兼容位图(必须用pDC创建，否则画出的图形变成黑色)
	bmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	CBitmap* pOldBit = dcMem.SelectObject(&bmp);
	//按原来背景填充客户区，不然会是黑色ec4141
	dcMem.FillSolidRect(rc, RGB(0xEC, 0x41, 0x41));

	CPoint pt[4] = { 0 };
	pt[0] = { rc.left,rc.top };
	pt[1] = { rc.left + rc.Width(),rc.top };
	pt[2] = { rc.left + rc.Width() ,rc.top + rc.Height() };
	pt[3] = { rc.left  ,rc.top + rc.Height() };
	myPolygon(&dcMem, pt, 4, RGB(0xf0, 0xf0, 0xf0), 0);
	pt[0] = { rc.left,  rc.top };
	pt[1] = { rc.left + rc.Width(), rc.top };
	pt[2] = { rc.left + rc.Width() ,rc.top + +60 };
	pt[3] = { rc.left  ,rc.top + 60 };
	myPolygon(&dcMem, pt, 4, RGB(0xec, 0x41, 0x41), 0);
	myRectangle(&dcMem, 979, 16, 0, 0);
	// 绘制图标
	dcMem.DrawIcon(10, 10, g_hIcon);

	pt[0] = { rc.left,  570 };
	pt[1] = { rc.left + rc.Width(), 570 };
	pt[2] = { rc.left + rc.Width() ,572};
	pt[3] = { rc.left  ,572 };
	myPolygon(&dcMem, pt, 4, RGB(0xe0, 0xe0, 0xe0), 0);

	LOGFONT lf;
	CFont fontTemp, * pFontOld;

	dcMem.GetCurrentFont()->GetLogFont(&lf);
	lstrcpy(lf.lfFaceName, _T("宋体"));
	lf.lfWidth = 0;
	lf.lfWeight = FW_HEAVY;
	lf.lfHeight = 0xE;
	fontTemp.CreateFontIndirect(&lf);      //创建字体
	pFontOld = dcMem.SelectObject(&fontTemp);
	dcMem.SetBkMode(0);
	COLORREF  obco = dcMem.SetTextColor(0x34bffc);
	dcMem.TextOut(55, 15, "本地音乐播放器 2.0");
	dcMem.SelectObject(pFontOld);
	dcMem.SetTextColor(obco);
	DeleteObject(fontTemp);
	CRect rt;

	::GetWindowRect(g_hwnd, &rt);
	if (rt.Width() < 0 || rt.Height() < 0)
	{
		return;
	}
	int x=0, y=0, w=0, h=0;

	
		if (rt.left >= 0)
		{
			x = 0;
			w = rc.Width();
		}
		else if (rt.left < 0)
		{
			x = (~rt.left) + 1;
			w = rc.Width() - x;
		}
		if (rt.top >= 0)
		{
			y = 0;
			h = 60;
		}
		else if (rt.top < 0)
		{
			y = (~rt.top) + 1;
			h = 60 - y;
		}
	
	
	if (w <= 0 || h <= 0)
	{
		return;
	}


	pDC->StretchBlt(0, 0, 1020, 60, &dcMem, 0, 0, 1020, 60, SRCCOPY);
	pDC->StretchBlt(0, 570, 1020, 4, &dcMem, 0,570, 1020, 4, SRCCOPY);
	//绘图完成后的清理
	dcMem.DeleteDC();      //删除DC
	bmp.DeleteObject();  //删除位图
	return;
}