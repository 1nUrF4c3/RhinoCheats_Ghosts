//=====================================================================================

#include "MainDlg.h"

//=====================================================================================

void cMainDlg::Attach()
{
	_info.SetText(0, "Attaching to iw6mp64_ship.exe");

	_lastStatus = _process.Attach(L"iw6mp64_ship.exe");

	char szError[1024] = { NULL };
	wsprintf(szError, "Status: 0x%X", _lastStatus);

	if (!NT_SUCCESS(_lastStatus)) 
		cMessage::ShowError(_hwnd, "Failed to attach to Call of Duty: Ghosts - Multiplayer.");

	else 
		cMessage::ShowInfo(_hwnd, "Successfully attached to Call of Duty: Ghosts - Multiplayer.");

	_info.SetText(0, "Idle");

	_info.SetText(1, szError);
}

//=====================================================================================

void cMainDlg::Detach()
{
	_info.SetText(0, "Detaching from iw6mp64_ship.exe");

	_lastStatus = _process.Detach();

	char szError[1024] = { NULL };
	wsprintf(szError, "Status: 0x%X", _lastStatus);

	if (!NT_SUCCESS(_lastStatus)) 
		cMessage::ShowError(_hwnd, "Failed to detach from Call of Duty: Ghosts - Multiplayer.");

	else 
		cMessage::ShowInfo(_hwnd, "Successfully detached from Call of Duty: Ghosts - Multiplayer.");

	_info.SetText(0, "Idle");

	_info.SetText(1, szError);
}

//=====================================================================================

void cMainDlg::Inject()
{
	_info.SetText(0, "Injecting into iw6mp64_ship.exe");
	
	_inject.Disable();

	if (_process.valid())
	{
		HRSRC hResource = FindResource(_instance, MAKEINTRESOURCE(IDR_PAYLOAD), "PAYLOAD");
		DWORD dwPayloadSize = SizeofResource(_instance, hResource);
		LPVOID pPayloadData = VirtualAlloc(NULL, dwPayloadSize, MEM_COMMIT, PAGE_READWRITE);

		CopyMemory(pPayloadData, LockResource(LoadResource(_instance, hResource)), dwPayloadSize);

		_lastStatus = _process.mmap().MapImage(dwPayloadSize, pPayloadData, false, blackbone::WipeHeader).status;

		char szError[1024] = { NULL };
		wsprintf(szError, "Status: 0x%X", _lastStatus);

		if (!NT_SUCCESS(_lastStatus)) 
			cMessage::ShowError(_hwnd, "Failed to inject into Call of Duty: Ghosts - Multiplayer.");

		else 
			cMessage::ShowInfo(_hwnd, "Successfully injected into Call of Duty: Ghosts - Multiplayer.");

		_info.SetText(1, szError);
	}

	else 
		cMessage::ShowWarning(_hwnd, "Call of Duty: Ghosts - Multiplayer is not attached.");

	_inject.Enable();

	_info.SetText(0, "Idle");
}

//=====================================================================================