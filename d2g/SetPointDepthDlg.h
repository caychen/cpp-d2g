#pragma once


// CSetPointDepthDlg dialog

class CSetPointDepthDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetPointDepthDlg)

public:
	CSetPointDepthDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetPointDepthDlg();

// Dialog Data
	enum { IDD = IDD_SETPOINTDEPTHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_fPointDepth;
};
