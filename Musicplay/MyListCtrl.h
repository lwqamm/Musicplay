#pragma once


// CMyListCtrl

class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

public:
	CMyListCtrl();
	virtual ~CMyListCtrl();
public:
	//���������б�����ɫ
	void SetOddItemBkColor(COLORREF color,BOOL bDraw=TRUE);
	//����ż���б�����ɫ
	void SetEvenItemBkColor(COLORREF color,BOOL bDraw=TRUE);
	//�����ȵ��б�����ɫ
	void SetHoverItemBkColor(COLORREF color,BOOL bDraw=TRUE);
	//����ѡ���б�����ɫ
	void SetSelectItemBkColor(COLORREF color,BOOL bDraw=TRUE);
	//�����������ı���ɫ
	void SetOddItemTextColor(COLORREF color,BOOL bDraw=TRUE);
	//����ż�����ı���ɫ
	void SetEvenItemTextColor(COLORREF color,BOOL bDraw=TRUE);
	//�����ȵ����ı���ɫ
	void SetHoverItemTextColor(COLORREF color,BOOL bDraw=TRUE);
	//����ѡ�����ı���ɫ
	void SetSelectItemTextColor(COLORREF color,BOOL bDraw=TRUE);
	//
	void SetListdxColor(int dx, BOOL ret=TRUE );
protected:
	BOOL m_bTracking;
	int m_nHoverIndex;//��ǰ�ȵ���Ŀ����
	COLORREF m_OddItemBkColor;//�����б�����ɫ
	COLORREF m_EvenItemBkColor;//ż���б�����ɫ
	COLORREF m_HoverItemBkColor;//�ȵ��б�����ɫ
	COLORREF m_SelectItemBkColor;//ѡ���б�����ɫ
	COLORREF m_OddItemTextColor;//�������ı���ɫ
	COLORREF m_EvenItemTextColor;//ż�����ı���ɫ
	COLORREF m_HoverItemTextColor;//�ȵ����ı���ɫ
	COLORREF m_SelectItemTextColor;//ѡ�����ı���ɫ
	int      m_dex = -1;
	int      m_Clickdx = -1;
	BOOL     m_dexColor = TRUE;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult);
 
	 
protected:
	virtual void PreSubclassWindow();
};


