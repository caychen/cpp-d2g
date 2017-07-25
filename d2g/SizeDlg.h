#pragma once


// CSizeDlg dialog

class CSizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CSizeDlg)

public:
	CSizeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSizeDlg();

// Dialog Data
	enum { IDD = IDD_SIZEOFRECTANGLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int m_length;
	int m_width;
};
