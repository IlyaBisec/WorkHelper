#include "gui/FileExplorerPanel.h"

#include <wx/artprov.h>
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
    m_icon = new wxStaticBitmap(this, wxID_ANY, wxArtProvider::GetBitmap(wxART_NORMAL_FILE));
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

void FileExplorerPanel::highLight(bool enable)
{
	if (m_highlighted != enable)
	{
		m_highlighted = enable;
		Refresh();
	}
}

void FileExplorerPanel::setFile(const wxString &path)
{
	m_path = path;
	updateDisplay();
}

void FileExplorerPanel::onAddFile(wxCommandEvent &event)
{
	wxFileDialog dialog
	(
		this,
		"Select file",
		"",
		"",
		"Word documents (*.doc;*.docx)|*doc;*.docx|"
		"Excel documents (*.xls;*.xlsx)|*.xls;*.xlsx|"
		"All files(*.*)|*.*",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);

//	dialog.SetFilterIndex(2);

	if (dialog.ShowModal() == wxID_OK)
		setFile(dialog.GetPath());
}


void FileExplorerPanel::onPaste(wxKeyEvent &event)
{
	if (event.ControlDown() && event.GetKeyCode() == 'V')
	{
		if (wxTheClipboard->Open())
		{
			if (wxTheClipboard->IsSupported(wxDF_FILENAME))
			{
				wxFileDataObject data;
				wxTheClipboard->GetData(data);
				if (!data.GetFilenames().IsEmpty())
					setFile(data.GetFilenames()[0]);
			}
			wxTheClipboard->Close();
		}
	}
	event.Skip();
}

void FileExplorerPanel::updateDisplay()
{
	if (m_path.IsEmpty())
	{
		m_icon->SetBitmap(wxArtProvider::GetBitmap(wxART_NORMAL_FILE));
		m_filename->SetLabel("File not selected");
		SetToolTip("");
		return;
	}

	wxFileName wx_filename(m_path);
	wxString name = wx_filename.GetFullName();
	wxString ext = wx_filename.GetExt();

	// Get icon of file type
	wxIcon icon;
	wxFileType *type = wxTheMimeTypesManager->GetFileTypeFromExtension(ext);
	if (type)
	{
		wxIconLocation loc;
		if (type->GetIcon(&loc))
			icon.CopyFromBitmap(wxBitmap(loc.GetFileName(), wxBITMAP_TYPE_ICO));
		delete type;
	}

	if (icon.IsOk())
		m_icon->SetIcon(icon);
	else
		m_icon->SetBitmap(wxArtProvider::GetBitmap(wxART_NORMAL_FILE));

	m_filename->SetLabel(name);
	SetToolTip(getFileToolTip(m_path));
}


wxString FileExplorerPanel::getFileToolTip(const wxString &path)
{
	wxFileName wx_filename(path);
	if (!wx_filename.FileExists()) return "File not found";

	wxULongLong size = wx_filename.GetSize();
	wxString sizeStr;

	if (size.GetValue() > 1024 * 1024)
		sizeStr = wxString::Format("%.2f MB", (double) size.GetValue() / (1024.0 * 1024.0));
	else
		sizeStr = wxString::Format("%.2f KB", (double) size.GetValue() / 1024.0);

	wxDateTime dateTime;
	wx_filename.GetTimes(nullptr, &dateTime, nullptr);

	wxString info;
	info << "Path: " << wx_filename.GetFullPath() << "\n"
		<< "Size: " << sizeStr << "\n"
		<< "Changed date: " << dateTime.FormatISOCombined(' ') << "\n"
		<< "Extension: " << wx_filename.GetExt();
	
	return info;
}