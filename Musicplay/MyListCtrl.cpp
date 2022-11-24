// MyListCtrl.cpp : 实现文件
//
#include "pch.h"
#include "Musicplay.h"
#include "MyListCtrl.h"
#include <atltypes.h>


// CMyListCtrl

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl()
{
	m_OddItemBkColor=0xFEF1E3;//奇数行背景颜色
	m_EvenItemBkColor=0xFFFFFF;//偶数行背景颜色
	m_HoverItemBkColor = RGB(228, 228, 228);// 0xFF;//热点行背景颜色
	m_SelectItemBkColor = RGB(228, 228, 228);// GetSysColor(COLOR_HIGHLIGHT);//选中行背景颜色
	m_OddItemTextColor=GetSysColor(COLOR_BTNTEXT);//奇数行文本颜色
	m_EvenItemTextColor=GetSysColor(COLOR_BTNTEXT);//偶数行文本颜色
	m_HoverItemTextColor = RGB(0x33, 0x33, 0x33);// GetSysColor(COLOR_HIGHLIGHTTEXT);//热点行文本颜色
	m_SelectItemTextColor = RGB(0x33, 0x33, 0x33);/*RGB(0, 255,200)*/GetSysColor(COLOR_BTNTEXT);//选中行文本颜色
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



// CMyListCtrl 消息处理程序


void CMyListCtrl::SetOddItemBkColor(COLORREF color,BOOL bDraw)//设置奇数行背景颜色
{
	m_OddItemBkColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetEvenItemBkColor(COLORREF color,BOOL bDraw)//设置偶数行背景颜色
{
	m_EvenItemBkColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetHoverItemBkColor(COLORREF color,BOOL bDraw)//设置热点行背景颜色
{
	m_HoverItemBkColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetSelectItemBkColor(COLORREF color,BOOL bDraw)//设置选中行背景颜色
{
	m_SelectItemBkColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetOddItemTextColor(COLORREF color,BOOL bDraw)//设置奇数行文本颜色
{
	m_OddItemTextColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetEvenItemTextColor(COLORREF color,BOOL bDraw)//设置偶数行文本颜色
{
	m_EvenItemTextColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetHoverItemTextColor(COLORREF color,BOOL bDraw)//设置热点行文本颜色
{
	m_HoverItemTextColor=color;
	if(bDraw)InvalidateRect(NULL);
}
void CMyListCtrl::SetSelectItemTextColor(COLORREF color,BOOL bDraw)//设置选中行文本颜色
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
	 /*修改内容： 
	   改用switch case语句
	   case语句中增加了CDDS_ITEMPREPAINT和CDDS_ITEMPOSTPAINT两个状态判断
	   同时在改变颜色时增加了一个函数SetItemState
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
			 //选中行颜色改变
			 if (GetItemState(nItemIndex,LVIS_SELECTED) == LVIS_SELECTED/*pLVCD->nmcd.uItemState & CDIS_SELECTED*/)
			 {      
				 s_bThisItemSelect = TRUE; 
				 //增加了下面这个SetItemState函数
				 SetItemState(pLVCD->nmcd.dwItemSpec, 0, LVIS_SELECTED); 
				 pLVCD->clrText = m_SelectItemTextColor;     
				 pLVCD->clrTextBk =m_SelectItemBkColor;  
				// pLVCD->clrTextBk = RGB(228, 228, 228);
				// pLVCD->clrText = 0;//RGB(0x35, 0xC1, 0xFF);

			 }          
			 
             else if((nItemIndex==m_nHoverIndex))
			 { //热点行
				 pLVCD->clrTextBk=m_HoverItemBkColor;
				 pLVCD->clrText=m_HoverItemTextColor;
			
				 
				 
			 }
			 else if(nItemIndex == m_dex)
			 {//偶数行 比如 0、2、4、6
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
			 {	//奇数行 比如 1、3、5、7
				// pLVCD->clrTextBk=m_OddItemBkColor;
				// pLVCD->clrText=m_OddItemTextColor;
			 }
		 *pResult = CDRF_NOTIFYPOSTPAINT;    
	}       
		 break;    
		 //加了一个状体判断
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
