#ifndef _TOOLBARPANEL_H_
#define _TOOLBARPANEL_H_
#endif // !_TOOLBARPANEL_H_
#pragma once

// ToolbarPanel.H - header that contains buttons
// 23/11/2025
// ilya_bisec (c) 2025-2026

#include <wx/wx.h>

class ToolbarPanel : public wxPanel
{
public:
	ToolbarPanel(wxWindow *parent);

	wxButton *btnAdd = nullptr;
	wxButton *btnDelete = nullptr;
	wxButton *btnRefresh = nullptr;

private:
	void layout();
};