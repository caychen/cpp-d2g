// ButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "ButtonEx.h"


// CButtonEx

IMPLEMENT_DYNAMIC(CButtonEx, CButton)

CButtonEx::CButtonEx()
{
}

CButtonEx::~CButtonEx()
{
}


BEGIN_MESSAGE_MAP(CButtonEx, CButton)

END_MESSAGE_MAP()


// CButtonEx message handlers

void CButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Add your code to draw the specified item

	::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, DFC_BUTTON, DFCS_BUTTONPUSH);
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	//get the button text
	CString strText;
	GetWindowText(strText);

	//Draw the button text using the text color red
	LONG dist = 0;
	CBrush brush;
	CRect rect;
	brush.CreateSolidBrush(RGB(0,0,128));

	rect.CopyRect(&lpDrawItemStruct->rcItem);
	dist = 2;
	rect.left += dist;
	rect.right -= dist;
	rect.top += dist;
	rect.bottom -= dist;

	::FillRect(lpDrawItemStruct->hDC, &rect, (HBRUSH)brush.m_hObject);
	::SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
	COLORREF crOldColor;
	crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(255, 0, 0));
	
	//end
	::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	::SetTextColor(lpDrawItemStruct->hDC, crOldColor);
}

void CButtonEx::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	this->ModifyStyle(NULL,BS_OWNERDRAW);//设置自定义重绘，才会调用DrawItem函数
	CButton::PreSubclassWindow();
}