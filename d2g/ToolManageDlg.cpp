// ToolManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "d2g.h"
#include "ToolManageDlg.h"
#include <locale.h>

// CToolManageDlg dialog
extern CString GetModuleFolderPath();
BOOL bAutoBreath = FALSE;

std::vector<int> tmSelect;
std::vector<ToolManage> tmVector;
IMPLEMENT_DYNAMIC(CToolManageDlg, CDialog)

CToolManageDlg::CToolManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolManageDlg::IDD, pParent)
	, m_bModifyFlag(FALSE)
{
	//06-30修改Cf
	//Start
	//tmSelect.clear();
	//End
}

CToolManageDlg::~CToolManageDlg()
{
	/*if(fp)
	{
		fclose(fp);
	}*/
}

void CToolManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CToolManageDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_TOOLNUMBER_LIST, &CToolManageDlg::OnCbnSelchangeToolNumberList)
	ON_BN_CLICKED(IDB_BTN_MODIFY, &CToolManageDlg::OnBnClickedBtnModify)
	ON_BN_CLICKED(IDOK, &CToolManageDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CToolManageDlg message handlers

BOOL CToolManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CComboBox* pToolComboBox = (CComboBox*)GetDlgItem(IDC_TOOLNUMBER_LIST);
	for(int i = 0; i < tmVector.size(); ++i)
	{
		pToolComboBox->InsertString(i, tmVector.at(i).ToolNo);
	}

	int pos = 0;
	pToolComboBox->SetCurSel(pos);
	CString str;
	str.Format(L"%s", tmVector[pos].ToolName);
	SetDlgItemText(IDC_TOOL_NAME, str);

	str.Format(L"%g", tmVector[pos].ToolRadius);
	SetDlgItemText(IDC_TOOL_RADIUS, str);

	str.Format(L"%d", tmVector[pos].RotationRate);
	SetDlgItemText(IDC_ROTATION_RATE, str);

	((CEdit*)GetDlgItem(IDC_TOOL_NAME))->SetReadOnly();
	((CEdit*)GetDlgItem(IDC_TOOL_RADIUS))->SetReadOnly();
	((CEdit*)GetDlgItem(IDC_ROTATION_RATE))->SetReadOnly();

	//Start Cf_Lsd_06_30
	for(int i = 0; i < tmSelect.size(); ++i)
	{
		((CButton*)GetDlgItem(IDC_CHECK_T1 + tmSelect.at(i)))->SetCheck(TRUE);
		//tmSelect.push_back(i);
	}
	//End Cf_Lsd_06_30

	//Start Cf_Lsd_10_22
	((CButton*)GetDlgItem(IDC_CHECK_BREATH))->SetCheck(bAutoBreath);
	//End Cf_Lad_10_22

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CToolManageDlg::OnCbnSelchangeToolNumberList()
{
	// TODO: Add your control notification handler code here
	CComboBox *pToolList = (CComboBox*)GetDlgItem(IDC_TOOLNUMBER_LIST);
	int nSelectedIndex = pToolList->GetCurSel();
	CString str;
	str.Format(L"%s", tmVector[nSelectedIndex].ToolName);
	SetDlgItemText(IDC_TOOL_NAME, str);

	str.Format(L"%g", tmVector[nSelectedIndex].ToolRadius);
	SetDlgItemText(IDC_TOOL_RADIUS, str);

	str.Format(L"%d", tmVector[nSelectedIndex].RotationRate);
	SetDlgItemText(IDC_ROTATION_RATE, str);
}

void CToolManageDlg::OnBnClickedBtnModify()
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_TOOL_RADIUS))->SetReadOnly(m_bModifyFlag);
	((CEdit*)GetDlgItem(IDC_ROTATION_RATE))->SetReadOnly(m_bModifyFlag);
	((CEdit*)GetDlgItem(IDC_TOOL_NAME))->SetReadOnly(m_bModifyFlag);

	m_bModifyFlag = !m_bModifyFlag;
	if(m_bModifyFlag)
	{
		SetDlgItemText(IDB_BTN_MODIFY, L"保存");
	}
	else
	{
		SetDlgItemText(IDB_BTN_MODIFY, L"修改");

		int nSelected = ((CComboBox*)GetDlgItem(IDC_TOOLNUMBER_LIST))->GetCurSel();

		CString strNewValue;
		//刀具名称
		GetDlgItemText(IDC_TOOL_NAME, strNewValue);
		_tcscpy_s(tmVector.at(nSelected).ToolName, strNewValue);

		//刀具半径
		GetDlgItemText(IDC_TOOL_RADIUS, strNewValue);
		double fNewValue = _wtof(strNewValue);
		tmVector.at(nSelected).ToolRadius = fNewValue;

		//主轴转速
		int NewSpeedValue = GetDlgItemInt(IDC_ROTATION_RATE);
		tmVector.at(nSelected).RotationRate = NewSpeedValue;
		
		SaveDataToConfigureFile();
	}
}

void CToolManageDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	tmSelect.clear();
 	for(int i = 0; i < tmVector.size(); ++i)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_T1 + i))->GetCheck())
		{
			tmSelect.push_back(i);
		}
	}

	//修改bAutoBreath的值
	if(((CButton*)GetDlgItem(IDC_CHECK_BREATH))->GetCheck() == 1)
		bAutoBreath = TRUE;
	else
		bAutoBreath = FALSE;

	if(m_bModifyFlag)
	{
		int iRes = MessageBox(L"修改的数据未保存，是否直接退出？", L"提示", MB_OKCANCEL | MB_ICONWARNING);
		if(IDOK != iRes)
			return ;
	}
	
	OnOK();
}

void CToolManageDlg::SaveDataToConfigureFile()
{
	TCHAR* oldLocale = _tcsdup( _tsetlocale(LC_CTYPE, NULL) );
	_tsetlocale( LC_CTYPE, L"chs" );//设定<ctpye.h>中字符处理方式

	ToolManage tm;
	ZeroMemory(&tm, sizeof(tm));

	CString strExeFolderName = GetModuleFolderPath();
	CString ConfigureFilePath = strExeFolderName + _T("\\") + _T("ToolManage.ini");

	FILE *fp = NULL;
	_tfopen_s(&fp, ConfigureFilePath, L"w");
	if(fp)
	{
		for(int i = 0; i < tmVector.size(); ++i)
		{
			tm = tmVector.at(i);
			fwprintf_s(fp, L"%s %s %g %d", 
				tm.ToolNo, 
				tm.ToolName, 
				tm.ToolRadius, 
				tm.RotationRate);

			if(i != tmVector.size() - 1)
				fwprintf_s(fp,L"\n");
		}
		fclose(fp);
	}

	_tsetlocale( LC_CTYPE, oldLocale );
	free( oldLocale );//还原区域设定
}