#pragma once


// CSetArcDlg dialog

class CSetArcDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetArcDlg)

public:
	CSetArcDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetArcDlg();

// Dialog Data
	enum { IDD = IDD_ARC_SETDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_arcColor;
	double m_arcCenterX;
	double m_arcCenterY;
	double m_arcRadius;
	double m_arcStartAngle;
	double m_arcEndAngle;
	CComboBox *pColorSelectCombo;
public:
	virtual BOOL OnInitDialog();
public:
	int m_arcDirection;
};
