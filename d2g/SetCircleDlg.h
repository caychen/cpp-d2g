#pragma once


// CSetCircleDlg dialog

class CSetCircleDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetCircleDlg)

public:
	CSetCircleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetCircleDlg();

// Dialog Data
	enum { IDD = IDD_CIRCLE_SETDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_circleColor;
	double m_centerX;
	double m_centerY;
	double m_fCircleRadius;
	CComboBox *pColorSelectCombo;
	virtual BOOL OnInitDialog();
};
