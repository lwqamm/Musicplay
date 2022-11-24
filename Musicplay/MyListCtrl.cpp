// MyListCtrl.cpp : ʵ���ļ�
//
#include "pch.h"
#include "Musicplay.h"
#include "MyListCtrl.h"
#include <atltypes.h>


// CMyListCtrl

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl()
{
	m_OddItemBkColor=0xFEF1E3;//�����б�����ɫ
	m_EvenItemBkColor=0xFFFFFF;//ż���б�����ɫ
	m_HoverItemBkColor = RGB(228, 228, 228);// 0xFF;//�ȵ��б�����ɫ
	m_SelectItemBkColor = RGB(228, 228, 228);// GetSysColor(COLOR_HIGHLIGHT);//ѡ���б�����ɫ
	m_OddItemTextColor=GetSysColor(COLOR_BTNTEXT);//�������ı���ɫ
	m_EvenItemTextColor=GetSysColor(COLOR_BTNTEXT);//ż�����ı���ɫ
	m_HoverItemTextColor = RGB(0x33, 0x33, 0x33);// GetSysColor(COLOR_HIGHLIGHTTEXT);//�ȵ����ı���ɫ
	m_SelectItemTextColor = RGB(0x33, 0x33, 0x33);/*RGB(0, 255,200)*/GetSysColor(COLOR_BTNTEXT);//ѡ�����ı���ɫ
	m_nHoverIndex=-1;
	m_bTracking=FALSE;
}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyListCtrl::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(NM_CLICK, &CMyListCtrl::OnNMClickList1)
END_MESSAGE_MAP()



// CMyListCtrl ��Ϣ�������


void CMyListCtrl::SetOddItemBkColor(COLORREF color,BOOL bDraw)//���������б�����ɫ
{
	m_OddItemBkColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetEvenItemBkColor(COLORREF color,BOOL bDraw)//����ż���б�����ɫ
{
	m_EvenItemBkColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetHoverItemBkColor(COLORREF color,BOOL bDraw)//�����ȵ��б�����ɫ
{
	m_HoverItemBkColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetSelectItemBkColor(COLORREF color,BOOL bDraw)//����ѡ���б�����ɫ
{
	m_SelectItemBkColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetOddItemTextColor(COLORREF color,BOOL bDraw)//�����������ı���ɫ
{
	m_OddItemTextColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetEvenItemTextColor(COLORREF color,BOOL bDraw)//����ż�����ı���ɫ
{
	m_EvenItemTextColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetHoverItemTextColor(COLORREF color,BOOL bDraw)//�����ȵ����ı���ɫ
{
	m_HoverItemTextColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetSelectItemTextColor(COLORREF color,BOOL bDraw)//����ѡ�����ı���ɫ
{
	m_SelectItemTextColor=color;
	if(bDraw)InvalidateRect(NULL);
}

void CMyListCtrl::SetListdxColor(int dx, BOOL ret)
{
	m_dex = dx;
	m_dexColor = ret;
	SetItemData(dx, 5);
	this->Invalidate(TRUE);
	UpdateData(FALSE);

}


void CMyListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int nIndex=HitTest(point);
	if(nIndex!=m_nHoverIndex){
		if (nIndex == m_dex)
		{
			SetHoverItemBkColor(RGB(228, 228, 228));
			SetHoverItemTextColor(RGB(0x35, 0xC1, 0xFF));
			
		}
		else
		{
			SetHoverItemBkColor(RGB(228, 228, 228));
			SetHoverItemTextColor(RGB(0x33, 0x33, 0x33));
			
		}

		int nOldIndex=m_nHoverIndex;
		m_nHoverIndex=nIndex;
		CRect rc;
		if(nOldIndex!=-1){
			GetItemRect(nOldIndex,&rc,LVIR_BOUNDS);
			InvalidateRect(&rc);
		}		
		if(m_nHoverIndex!=-1){
			GetItemRect(m_nHoverIndex,&rc,LVIR_BOUNDS);
			InvalidateRect(&rc);
		}
	}

	//=====================================================
	if(!m_bTracking) 
	{ 
		TRACKMOUSEEVENT   tme; 
		tme.cbSize   =   sizeof(tme); 
		tme.hwndTrack   =   m_hWnd; 
		tme.dwFlags   =   TME_LEAVE;//   |   TME_HOVER; 
		tme.dwHoverTime   =   1; 
		m_bTracking   =   _TrackMouseEvent(&tme); 
	} 
	CListCtrl::OnMouseMove(nFlags, point);
}
LRESULT CMyListCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking=FALSE;
	
	if(m_nHoverIndex!=-1){
		CRect rc;
		GetItemRect(m_nHoverIndex,&rc,LVIR_BOUNDS);
		m_nHoverIndex=-1;
	 	InvalidateRect(&rc);
	}
	return 0;
}
void CMyListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	 /*�޸����ݣ� 
	   ����switch case���
	   case�����������CDDS_ITEMPREPAINT��CDDS_ITEMPOSTPAINT����״̬�ж�
	   ͬʱ�ڸı���ɫʱ������һ������SetItemState
	   */
	 static BOOL s_bThisItemSelect = FALSE;   
	 NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR); 
	 int nItemIndex=pLVCD->nmcd.dwItemSpec;
	 *pResult = CDRF_DODEFAULT;   
	 switch (pLVCD->nmcd.dwDrawStage) 
	 {   
	 case CDDS_PREPAINT:  
		 *pResult = CDRF_NOTIFYITEMDRAW;  
		 break;   
	 case CDDS_ITEMPREPAINT:  
		 {    
			 UINT ordin = pLVCD->nmcd.lItemlParam; 
			 s_bThisItemSelect = FALSE; 
			 //ѡ������ɫ�ı�
			 if (GetItemState(nItemIndex,LVIS_SELECTED) == LVIS_SELECTED/*pLVCD->nmcd.uItemState & CDIS_SELECTED*/)
			 {      
				 s_bThisItemSelect = TRUE; 
				 //�������������SetItemState����
				 SetItemState(pLVCD->nmcd.dwItemSpec, 0, LVIS_SELECTED); 
				 pLVCD->clrText = m_SelectItemTextColor;     
				 pLVCD->clrTextBk =m_SelectItemBkColor;  
				// pLVCD->clrTextBk = RGB(228, 228, 228);
				// pLVCD->clrText = 0;//RGB(0x35, 0xC1, 0xFF);

			 }          
			 
             else if((nItemIndex==m_nHoverIndex))
			 { //�ȵ���
				 pLVCD->clrTextBk=m_HoverItemBkColor;
				 pLVCD->clrText=m_HoverItemTextColor;
			
				 
				 
			 }
			 else if(nItemIndex == m_dex)
			 {//ż���� ���� 0��2��4��6
				// pLVCD->clrTextBk=m_EvenItemBkColor;
				// pLVCD->clrText  =    m_EvenItemTextColor;
				 switch (m_dexColor)
				 {
				 case TRUE:
				 {
					 pLVCD->clrTextBk = RGB(228, 228, 228);
					 pLVCD->clrText = RGB(0x35, 0xC1, 0xFF);
					 break;
				 }
				 case FALSE:
				 {
					 pLVCD->clrTextBk = RGB(240, 240, 240);
					 pLVCD->clrText = RGB(0x33, 0x33, 0x33);
					 break;
				 }
				 default:
					 break;
				 }
				
			 }
			 else
			 {	//������ ���� 1��3��5��7
				// pLVCD->clrTextBk=m_OddItemBkColor;
				// pLVCD->clrText=m_OddItemTextColor;
			 }
		 *pResult = CDRF_NOTIFYPOSTPAINT;    
	}       
		 break;    
		 //����һ��״���ж�
	 case CDDS_ITEMPOSTPAINT:   
		 if (s_bThisItemSelect) 
	 	 SetItemState(pLVCD->nmcd.dwItemSpec, 0xFF, LVIS_SELECTED);  
		 break;  
	 } 
	
}

void CMyListCtrl::OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int dx = pNMItemActivate->iItem;
	m_Clickdx = dx;
	if (dx == m_dex)
	{
		SetSelectItemBkColor(RGB(228, 228, 228));
		SetSelectItemTextColor(RGB(0x35, 0xC1, 0xFF));
	}
	else
	{
		SetSelectItemBkColor(RGB(228, 228, 228));
		SetSelectItemTextColor(RGB(0x33, 0x33, 0x33));
	}
	return;
}

 
 

void CMyListCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	 ModifyStyle(LVS_SHOWSELALWAYS, LVS_SINGLESEL);    // TRICKY!!! can`t with LVS_SHOWSELALWAYS 
	 SetExtendedStyle(GetExtendedStyle()|LVS_EX_FULLROWSELECT);    
	 CListCtrl::PreSubclassWindow(); 
}
