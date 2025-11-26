#include "gui/ToolBarPanel.h"

ToolbarPanel::ToolbarPanel(wxWindow *parent)
	: wxPanel(parent, wxID_ANY)
{
	btnAdd = new wxButton(this, wxID_ANY, "Add file");
	btnDelete = new wxButton(this, wxID_ANY, "Delete");
	btnRefresh = new wxButton(this, wxID_ANY, "Refresh");

	layout();
}

void ToolbarPanel::layout()
{
	wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);

	box->Add(btnAdd, 0, wxRIGHT, 5);
	box->Add(btnDelete, 0, wxRIGHT, 5);
	box->Add(btnRefresh, 0);

	SetSizer(box);
}
