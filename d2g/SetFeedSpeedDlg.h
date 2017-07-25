#pragma once


// CSetFeedSpeedDlg dialog

class CSetFeedSpeedDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetFeedSpeedDlg)

public:
	CSetFeedSpeedDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetFeedSpeedDlg();

// Dialog Data
	enum { IDD = IDD_FEEDSPEED_SETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_iNewFeedSpeed;
public:
	virtual BOOL OnInitDialog();
};
