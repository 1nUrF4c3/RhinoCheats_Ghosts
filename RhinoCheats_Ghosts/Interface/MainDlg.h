//=====================================================================================

#pragma once

#include "../Resources/Resource.h"
#include "WinAPI/Dialog.hpp"
#include "WinAPI/Button.hpp"
#include "WinAPI/StatusBar.hpp"
#include "WinAPI/Message.hpp"

#include <BlackBone/Process/Process.h>

//=====================================================================================

class cMainDlg : public cDialog
{
public:

	cMainDlg(HINSTANCE instance);
	~cMainDlg();

private:

	void Attach();
	void Detach();
	void Inject();

	DLG_HANDLER(OnInit);
	DLG_HANDLER(OnClose);

	DLG_HANDLER(OnAttach);
	DLG_HANDLER(OnDetach);
	DLG_HANDLER(OnInject);

protected:

	blackbone::Process _process;
	std::mutex _lock;

	ctrl::cButton _inject;
	ctrl::cStatusBar _info;

	HINSTANCE _instance = NULL;
	NTSTATUS _lastStatus = NULL;
};

//=====================================================================================