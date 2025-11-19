#include "gui/WindowFrame.h"
#include "gui/FileExplorerPanel.h" 


wxBEGIN_EVENT_TABLE(WindowFrame, wxFrame)
wxEND_EVENT_TABLE()

WindowFrame::WindowFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 400))
{
    wxPanel *panel = new wxPanel(this);
    auto *filePanel = new FileExplorerPanel(panel);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(filePanel, 0, wxEXPAND | wxALL, 10);
    panel->SetSizer(sizer);
}
