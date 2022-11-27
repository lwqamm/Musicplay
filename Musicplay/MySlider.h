#pragma once
#include <afxcmn.h>
#include <afx.h>
class MySlider
{
public:
	MySlider();
	~MySlider();
public:
	//��ȡ����λ�÷��ذٷֱ�
	int GetPos() const;
	//���û���λ�ðٷֱ�
	void SetPos( int nPos);

	//���û��鱳����ɫ
	void SetBack(COLORREF color);
	//���ý�������ɫ
	void SetSpe(COLORREF color);
	//���ý�������խ
	void SetSize(int size);
	//���ؽ�������խ
	int GetSize();
	//���ؽ�������խ
	POINT GetBraxy();
	//���ý��������ȱ�����100�ı���
	void SetLen(int len);
	//���ؽ���������
	int GetLen();
	//�����������������ָ��CDC�豸�ʹ��ڱ�����ɫ
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
	//��������
	BOOL myPolygon(CDC* dc,const POINT* lpPoints, int nCount, COLORREF crColor);
     //������
	BOOL ellipse(CDC* dc);
	void OnPaint(CDC* dc);
	void OnLButtonUp(CPoint point);
	void OnLButtonDown(CPoint point);
	void OnMouseMove(CPoint point);
	//�ػ�
	void myRedraw(CDC* dc);
	//�����������
	void SetPosxy();
	//��ȡ������
	void GetWindowRect();
private:	
	int m_Slider_len; //���������ȱ�����100�ı���
	int m_Slider_size ; //��������С �������
	BOOL m_size = FALSE;//�Ƿ�������
	int m_Slider_pos ;   //������λ��
	COLORREF m_Slider_back;//������������ɫ
	COLORREF m_windows_back;//���ڱ�����ɫ
	COLORREF m_Slider_spe;  //��������ɫ
	CDC* m_dc;    //���ƽ��������豸
	CRect rc;
	MYRECT m_pos_rect = { 0 }; //������pos
	POINT  m_Slider_xy;        //��������ʼλ��
	HWND m_hWnd;              //�����ھ��
	BOOL m_Slider = FALSE; //�Ƿ�ˢ�±���
	BOOL m_back = FALSE;   //�Ƿ񻭳��˻���
	BOOL m_Slider_btnDwn = FALSE; //����������
	BOOL m_Slider_braDwn = FALSE; //���µ��Ƿ�Ϊ����������
	BOOL m_Slider_posbtn = FALSE;
	int   m_width = 0;
	int   m_height = 0;
	BOOL  m_bar = FALSE;         //��һ��
};


