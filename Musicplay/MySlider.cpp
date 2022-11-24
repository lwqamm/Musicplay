#include "pch.h"
#include "MySlider.h"
#include <iostream>
#include "MicPalay.h"

//#include <wingdi.cpp>
//#include <afxwin.h>
#define BK_COLOR RGB(255, 255, 255)
#define SEP_COLOR RGB(0x42,0x99,0xED)	//4299ed
#define WINDOWS_COLOR RGB(0xF0,0xF0,0xF0)	//4299ed
#define VT_DOWN    1
#define VT_UP      2
#define VT_MOVE    3


extern int GetDcColor(int* x1, int* y1, int w, int h);

BOOL MP3Btn = FALSE;       //MP3����������
BOOL MP3BtnUP = FALSE;     //MP3����������
extern HWND g_hwnd ;
MySlider* g_mthis = NULL;//�������

typedef struct _MYPOINT
{
	POINT pt;
	int x;
}MYPOINT,*PMYPOINT;
MySlider::MySlider()
{
	m_hWnd=0;
	 m_Slider_len = 100; //���������ȱ�����100�ı���
	m_Slider_size = 10; //��������С �������
	 m_Slider_pos = 0;   //������λ��
	 m_Slider_back = COLORREF(BK_COLOR);//������������ɫ COLORREF(DLG_BK_COLOR);
	 m_windows_back = COLORREF(WINDOWS_COLOR);
	 m_Slider_spe = COLORREF(SEP_COLOR);  //��������ɫ
	 m_dc=NULL;    //���ƽ��������豸
	 m_Slider_xy = {0,0};
	 g_mthis = this;
	 GetWindowRect();
	// m_cur1 = GetCursor();
}
MySlider::~MySlider()
{

}
int MySlider::GetPos() const
{
	
	return m_Slider_pos;
}

int poss = 1;
void MySlider::GetWindowRect()
{
	int no_menu_bar_width = GetSystemMetrics(SM_CXFULLSCREEN);
	int no_menu_bar__height = GetSystemMetrics(SM_CYFULLSCREEN);
	m_width = no_menu_bar_width;
	m_height = no_menu_bar__height;
}
void MySlider::SetPos(int nPos)
{
	if (nPos== poss&& nPos != 0)
	{
		return;
	}
	if (nPos>100)
	{
		nPos = 100;
	}
	if (nPos<0)
	{
		nPos = 0;
	}
	poss = nPos+1;
	if (!::IsWindow(g_hwnd) || !::IsWindowVisible(g_hwnd) ||
		::IsIconic(g_hwnd))
	{
		m_Slider_pos = nPos;
		return;//��С������û��ʾʱ�˳�
	}
	CRect rt;
	::GetWindowRect(m_hWnd,&rt);
	if (rt.Width()<0||rt.Height()<0)
	{
		return;
	}
	if (rt.left<-(m_Slider_xy.x-1)||rt.top<-(m_Slider_xy.y-1))
	{
		return;
	}
	if (rt.left+ (rt.Width()-m_Slider_xy.x-m_Slider_len)>m_width||rt.top+(rt.Height()-(m_Slider_xy.y+m_Slider_size))>m_height)
	{
		return;
	}

	if (poss == 0)
	{
		return;
	}
	poss = 0;
	m_Slider_pos = nPos;
	int x = m_Slider_xy.x, y = m_Slider_xy.y - 4, w = m_Slider_xy.x + m_Slider_len,
		h = m_Slider_xy.y + m_Slider_size + 8;
	if (x<0||y<0||w<0||h<0)
	{
		return;
	}
	CDC dcMem;

	CBitmap bmp;  //�ڴ��г�����ʱͼ���λͼ


	dcMem.CreateCompatibleDC(m_dc);  //��������DC���������ڴ�DC
	//��������λͼ(������pDC���������򻭳���ͼ�α�ɺ�ɫ)
	bmp.CreateCompatibleBitmap(m_dc, rc.Width(), rc.Height());
	CBitmap* pOldBit = dcMem.SelectObject(&bmp);
	//��ԭ���������ͻ�������Ȼ���Ǻ�ɫ
	dcMem.FillSolidRect(rc, m_windows_back);
	myRedraw(&dcMem);//�ػ�
	
	m_dc->StretchBlt(x, y, w,h, &dcMem, x, y, w, h, SRCCOPY);

	//���ڴ�DC�ϵ�ͼ�󿽱���ǰ̨
	//��ͼ��ɺ������
	dcMem.DeleteDC();      //ɾ��DC
	bmp.DeleteObject();  //ɾ��λͼ

	return;
}

void MySlider::SetBack(COLORREF color)
{
	if (color<=0)
	{
		return;
	}
	m_Slider_back = color;
}
void MySlider::SetSpe(COLORREF color)
{
	if (color <= 0)
	{
		return;
	}
	m_Slider_spe = color;
}
void MySlider::SetSize(int size)
{
	if (size>0)
	{
		m_Slider_size = size;
	}
}
int MySlider::GetSize()
{
	return m_Slider_size;
}
POINT MySlider::GetBraxy()
{
	return m_Slider_xy;
}
void MySlider::SetLen(int len)
{
	if (m_hWnd==0)
	{
		return;
	}
	if (len>=100)
	{
		CRect rect;
		GetClientRect(m_hWnd, &rect);
		int w = rect.Width();
		if (m_Slider_xy.x + len > w)
		{
			
			len = w - m_Slider_xy.x;
		}

		m_Slider_len = len;
	}
}
int MySlider::GetLen()
{
	return m_Slider_len;
}
BOOL MySlider::myPolygon(CDC* dc,const POINT* lpPoints, int nCount, COLORREF crColor)
{
	if (nCount <= 2 || !m_dc)
	{
		return FALSE;
	}
	for (int i = 0; i < nCount; i++)
	{
		if (lpPoints[i].x < 0 || lpPoints[i].y<0)
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
	if (crColor > 0)
	{
		pen.CreatePen(PS_SOLID, 1, crColor);
		brush.CreateSolidBrush(crColor);//��ɫ��ˢ35c1ff
		obbrush = dc->SelectObject(&brush);
		obpen = dc->SelectObject(&pen);
	}
	else
	{
		return FALSE;
	}
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

	x = lpPoints[0].x ;
	y = lpPoints[0].y ;
	if (GetDcColor(&x,&y, lpPoints[1].x- lpPoints[0].x, lpPoints[2].y- lpPoints[1].y)==-1)
	{
		return FALSE; 
	}

	if (x<0||y<0)
	{
		return FALSE;
	}

	if (crColor > 0)
	{

		dc->FloodFill(x, y, crColor); //�������
		dc->SelectObject(obbrush);
		dc->SelectObject(obpen);
		::DeleteObject(obpen);
		::DeleteObject(obbrush);
		return TRUE;
	}
	dc->FloodFill(x, y, m_dc->GetBkColor()); //�������
	dc->SelectObject(obbrush);
	dc->SelectObject(obpen);
	::DeleteObject(brush);
	::DeleteObject(pen);
	return TRUE;
}

BOOL MySlider::ellipse(CDC* dc)
{
	if ( !m_dc)
	{
		return FALSE;
	}
	int x = m_pos_rect.x, y = m_pos_rect.y, w = m_pos_rect.x1, h = m_pos_rect.y1;
	if (x < 0 || y < 0 || w < 0 || h < 0)
	{
		return FALSE;
	}
	CPen  * pOldPen;
	CPen pen1(PS_SOLID, 1, m_Slider_spe);
	CBrush bsh, * obsh;
	bsh.CreateSolidBrush(m_Slider_spe);//������ˢ
	pOldPen = dc->SelectObject(&pen1);
	obsh = dc->SelectObject(&bsh);
	 bool ret= dc->Ellipse(m_pos_rect.x, m_pos_rect.y, m_pos_rect.x1, m_pos_rect.y1);
	 dc->SelectObject(pOldPen);
	 ::DeleteObject(&pen1);
	 dc->SelectObject(obsh);
	 ::DeleteObject(&bsh);
	 m_back = TRUE;
	 return ret;
}
void MySlider::OnPaint(CDC* dc)
{
	if (m_Slider_size<=2)
	{
		m_Slider_size = 2;
	}
	m_Slider_len = m_Slider_len - (m_Slider_len%100);
	POINT pt[4] = { 0 };
	pt[0] = { m_Slider_xy.x, m_Slider_xy.y };
	pt[1] = { m_Slider_xy.x+ m_Slider_len, m_Slider_xy.y};
	pt[2] = { m_Slider_xy.x + m_Slider_len, m_Slider_xy.y+ m_Slider_size };
	pt[3] = { m_Slider_xy.x , m_Slider_xy.y + m_Slider_size };
	
	myPolygon(dc,pt,4, m_Slider_back);
	m_bar = TRUE;
	if (m_Slider_pos==0)
	{
		return;
	}
	int tpos = m_Slider_len / 100 * m_Slider_pos;
	pt[0] = { m_Slider_xy.x, m_Slider_xy.y };
	pt[1] = { m_Slider_xy.x + tpos, m_Slider_xy.y };
	pt[2] = { m_Slider_xy.x + tpos, m_Slider_xy.y + m_Slider_size };
	pt[3] = { m_Slider_xy.x , m_Slider_xy.y + m_Slider_size };
	myPolygon(dc,pt, 4, m_Slider_spe);
	SetPosxy();
	if (m_Slider)
	{
		ellipse(dc);
	}

}
BOOL MySlider::Create(HWND hwnd,CDC* dc, COLORREF color, int x, int y)
{
	if (!hwnd||!dc)
	{
		return FALSE;
	}
	m_dc=dc;
	m_hWnd = hwnd;
	GetClientRect(m_hWnd, &rc);
	if (color>0)
	{
		m_windows_back = color;
	}
	if (x>0||y>0)
	{
		m_Slider_xy.x = x;
		m_Slider_xy.y = y;
	}
	return 0;
}

void MySlider::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//SendMessage(hwnd, WM_LBUTTONDOWN,0, MAKELPARAM(x,y));��������
	


	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		CPoint pt;
		POINTSTOPOINT(pt, lParam)
			OnLButtonDown(pt);
		return ;
	}
	case WM_LBUTTONUP:
	{
		CPoint pt;
		POINTSTOPOINT(pt, lParam)
			OnLButtonUp(pt);
		return ;
	}
	case WM_MOUSEMOVE:
	{
		CPoint pt;
		POINTSTOPOINT(pt, lParam)
			OnMouseMove(pt);
		return ;
	}
	default:
		break;
	}
	return ;
}

void MySlider::OnLButtonUp(CPoint point)
{
	int x = point.x, y = point.y;
	if (point.x >= m_Slider_xy.x && point.x <= m_Slider_xy.x + m_Slider_len &&
		point.y >= m_Slider_xy.y - 4 && point.y <= m_Slider_xy.y + m_Slider_size + 4)
	{
		MP3BtnUP = TRUE;
	}
		//ellipse(point);
	if (m_Slider_braDwn == TRUE&& m_Slider_btnDwn == FALSE)
	{
		int tpos = m_Slider_len / 100;
		int pos = 0;
		pos = (x - m_Slider_xy.x) / tpos;
		if (pos>=100)
		{
			pos = 100;
		} 
		else if (pos<=0)
		{
			pos = 0;
		}
	
		//SetPos(pos);
		m_Slider_pos = pos;

		m_Slider_braDwn = FALSE;
		return;
	}

	m_Slider_braDwn = FALSE;
	m_Slider_btnDwn = FALSE;
	return;
}

void MySlider::OnLButtonDown(CPoint point)
{
	SetPosxy();
	int x = point.x, y = point.y;
	if (x>=m_pos_rect.x&&x<= m_pos_rect.x1 &&y>= m_pos_rect.y&&y<= m_pos_rect.y1)
	{
		if (m_Slider_btnDwn == TRUE)
		{
			return;
		}
		MP3Btn = TRUE;
		m_Slider_braDwn = FALSE;
		m_Slider_btnDwn = TRUE;
		return;
	}
	if (x >= m_Slider_xy.x && x <= m_Slider_xy.x+m_Slider_len && y >= m_Slider_xy.y-2 && y <= m_Slider_xy.y+m_Slider_size+2)
	{
		if (m_Slider_braDwn == TRUE|| m_Slider_btnDwn==TRUE)
		{
			return;
		}
		MP3Btn = TRUE;
		m_Slider_btnDwn = FALSE;
		m_Slider_braDwn = TRUE;
		return;
	}

	return;
}

void MySlider::OnMouseMove(CPoint point)
{
	
	
		if (point.x >= m_Slider_xy.x && point.x <= m_Slider_xy.x + m_Slider_len &&
			point.y >= m_Slider_xy.y - 4 && point.y <= m_Slider_xy.y + m_Slider_size + 4)
		{
			m_Slider = TRUE;
			
			if (m_back)
			{
				return;
			}
			if (!m_size)
			{
				m_Slider_size += 3;
				m_size = TRUE;
			}
			SetPosxy();
		}
		else
		{		
		 m_Slider = FALSE;	
		 if (m_size)
		 {
			 m_Slider_size -= 3;
			 m_size = FALSE;
		 }
		 SetPosxy();
		}

	return;
}

void MySlider::myRedraw( CDC* dc)
{
	if (m_Slider_size <= 2)
	{
		m_Slider_size = 2;
	}
	CRect rect(m_Slider_xy.x, m_Slider_xy.y-4, m_Slider_xy.x + m_Slider_len, m_Slider_xy.y + m_Slider_size + 4);
	int x = m_Slider_xy.x, y = m_Slider_xy.y - 4, w = m_Slider_xy.x + m_Slider_len, h = m_Slider_xy.y + m_Slider_size + 4;
	if (x < 0 || y < 0 || w < 0 || h < 0)
	{
		return;
	}
	CPen NewPen, * pOldPen;
	NewPen.CreatePen(PS_SOLID, 1, m_windows_back);
	pOldPen = dc->SelectObject(&NewPen);

	CBrush NewBrush, * pOldBrush;
	NewBrush.CreateSolidBrush(m_windows_back);//������ˢ
	pOldBrush = dc->SelectObject(&NewBrush);//ѡ��ˢ������ԭ����ˢ
	dc->Rectangle(rect);
	dc->SelectObject(pOldPen);//�ָ��豸������
	dc->SelectObject(pOldBrush);
	::DeleteObject(&NewPen);
	::DeleteObject(&NewBrush);
	OnPaint(dc);
	m_back = FALSE;
}

void MySlider::SetPosxy()
{
	if (m_Slider_pos == 100)
	{
		m_pos_rect.x = m_Slider_xy.x+m_Slider_len - (m_Slider_size + 6);
		if (m_Slider_xy.y == 0)
		{
			m_pos_rect.y = 0;
		}
		else
		{
			m_pos_rect.y = m_Slider_xy.y - 4;
			m_pos_rect.y1 = m_pos_rect.y + m_Slider_size + 8;
			m_pos_rect.x1 = m_pos_rect.x + m_Slider_size + 8;
		}

	}
	else
	{
		m_pos_rect.x = m_Slider_xy.x + (m_Slider_pos * (m_Slider_len / 100));
		if (m_Slider_xy.y == 0)
		{
			m_pos_rect.y = 0;
		}
		else
		{
			m_pos_rect.y = m_Slider_xy.y - 4;
			m_pos_rect.y1 = m_pos_rect.y + m_Slider_size + 8;;
			m_pos_rect.x1 = m_pos_rect.x + m_Slider_size + 8;
		}
	}
	return ;
}



///////////////////////////////////////////////////////////
