#pragma once


// CSetNewStepDlg dialog

class CSetNewStepDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetNewStepDlg)

public:
	CSetNewStepDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetNewStepDlg();

// Dialog Data
	enum { IDD = IDD_NEWSTEP_SETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_fNewStep;
public:
	virtual BOOL OnInitDialog();
};
