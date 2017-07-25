#include "StdAfx.h"
#include "SliderCtrlEx.h"
#include "math.h"

CSliderCtrlEx::CSliderCtrlEx(void)
{
}

CSliderCtrlEx::~CSliderCtrlEx(void)
{
}
BEGIN_MESSAGE_MAP(CSliderCtrlEx, CSliderCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CSliderCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl::OnLButtonDown(nFlags, point);

	CRect   RectClient,RectChannel;
	GetClientRect(RectClient); 
	GetChannelRect(RectChannel);
	int nMax = 0;
	int nMin = 0;
	GetRange(nMin,nMax);

	int nPos = 1.0 * (nMax - nMin) * (point.x - RectChannel.left) / RectChannel.Width() + 0.5;
	SetPos(nPos + 1);
}