#ifndef _MAIN_WINDOW_FRAME_H_
#define _MAIN_WINDOW_FRAME_H_
#endif // !_MAIN_WINDOW_FRAME_H_
#pragma once

// MainWindowFrame.H - Main application window
// 30/10/2025
// ilya_bisec (c) 2025-2026


#include "FileExplorerPanel.h"
#include "ToolBarPanel.h"

// Main window app
class MainWindowFrame : public wxFrame
{
public:
    MainWindowFrame();

    void initUI();

private:
    ToolbarPanel *m_toolbar = nullptr;
    FileExplorerPanel *m_list = nullptr;

    // Event handlers
    void onAdd(wxCommandEvent &evt);
    void onDelete(wxCommandEvent &evt);
    void onRefresh(wxCommandEvent &evt);
    void onPaste(wxCommandEvent &evt);

    wxDECLARE_EVENT_TABLE();
};
