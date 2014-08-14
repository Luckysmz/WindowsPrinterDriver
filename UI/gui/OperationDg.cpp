// OperationDg.cpp : implementation of the COperationDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "precomp.h"
#include "OperationDg.h"
#include "PdkApp.h"

/////////////////////////////////////////////////////////////////////////////
//
CCommandDlg::CCommandDlg(kCommandPosition command, COLORREF tColor)
: m_commandType(command)
, m_bSendCommand(false)
, m_buzzerCombox(this, 1)
, m_nFeedPaper(0)
{
	m_hbrBkgnd = CreateSolidBrush(tColor);
}

CCommandDlg::CCommandDlg(kCommandPosition command, HBRUSH brush)
: m_commandType(command)
, m_bSendCommand(false)
, m_buzzerCombox(this, 1)
, m_nFeedPaper(0)
{
	m_hbrBkgnd.Attach(brush);
}

CCommandDlg::~CCommandDlg()
{
	DeleteObject ( m_hbrBkgnd );
}

LRESULT CCommandDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow(GetParent());

	CButton radio;
	radio.Attach(GetDlgItem(IDC_LOGO_ALIGN_LEFT));
	radio.SetCheck(1);

	radio.Detach();
	radio.Attach(GetDlgItem(IDC_LOGO_PRINT_BEFORE_CUT));
	radio.SetCheck(1);

	m_feedPaperSpin.Attach(GetDlgItem(IDC_FEED_PAPER_SPIN));
	m_feedPaper.SubclassWindow(GetDlgItem(IDC_FEED_PAPER));
	m_feedPaperSpin.SetRange(0, 100);
	m_feedPaperSpin.SetBuddy(m_feedPaper);

	m_feedPaper.LimitText(3);

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	switch (m_commandType)
	{
	case kBeginDoc:
		SetWindowText(_T("�ĵ���ʼ"));
		break;
	case kBeginPage:
		SetWindowText(_T("ҳ��ʼ"));
		break;
	case kEndPage:
		SetWindowText(_T("ҳ����"));
		break;
	case kEndDoc:
		SetWindowText(_T("�ĵ�����"));
		break;
	}

	m_wndHex.LimitText(MAX_PATH);
	m_wndHex.SetReadOnly(!m_bSendCommand);

	m_cashCombox.Attach(GetDlgItem(IDC_CASH_COMBOX));
	m_buzzerCombox.SubclassWindow(GetDlgItem(IDC_BUZZER_COMBOX));

	m_cashCombox.AddString(_T("����Ǯ��"));
	m_cashCombox.AddString(_T("��Ǯ�� #1(10ms)"));
	m_cashCombox.AddString(_T("��Ǯ�� #1(20ms)"));
	m_cashCombox.AddString(_T("��Ǯ�� #1(40ms)"));
	m_cashCombox.AddString(_T("��Ǯ�� #1(60ms)"));
	m_cashCombox.AddString(_T("��Ǯ�� #2(10ms)"));
	m_cashCombox.AddString(_T("��Ǯ�� #2(20ms)"));
	m_cashCombox.AddString(_T("��Ǯ�� #2(40ms)"));
	m_cashCombox.AddString(_T("��Ǯ�� #2(60ms)"));
	m_cashCombox.AddString(_T("��Ǯ�� #1+#2(10ms)"));
	m_cashCombox.AddString(_T("��Ǯ�� #1+#2(20ms)"));
	m_cashCombox.AddString(_T("��Ǯ�� #1+#2(40ms)"));
	m_cashCombox.AddString(_T("��Ǯ�� #1+#2(60ms)"));
	m_cashCombox.SetCurSel(0);

	m_buzzerCombox.AddString(_T("�رշ�����"));
	m_buzzerCombox.AddString(_T("���� 200ms"));
	m_buzzerCombox.AddString(_T("���� 1s"));
	m_buzzerCombox.AddString(_T("���� 2s"));
	m_buzzerCombox.AddString(_T("���� 4s"));
	m_buzzerCombox.SetCurSel(0);

	m_cutOptions.AddString(_T("�ֶ���ֽ"));
	m_cutOptions.AddString(_T("����ֽ"));
	m_cutOptions.AddString(_T("ȫ��ֽ"));
	m_cutOptions.SetCurSel(1);

	m_logPrintOptions.AddString(_T("����ӡlogoλͼ"));
	m_logPrintOptions.AddString(_T("��ӡlogoλͼ1"));
	m_logPrintOptions.AddString(_T("��ӡlogoλͼ2"));
	m_logPrintOptions.AddString(_T("��ӡlogoλͼ3"));
	m_logPrintOptions.AddString(_T("��ӡlogoλͼ4"));
	m_logPrintOptions.AddString(_T("��ӡlogoλͼ5"));
	m_logPrintOptions.AddString(_T("��ӡlogoλͼ6"));
	m_logPrintOptions.AddString(_T("��ӡlogoλͼ7"));
	m_logPrintOptions.AddString(_T("��ӡlogoλͼ8"));
	m_logPrintOptions.AddString(_T("��ӡlogoλͼ9"));
	m_logPrintOptions.SetCurSel(0);

	BOOL enableOption = m_logPrintOptions.GetCurSel() != 0;

	::EnableWindow(GetDlgItem(IDC_LOGO_ALIGN_LEFT), enableOption);
	::EnableWindow(GetDlgItem(IDC_LOGO_ALIGN_CENTER), enableOption);
	::EnableWindow(GetDlgItem(IDC_LOGO_ALIGN_RIGHT), enableOption);
	::EnableWindow(GetDlgItem(IDC_LOGO_PRINT_BEFORE_CUT), enableOption);
	::EnableWindow(GetDlgItem(IDC_LOGO_PRINT_AFTER_CUT), enableOption);

    return TRUE;
}

LRESULT CCommandDlg::OnEraseBkgnd(HDC hDC)
{
	RECT rcClient;

	GetClientRect ( &rcClient );
	FillRect ( hDC, &rcClient, m_hbrBkgnd );
	return 1;    // we painted the background
}

LRESULT CCommandDlg::OnCtlColorStatic(HDC hDC, HWND hWnd)
{
	return (LRESULT)m_hbrBkgnd.m_hBrush;
}

LRESULT CCommandDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return 0;
}

LRESULT CCommandDlg::OnDefaultButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    return 0;
}

LRESULT CCommandDlg::OnSendCommandClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DoDataExchange(true);

	m_wndHex.SetReadOnly(!m_bSendCommand);

    return 0;
}

void CCommandDlg::OnLogPrintComboxSelected(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (uNotifyCode == CBN_SELCHANGE && wndCtl.m_hWnd == m_logPrintOptions.m_hWnd)
	{
		BOOL enableOption = m_logPrintOptions.GetCurSel() != 0;

		::EnableWindow(GetDlgItem(IDC_LOGO_ALIGN_LEFT), enableOption);
		::EnableWindow(GetDlgItem(IDC_LOGO_ALIGN_CENTER), enableOption);
		::EnableWindow(GetDlgItem(IDC_LOGO_ALIGN_RIGHT), enableOption);
		::EnableWindow(GetDlgItem(IDC_LOGO_PRINT_BEFORE_CUT), enableOption);
		::EnableWindow(GetDlgItem(IDC_LOGO_PRINT_AFTER_CUT), enableOption);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
COperationDlg::COperationDlg(CPdkUIContext *pUICtxt)
: CPdkWin(pUICtxt)
{}

LRESULT COperationDlg::OnInitDialog(HWND hWnd, LPARAM /*lParam*/)
{
	return TRUE;
}

LRESULT COperationDlg::OnButtonClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	kCommandPosition commandPostion = kCommandBegin;
	switch (wID)
	{
	case IDC_BEGIN_DOC:
		commandPostion = kBeginDoc;
		break;
	case IDC_BEGIN_PAGE:
		commandPostion = kBeginPage;
		break;
	case IDC_END_PAGE:
		commandPostion = kEndPage;
		break;
	case IDC_END_DOC:
		commandPostion = kEndDoc;
		break;
	}

	if ((commandPostion > kCommandBegin) && (commandPostion < kCommandEnd))
	{
		CDC dc = GetDC();
		LOGBRUSH lb = {0};
		dc.GetCurrentBrush().GetLogBrush(&lb);
		CCommandDlg dlg(commandPostion, dc.GetCurrentBrush()/*lb.lbColor*/);
		dlg.DoModal();
	}

	return 0;
}