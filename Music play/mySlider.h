#pragma once
#include "afxcmn.h"

class MySlider : public CSliderCtrl
{
public:
	MySlider();
	virtual   ~MySlider() { delete_gdi_objs(); }
	afx_msg   void OnPaint();
	afx_msg   void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg   void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg	  void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	void	  set_bk_color(COLORREF bk) { m_dlg_bk_color = bk; }
	void	  set_thumb_rate(float rate) { m_thumb_rate = rate; }
	void	  set_thumb_colors(COLORREF out, COLORREF in, COLORREF highlight) { m_thumb_color = out; m_thumb_on_color = in; m_thumb_color = highlight; init_gdi_objs(); }
	void	  set_channel_colors(COLORREF bk, COLORREF left) { m_channel_bk_color = bk; m_channel_left_color = left;  init_gdi_objs(); }
private:
	void	  init_gdi_objs();
	void	  delete_gdi_objs();
	void	  owner_draw(CDC* pDc);
	void	  get_ellipse_rect(CRect& rect);
private:
	COLORREF  m_dlg_bk_color;
	COLORREF  m_channel_bk_color;
	COLORREF  m_channel_left_color;
	COLORREF  m_thumb_color;
	COLORREF  m_thumb_on_color;
	COLORREF  m_thumb_selected_color;
	float	  m_thumb_rate;
	BOOL	  thumb_down;
	BOOL	  thumb_selected;
	CPen	  pen_thumb_selected;
	CPen	  pen_thumb;
	CPen	  pen_thumb_on;
	CBrush	  brush_thumb_selected;
	CBrush	  brush_thumb;
	CBrush	  brush_thumb_on;
};