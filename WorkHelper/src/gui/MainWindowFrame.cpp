#include "gui/MainWindowFrame.h"

#include <wx/clipbrd.h>


wxBEGIN_EVENT_TABLE(MainWindowFrame, wxFrame)
wxEND_EVENT_TABLE()


void MainWindowFrame::onAddFile(wxCommandEvent &)
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

void MainWindowFrame::onPase(wxCommandEvent &)
{
    if (!wxTheClipboard->Open())
        return;

    if (wxTheClipboard->IsSupported(wxDF_FILENAME))
    {
        wxFileDataObject data;
        wxTheClipboard->GetData(data);
        
        // Add all files from buffer
        for (auto &file : data.GetFilenames())
        {
            m_list->addFile(file);
        }
    }
    wxTheClipboard->Close();
}
