#pragma once
#include <wx/wx.h>

class WindowFrame : public wxFrame
{
public:
    WindowFrame(const wxString &title);

private:
    void OnButtonClick(wxCommandEvent &event);

    wxButton *m_button;

    wxDECLARE_EVENT_TABLE();
};
