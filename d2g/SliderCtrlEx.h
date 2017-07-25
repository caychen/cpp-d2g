#pragma once
#include "afxcmn.h"

class CSliderCtrlEx :
	public CSliderCtrl
{
public:
	CSliderCtrlEx(void);
	virtual ~CSliderCtrlEx(void);

public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
