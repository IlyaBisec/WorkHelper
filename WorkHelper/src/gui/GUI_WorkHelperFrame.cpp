#include "gui/GUI_WorkHelperFrame.h"


wxBEGIN_EVENT_TABLE(WorkHelperFrame, wxFrame)
EVT_BUTTON(1001, WorkHelperFrame::OnButtonClick)
wxEND_EVENT_TABLE()

WorkHelperFrame::WorkHelperFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 400))
{
    wxPanel *panel = new wxPanel(this, wxID_ANY);

    m_button = new wxButton(panel, 1001, "Нажми меня", wxPoint(20, 60));
    new wxStaticText(panel, wxID_ANY, "Привет, wxWidgets + C++17!", wxPoint(20, 20));
}

void WorkHelperFrame::OnButtonClick(wxCommandEvent &event)
{
    wxMessageBox("Ты нажал кнопку!", "Событие", wxOK | wxICON_INFORMATION);
}
