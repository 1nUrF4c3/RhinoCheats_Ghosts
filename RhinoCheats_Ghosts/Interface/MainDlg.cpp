//=====================================================================================

#include "MainDlg.h"

//=====================================================================================

cMainDlg::cMainDlg(HINSTANCE instance) : cDialog(IDD_MAIN), _instance(instance)
{
	_messages[WM_INITDIALOG] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnInit);
	_messages[WM_COMMAND] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnCommand);
	_messages[WM_CLOSE] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnClose);

	_events[ID_CTRL_A] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnAttach);
	_events[ID_CTRL_D] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnDetach);
	_events[ID_PROCESS_ATTACH] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnAttach);
	_events[ID_PROCESS_DETACH] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnDetach);
	_events[IDC_INJECT] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnInject);
}

//=====================================================================================

cMainDlg::~cMainDlg()
{

}

//=====================================================================================

INT_PTR cMainDlg::OnInit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	cDialog::OnInit(hDlg, message, wParam, lParam);

	HICON hIcon = LoadIcon(_instance, MAKEINTRESOURCE(IDI_ICON));

	SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	DeleteObject(hIcon);

	_inject.Attach(hDlg, IDC_INJECT);
	_info.Attach(CreateStatusWindow(WS_CHILD | WS_VISIBLE, "", hDlg , IDR_STATUS));
	
	char szError[1024] = { NULL };
	wsprintf(szError, "Status: 0x%X", _lastStatus);

	_info.SetParts({ 190, -1 });
	_info.SetText(0, "Idle");
	_info.SetText(1, szError);

	return TRUE;
}

//=====================================================================================

INT_PTR cMainDlg::OnClose(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return cDialog::OnClose(hDlg, message, wParam, lParam);
}

//=====================================================================================

INT_PTR cMainDlg::OnAttach(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	Attach();

	return TRUE;
}

//=====================================================================================

INT_PTR cMainDlg::OnDetach(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	Detach();

	return TRUE;
}

//=====================================================================================

INT_PTR cMainDlg::OnInject(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::lock_guard<std::mutex> lg(_lock);
	std::thread(&cMainDlg::Inject, this).detach();

	return TRUE;
}

//=====================================================================================
