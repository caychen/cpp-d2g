#pragma once


// CButtonEx

class CButtonEx : public CButton
{
	DECLARE_DYNAMIC(CButtonEx)

public:
	CButtonEx();
	virtual ~CButtonEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void PreSubclassWindow();

};