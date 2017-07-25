#pragma once


// CSetLineDlg dialog

class CSetLineDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetLineDlg)

public:
	CSetLineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetLineDlg();

// Dialog Data
	enum { IDD = IDD_LINE_SETDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_lineStartX;
	double m_lineStartY;
	double m_lineEndX;
	double m_lineEndY;
	CString m_lineColor;

	CComboBox *pColorSelectCombo;
	virtual BOOL OnInitDialog();
};
