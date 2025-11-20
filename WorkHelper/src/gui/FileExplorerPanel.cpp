#include "gui/FileExplorerPanel.h"

#include <wx/mimetype.h>
#include <wx/clipbrd.h>
#include <wx/filename.h>


// -----------------FileDropTarget--------------

wxDragResult FileDropTarget::OnEnter(wxCoord x, wxCoord y, wxDragResult result)
{
	// Turn on high light
	m_owner->highLight(true);
	return wxDragCopy;
}

void FileDropTarget::OnLeave()
{
	m_owner->highLight(false);
}

bool FileDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filanames)
{
	m_owner->highLight(false);

	// Add any file
	for (auto &f : filanames)
		m_owner->addFile(f);

	return true;
}


// -----------------FileExplorerPanel--------------

FileExplorerPanel::FileExplorerPanel(wxWindow *parent)
	:wxDataViewListCtrl(parent, wxID_ANY, wxDefaultPosition, wxSize(200,80))
{
	//AppendIconTextColumn("File");

	// Handles mouse movement for tooltip metadata
	Bind(wxEVT_MOTION, &FileExplorerPanel::onMouseMove, this);

	// Set drag and drop object
	SetDropTarget(new FileDropTarget(this));
}

wxIcon FileExplorerPanel::getFileIcon(const wxString &path)
{
	auto extension = wxFileName(path).GetExt();

	// Find MIME type by extension (wxWidgets find icon automatically)
	wxFileType *fileType = wxTheMimeTypesManager->GetFileTypeFromExtension(extension);

	if (!fileType)
		return wxIcon();

	wxIconLocation location;

	if (fileType->GetIcon(&location))
		return wxIcon(location);

	return wxIcon();
}

void FileExplorerPanel::addFile(const wxString &path)
{
	wxFileName fileName(path);
	wxIcon icon = getFileIcon(path);

	// Icon + Text
	wxDataViewIconText value(fileName.GetFullName(), icon);

	// Writing recieved info
	wxVector<wxVariant> cols;
	cols.push_back(wxVariant(value));

	// Add string
	AppendItem(cols);

	// Save full file path(inviseble)
	m_files.push_back(path);
}

wxString FileExplorerPanel::getFilePath(unsigned row) const
{
	if (row < m_files.size())
	{
		return m_files[row];
	}

	return "";
}

void FileExplorerPanel::highLight(bool enable)
{
	SetBackgroundColour(enable ? wxColour(220, 240, 255) : *wxWHITE);
	Refresh();
}

void FileExplorerPanel::onMouseMove(wxMouseEvent &evt)
{
	// Get cursor position
	wxPoint point = evt.GetPosition();

	// If cursor nad string
	wxDataViewItem item;
	wxDataViewColumn *column = nullptr;

	HitTest(point, item, column);
	
	if (item.IsOk())
	{
		// Convert wxDataViewItem to string index
		int row = ItemToRow(item);

		if (row != wxNOT_FOUND && static_cast<size_t>(row) < m_files.size())
		{
			wxString file = m_files[row];
			wxFileName fileName(file);

			wxULongLong size = fileName.GetSize();
			wxString sizeStr;

			if (size.GetValue() > 1024 * 1024)
				sizeStr = wxString::Format("%.2f MB", (double) size.GetValue() / (1024.0 * 1024.0));
			else
				sizeStr = wxString::Format("%.2f KB", (double) size.GetValue() / 1024.0);

			wxString tip;
			tip << "Name: " << fileName.GetFullName() << "\n";
			tip << "Size: " << sizeStr << "\n";
			tip << "Extension: " << fileName.GetExt() << "\n";
			tip << "Changed date: " << fileName.GetModificationTime().FormatISOCombined();

			SetToolTip(tip);
		}
		else
		{
			// If item is valid but a valid string could not be retrieved
			UnsetToolTip();
		}
	}
	else
	{
		// The cursor is not above the line - remove the tooltip
		UnsetToolTip();
	}
	
	evt.Skip();
}
