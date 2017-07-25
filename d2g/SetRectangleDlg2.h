#pragma once


// CSetRectangleDlg2 dialog

class CSetRectangleDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CSetRectangleDlg2)

public:
	CSetRectangleDlg2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetRectangleDlg2();

// Dialog Data
	enum { IDD = IDD_RECTANGLE_SETDIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_rectangleColor;
	double m_rectangleLeftTopX;
	double m_rectangleLeftTopY;
	double m_rectangleRightBottomX;
	double m_rectangleRightBottomY;
	virtual BOOL OnInitDialog();
	CComboBox *pColorSelectCombo;
};
