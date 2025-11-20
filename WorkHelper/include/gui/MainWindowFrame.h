#ifndef _MAIN_WINDOW_FRAME_H_
#define _MAIN_WINDOW_FRAME_H_
#endif // !_MAIN_WINDOW_FRAME_H_
#pragma once

// MainWindowFrame.H - Main application window
// 30/10/2025
// ilya_bisec (c) 2025-2026


#include "FileExplorerPanel.h"

// Main window app
class MainWindowFrame : public wxFrame
{
public:
    MainWindowFrame()
        : wxFrame(nullptr, wxID_ANY, "Custom File List", wxDefaultPosition, wxSize(500, 400))
    {
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        // Create custom list
        m_list = new FileExplorerPanel(this);
        sizer->Add(m_list, 1, wxEXPAND | wxALL, 5);

        // Button "Add file"
        wxButton *addButton = new wxButton(this, wxID_ANY, "Add file");
        sizer->Add(addButton, 0, wxALIGN_LEFT | wxALL, 5);

        // Button click handler
        addButton->Bind(wxEVT_BUTTON, &MainWindowFrame::onAddFile, this);

        // Ctrl + V (paste files from buffer)
        Bind(wxEVT_MENU, &MainWindowFrame::onPase, this, wxID_PASTE);

        SetSizer(sizer);
        Centre();
    }

private:
    // File dialog
    void onAddFile(wxCommandEvent &);
    // Paste from buffer Ctrl + V
    void onPase(wxCommandEvent &);

    FileExplorerPanel *m_list;

    wxDECLARE_EVENT_TABLE();
};
