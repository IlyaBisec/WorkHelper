#include "gui/FileExplorerPanel.h"

#include <wx/filename.h>
#include <wx/filedlg.h>
#include <wx/tooltip.h>
#include <wx/mimetype.h>
#include <wx/datetime.h>
#include <wx/clipbrd.h>
#include <wx/statline.h>
#include <wx/dcbuffer.h>

// -----------------FileDropTarget--------------
FileDropTarget::FileDropTarget(FileExplorerPanel *owner): m_owner(owner)
{ }

bool FileDropTarget::OnDropFiles(wxCoord, wxCoord, const wxArrayString &filenames)
{
	m_owner->highLight(false);
	if (!filenames.IsEmpty())
		m_owner->setFile(filenames[0]);
	return true;
}

wxDragResult FileDropTarget::OnDragOver(wxCoord, wxCoord, wxDragResult result)
{
	m_owner->highLight(true);
	return result;
}


// -----------------FileExplorerPanel--------------

FileExplorerPanel::FileExplorerPanel(wxWindow *parent)
	:wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(400,80), wxBORDER_SIMPLE)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	// UI Elements
	//m_icon = new wxStaticBitmap(this, wxID_ANY, wxArtProvider::GetBitmap(wxART_NORMAL_FILE));
	m_filename = new wxStaticText(this, wxID_ANY, "File not selected");
	m_button = new wxButton(this, wxID_ANY, "Add");

	// Configuration
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_icon, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	sizer->Add(m_filename, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	sizer->Add(m_button, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(sizer);

	// Add Drag & Drop
	SetDropTarget(new FileDropTarget(this));

	// Events
	m_button->Bind(wxEVT_BUTTON, &FileExplorerPanel::onAddFile, this);
	Bind(wxEVT_KEY_DOWN, &FileExplorerPanel::onPaste, this);
	Bind(wxEVT_PAINT, [this](wxPaintEvent &)
		{
			wxAutoBufferedPaintDC dc(this);
			wxColor bg = m_highlighted ? wxColour(230, 250, 255) : *wxWHITE;
			dc.SetBrush(wxBrush(bg));
			dc.SetPen(wxPen(wxColour(150, 180, 100)));
			dc.DrawRectangle(GetClientRect());
		});
}

