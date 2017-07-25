//Ruler.cpp

#include "StdAfx.h"
#include "Ruler.h"

extern int g_iSpacing;
extern double g_fRate;
extern int g_vOffset;
extern int g_hOffset;
extern int g_vSetOffset;
extern int g_hSetOffset;

CRuler::CRuler()
{
	//初始化各数据
	width = 20;
	//g_iSpacing = 5;
	vernier.x = -width;
	vernier.y = -width;

	//create font
	hfont.CreateFont(
		width * 3 / 4, 0, 
		0, 0, 
		FW_NORMAL, 
		FALSE, 
		FALSE, 
		FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		PROOF_QUALITY, 
		DEFAULT_PITCH + FF_DONTCARE, 
		L"Arail"
		);

	vfont.CreateFont(
		width * 3 / 4, 0, 
		900, 900, 
		FW_NORMAL, 
		FALSE, 
		FALSE, 
		FALSE, 
		ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		PROOF_QUALITY, 
		DEFAULT_PITCH + FF_DONTCARE, 
		L"Arail"
		);	
}

CRuler::~CRuler()
{
	hfont.DeleteObject();
	vfont.DeleteObject();
}

//绘制标尺
void CRuler::Show(CWnd *pWnd, BOOL visible)
{
	CRect rect;
	pWnd->GetClientRect(&rect);
	ClientSize.cx = rect.Width() + g_hOffset;
	ClientSize.cy = rect.Height();
	hrect = vrect = rect;
	hrect.left = width;
	hrect.bottom = width;
	vrect.top = width;
	vrect.right = width;

	CClientDC dc(pWnd);

	//get system color
	COLORREF crface, crhighlight, crshadow;
	crface = ::GetSysColor(COLOR_3DFACE);
	crhighlight = ::GetSysColor(COLOR_3DHIGHLIGHT);
	crshadow = ::GetSysColor(COLOR_3DSHADOW);

	//create dc
	CPen penFace(PS_SOLID, 2, crface);
	CPen penHighLight(PS_SOLID, 2, crhighlight);
	CPen penShadow(PS_SOLID, 2, crshadow);
	CPen* pOldPen;
	CBrush brushFace(crface), *pOldBrush;

	//fill ruler
	pOldPen = dc.SelectObject(&penFace);
	pOldBrush = dc.SelectObject(&brushFace);
	dc.Rectangle(hrect);
	dc.Rectangle(vrect);
	dc.SelectObject(pOldBrush);

	//draw shadow
	dc.SelectObject(&penShadow);
	dc.MoveTo(width, width);
	dc.LineTo(ClientSize.cx, width);
	dc.MoveTo(width, width);
	dc.LineTo(width, ClientSize.cy);

	//draw highlight
	dc.SelectObject(&penHighLight);
	dc.MoveTo(0, 0);
	dc.LineTo(ClientSize.cx, 0);
	dc.MoveTo(0, 0);
	dc.LineTo(0, ClientSize.cy);

	//draw edge
	dc.SelectObject(pOldPen);
	dc.MoveTo(width, width);
	dc.LineTo(ClientSize.cx, width);
	dc.MoveTo(width, width);
	dc.LineTo(width, ClientSize.cy);

	//set viewport origin
	dc.SetViewportOrg(width, width);
	//set background mode
	dc.SetBkMode(TRANSPARENT);

	//show x-coordinate
	CFont* pOldFont = dc.SelectObject(&hfont);
	for(int i = 0; i < ClientSize.cx - width; i += g_iSpacing)
	{
		dc.MoveTo(i - g_hOffset, -1);
		dc.LineTo(i - g_hOffset, -width / 4);
		if(i % (g_iSpacing * 40) == 0)
		{
			dc.LineTo(i - g_hOffset, -3 * width / 4);
			CString txt;
			txt.Format(L"%.2f", (i - g_hSetOffset) * g_fRate);
			dc.SetTextAlign(TA_BOTTOM | TA_LEFT);
			dc.TextOut(i + 2 - g_hOffset, -width / 4 + 2, txt);
		}
		else if(i % (g_iSpacing * 20) == 0)
		{
			dc.LineTo(i - g_hOffset, -width / 2);
		}
	}

	//show y-coordinate
	dc.SelectObject(&vfont);

	for(int i = 0; i < ClientSize.cy - width; i += g_iSpacing)
	{
		dc.MoveTo(-1, ClientSize.cy - width + g_vOffset - i); 
		dc.LineTo(-width / 4, ClientSize.cy - width + g_vOffset - i);
		if(i % (g_iSpacing * 40) == 0)
		{
			dc.LineTo(-3 * width / 4, ClientSize.cy - width + g_vOffset - i);
			CString str;
			str.Format(L"%.2f", (i - g_vSetOffset) * g_fRate);
			dc.SetTextAlign(TA_BOTTOM | TA_LEFT);
			dc.TextOut(-width / 4 + 2, ClientSize.cy - width - 2 + g_vOffset - i, str);
		}
		else if(i % (g_iSpacing * 20) == 0)
		{
			dc.LineTo(-width / 2, ClientSize.cy - width + g_vOffset - i);
		}
	}
	//reset dc
	dc.SelectObject(pOldFont);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);

	if(pOldPen)
	{
		pOldPen->DeleteObject();
		pOldPen = NULL;
	}
	if(pOldFont)
	{
		pOldFont->DeleteObject();
		pOldFont = NULL;
	}
	if(pOldBrush)
	{
		pOldBrush->DeleteObject();
		pOldBrush = NULL;
	}
}

//显示游标
void CRuler::ShowVernier(CDC* pDC,POINT point)
{
	CPen penBlue(PS_SOLID, 1, RGB(0, 0, 255));
	CPen* pOldPen = pDC->SelectObject(&penBlue);

	pDC->SetROP2(R2_NOTXORPEN);

	//erase old vernier
	pDC->MoveTo(vernier.x, width - 1);
	pDC->LineTo(vernier.x, width / 8);
	pDC->MoveTo(width - 1, vernier.y);
	pDC->LineTo(width / 8, vernier.y);

	//draw current vernier
	pDC->MoveTo(point.x, width - 1);
	pDC->LineTo(point.x, width / 8);
	pDC->MoveTo(width - 1, point.y);
	pDC->LineTo(width / 8, point.y);

	pDC->SelectObject(pOldPen);
	pDC->SetROP2(R2_COPYPEN);

	vernier = point;
}

//隐藏游标
void CRuler::HideVernier(CDC *pDC)
{
	CPen penBlue(PS_SOLID, 1, RGB(0, 0, 255));
	CPen* pOldPen = pDC->SelectObject(&penBlue);

	pDC->SetROP2(R2_NOTXORPEN);

	//erase old vernier
	pDC->MoveTo(vernier.x, width - 1);
	pDC->LineTo(vernier.x, width / 8);
	pDC->MoveTo(width - 1, vernier.y);
	pDC->LineTo(width / 8, vernier.y);

	pDC->SelectObject(pOldPen);
	pDC->SetROP2(R2_COPYPEN);

	vernier.x = vernier.y = -width;
}

//返回指定点相对于标尺的位置
int CRuler::GetCursorPosition(POINT point)
{
	CRect rect1 = hrect, rect2 = vrect;
	rect1.InflateRect(0, 0, 0, 2);
	rect2.InflateRect(0, 0, 2, 0);

	if(rect1.PtInRect(point) && rect2.PtInRect(point))
		return CP_CORNER;
	if(rect1.PtInRect(point))
		return CP_HORIZONTAL;
	if(rect2.PtInRect(point))
		return CP_VERTICAL;

	rect1.top = rect1.bottom;
	rect1.left = rect2.right;
	rect1.bottom = rect2.bottom;
	if(rect1.PtInRect(point))
		return CP_CLIENT;

	return CP_OUTER;
}
