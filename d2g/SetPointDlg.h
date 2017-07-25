#pragma once


// CSetPointDlg dialog

class CSetPointDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetPointDlg)

public:
	CSetPointDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetPointDlg();

// Dialog Data
	enum { IDD = IDD_POINT_SETDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_pointX;
	double m_pointY;
	CString m_pointColor;
	CComboBox *pColorSelectCombo;
public:
	virtual BOOL OnInitDialog();
};
