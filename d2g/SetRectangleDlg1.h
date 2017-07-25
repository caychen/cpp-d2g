#pragma once


// CSetRectangleDlg1 dialog

class CSetRectangleDlg1 : public CDialog
{
	DECLARE_DYNAMIC(CSetRectangleDlg1)

public:
	CSetRectangleDlg1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetRectangleDlg1();

// Dialog Data
	enum { IDD = IDD_RECTANGLE_SETDIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_rectangleColor;
	double m_rectangleLeftTopX;
	double m_rectangleLeftTopY;
	double m_rectangleWidth;
	double m_rectangleHeight;
	CComboBox *pColorSelectCombo;

public:
	virtual BOOL OnInitDialog();
};
