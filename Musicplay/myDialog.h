#pragma once
//#include <afxdialogex.h>


BOOL myPolygon(CDC* dc, const POINT* lpPoints, int nCount, COLORREF crColor, POINT* pxy);

void myRectangle(CDC* dc, int x, int y, int btn, int dx);

void SetPaly(CDC* dc, int x, int y, int btn, int dx, int paly);

void myNewdraw(CDC* dc);
