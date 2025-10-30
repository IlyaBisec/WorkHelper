#pragma once
#include <wx/wx.h>

class WorkHelperFrame : public wxFrame
{
public:
    WorkHelperFrame(const wxString &title);

private:
    void OnButtonClick(wxCommandEvent &event);

    wxButton *m_button;

    wxDECLARE_EVENT_TABLE();
};
