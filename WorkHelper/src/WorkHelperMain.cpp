#include "WorkHelperMain.h"


bool WorkHelperApp::OnInit()
{
	MainWindowFrame *mainWindow = new MainWindowFrame();
	mainWindow->Show();
	return true;
}

// Lounch up app
wxIMPLEMENT_APP(WorkHelperApp);