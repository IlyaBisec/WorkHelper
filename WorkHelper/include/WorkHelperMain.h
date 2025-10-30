#pragma once

// WorkHelperMain.H - hedder for cpp, for main call func for init
// 25/10/2025
// ilya_bisec (c) 2025-2026

#include "wx/wx.h"

// Main window
class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString &title)
		: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 400))
	{
		// Add main title on the centre
		wxPanel *panel = new wxPanel(this, wxID_ANY);
		new wxStaticText(panel, wxID_ANY, "Hallo workHelperApp!", wxPoint(20, 20), wxDefaultSize);
	}
};

// App class
class WorkHelperApp : public wxApp
{
public:
	virtual bool OnInit() override
	{
		// Create main window
		MainWindow *window = new MainWindow("Exaple wxWidjets C++17");
		window->Show(true);
		return true;
	}
};