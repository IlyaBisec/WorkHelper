#include "gui/MainWindowFrame.h"

#include <wx/clipbrd.h>


wxBEGIN_EVENT_TABLE(MainWindowFrame, wxFrame)
    EVT_MENU(wxID_PASTE, MainWindowFrame::onPaste) // Ctrl + V
wxEND_EVENT_TABLE()


MainWindowFrame::MainWindowFrame()
    : wxFrame(nullptr, wxID_ANY, "Work Helper", wxDefaultPosition,
        wxSize(700, 500))
{
    wxBoxSizer *root = new wxBoxSizer(wxVERTICAL);

    // Create button panel
    m_toolbar = new ToolbarPanel(this);
    root->Add(m_toolbar, 0, wxEXPAND | wxALL, 5);

    // Create file explorer panel
    m_list = new FileExplorerPanel(this);
    root->Add(m_list, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Handlers
    m_toolbar->btnAdd->Bind(wxEVT_BUTTON, &MainWindowFrame::onAdd, this);
    m_toolbar->btnDelete->Bind(wxEVT_BUTTON, &MainWindowFrame::onDelete, this);
    m_toolbar->btnRefresh->Bind(wxEVT_BUTTON, &MainWindowFrame::onRefresh, this);

    // Ctrl + V Handler
    Bind(wxEVT_MENU, &MainWindowFrame::onPaste, this, wxID_PASTE);

    SetSizer(root);
    Centre();
}

// ------------------- Handlers -----------------------------

void MainWindowFrame::onAdd(wxCommandEvent &)
{
    wxFileDialog dialog(
        this,
        "Select file",
        "",
        "",
        "Word documents (*.doc;*.docx)|*doc;*.docx|"
        "Excel documents (*.xls;*.xlsx)|*.xls;*.xlsx|"
        "All files(*.*)|*.*",
        wxFD_OPEN | wxFD_MULTIPLE
    );

    if (dialog.ShowModal() == wxID_OK)
    {
        wxArrayString paths;
        dialog.GetPaths(paths);

        for (auto &p : paths)
        {
            m_list->addFile(p);
        }
    }
}

void MainWindowFrame::onDelete(wxCommandEvent &evt)
{
    m_list->deleteSelection();
}

void MainWindowFrame::onRefresh(wxCommandEvent &evt)
{
    m_list->clear();
}

void MainWindowFrame::onPaste(wxCommandEvent &evt)
{
    if (!wxTheClipboard->Open())
        return;

    if (wxTheClipboard->IsSupported(wxDF_FILENAME))
    {
        wxFileDataObject data;
        wxTheClipboard->GetData(data);

        for (auto &filename : data.GetFilenames())
            m_list->addFile(filename);
    }

    wxTheClipboard->Close();
}


void MainWindowFrame::initUI()
{
    wxBoxSizer *vert = new wxBoxSizer(wxVERTICAL);
    
    m_list = new FileExplorerPanel(this, 44);
    vert->Add(m_list, 1, wxEXPAND | wxALL, 6);

    wxBoxSizer *horiz = new wxBoxSizer(wxHORIZONTAL);

}
