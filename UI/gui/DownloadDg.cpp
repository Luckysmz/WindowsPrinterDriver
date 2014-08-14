// DownloadDg.cpp : implementation of the CDownloadDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "DownloadDg.h"
#include "PdkApp.h"
#include <atldlgs.h>

/////////////////////////////////////////////////////////////////////////////
//
CDownloadDlg::CDownloadDlg(CPdkUIContext *pUICtxt)
: CPdkWin(pUICtxt)
{}

LRESULT CDownloadDlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	CenterWindow(GetParent());

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	DWORD dwStyle = m_logoList.GetExtendedListViewStyle();
	dwStyle |=  LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_SHOWSELALWAYS;
	m_logoList.SetExtendedListViewStyle(dwStyle);
	m_logoList.InsertColumn(0, _T("���"), LVCFMT_LEFT, 45, 0);
	m_logoList.InsertColumn(1, _T("����"), LVCFMT_LEFT, 120, 0);
	m_logoList.InsertColumn(2, _T("·��"), LVCFMT_LEFT, 255, 0);

	return TRUE;
}

LRESULT CDownloadDlg::OnButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	switch (wID)
	{
	case IDC_BROWSER:
		{
			CFileDialog dlg(true, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, _T("�����ļ�\0*.font\0�̼������ļ�\0*.dat\0�����ļ�\0*.*\0\0"));

			if (dlg.DoModal() == IDOK)
			{
				m_szDownloadFile = dlg.m_szFileName;

				DoDataExchange(false);
			}
		}
		break;
	case IDC_DOWNLOAD_FILE:
		break;
	case IDC_ADD:
		{
			if (m_logoList.GetItemCount() >= 9)
			{
				MessageBox(_T("Logoλͼ��Ŀ�ﵽ���ޣ���ɾ������Ҫ��Logo�����ԣ�"), _T("��ʾ"), MB_OK);
			}
			else
			{
				CFileDialog dlg(true, _T("*.bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, _T("λͼ�ļ�\0*.bmp\0�����ļ�\0*.*\0\0"));

				if (dlg.DoModal() == IDOK)
				{
					int index = m_logoList.GetItemCount();
					TCHAR szBuf[MAX_PATH] = {0};
					_stprintf_s(szBuf, MAX_PATH, _T("%d"), index + 1);
					m_logoList.InsertItem(index, szBuf);
					m_logoList.SetItemText(index, 0, szBuf);
					m_logoList.SetItemText(index, 1, dlg.m_szFileTitle);
					m_logoList.SetItemText(index, 2, dlg.m_szFileName);
				}
			}
		}
		break;
	case IDC_DELETE:
		if (m_logoList.GetItemCount() > 0 && m_logoList.GetSelectedIndex() >= 0)
		{
			for (int i = m_logoList.GetSelectedIndex() + 1; i < m_logoList.GetItemCount(); ++i)
			{
				TCHAR szBuf[MAX_PATH] = {0};
				_stprintf_s(szBuf, MAX_PATH, _T("%d"), i);
				m_logoList.SetItemText(i, 0, szBuf);
			}

			m_logoList.DeleteItem(m_logoList.GetSelectedIndex());
		}
		break;
	case IDC_DOWNLOAD_ALL:
		break;
	case IDC_CLEAR_ALL:
		if (m_logoList.GetItemCount() > 0)
		{
			m_logoList.DeleteAllItems();
		}
		break;
	}
	return 0;
}

LRESULT CDownloadDlg::OnNotifyHandlerEX(LPNMHDR pnmh)
{
	DoDataExchange(true);

	if (m_logoList.GetItemCount() > 0 && m_logoList.GetSelectedIndex() >= 0)
	{
		CString szText;
		m_logoList.GetItemText(m_logoList.GetSelectedIndex(), 2, szText);
		m_wndImage.SetImageFile(szText);

		DoDataExchange(false);
	}

	return 0;
}

LRESULT CDownloadDlg::OnReflectNotifications(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hWndChild = NULL;

	switch(uMsg)
	{
	case WM_DRAWITEM:
		if(wParam)	// not from a menu
			hWndChild = ((LPDRAWITEMSTRUCT)lParam)->hwndItem;
		break;
	case WM_MEASUREITEM:
		if(wParam)	// not from a menu
			hWndChild = GetDlgItem(((LPMEASUREITEMSTRUCT)lParam)->CtlID);
		break;
	case WM_COMPAREITEM:
		if(wParam)	// not from a menu
			hWndChild =  ((LPCOMPAREITEMSTRUCT)lParam)->hwndItem;
		break;
	case WM_DELETEITEM:
		if(wParam)	// not from a menu  
			hWndChild =  ((LPDELETEITEMSTRUCT)lParam)->hwndItem;
	default:
		break;
	}

	if (hWndChild == NULL)
	{
		bHandled = FALSE;
		return 1;
	}

	ATLASSERT(::IsWindow(hWndChild));
	return ::SendMessage(hWndChild, OCM__BASE + uMsg, wParam, lParam);
}