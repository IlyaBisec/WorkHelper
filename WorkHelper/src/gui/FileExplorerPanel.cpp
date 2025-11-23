#include "gui/FileExplorerPanel.h"

#include <wx/mimetype.h>
#include <wx/clipbrd.h>
#include <wx/filename.h>
#include <wx/artprov.h>
#include <wx/dcbuffer.h>


// -----------------FileDropTarget---------------------------

bool FileDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filanames)
{
	if (!m_owner) return false;

	// Add any file
	for (const auto &f : filanames)
		m_owner->addFile(f);

	return true;
}


// -----------------FileExplorerPanel-------------------------
// Register events
wxBEGIN_EVENT_TABLE(FileExplorerPanel, wxScrolledWindow)
	EVT_PAINT(FileExplorerPanel::onPaint)
	EVT_MOTION(FileExplorerPanel::onMouseMove)
	EVT_LEFT_DOWN(FileExplorerPanel::onLeftDown)
	EVT_LEAVE_WINDOW(FileExplorerPanel::onLeaveWindow)
	EVT_CHAR_HOOK(FileExplorerPanel::onCharHook)
wxEND_EVENT_TABLE()



FileExplorerPanel::FileExplorerPanel(wxWindow *parent, int rowHeight)
	:wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE | wxBORDER_NONE),
	m_rowHeight(rowHeight),
	m_hoverIndex(-1),
	m_anchorIndex(-1)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	// Vertical scroll in row unit
	SetScrollRate(0, m_rowHeight);

	// Enable file drop
	SetDropTarget(new FileDropTarget(this));

	// Catch keys
	SetFocus();
}


void FileExplorerPanel::addFile(const wxString &path)
{
	wxFileName fileName(path);

	if (!fileName.Exists())
	{
		int result = wxMessageBox
		(
			"File does not exist:\n" + path + "\nRemove from list?",
			"Missing file",
			wxYES_NO | wxICON_WARNING
		);
		
		// If result = no, exit
		if(result != wxYES)
			return;

		// Add bad file
		Item item;
		item.fullPath = path;
		item.name = fileName.GetFullName() + " (missing)";
		item.iconBmp = wxArtProvider::GetBitmap(wxART_ERROR, wxART_OTHER, wxSize(32, 32));
		m_items.push_back(item);
		SetVirtualSize(-1, (int) m_items.size() * m_rowHeight);
		Refresh();
		
		return;
	}

	Item item;
	item.fullPath = path;
	item.name = fileName.GetFullName();

	// Load and scale icon for row height (add some indent) 
	int indent = 12;
	int iconSize = std::max(16, m_rowHeight - indent);
	item.iconBmp = loadAndScaleIcon(path, iconSize);

	m_items.push_back(std::move(item));

	// Refresh size and repaint
	SetVirtualSize(-1, (int) m_items.size() * m_rowHeight);
	Refresh();
}

void FileExplorerPanel::clear()
{
	m_items.clear();
	m_selected.clear();
	m_hoverIndex = -1;
	m_anchorIndex = -1;
	SetVirtualSize(-1, 0);
	Refresh();
}

void FileExplorerPanel::copySelectionToClipboard()
{
	if (m_selected.empty()) return;

	wxArrayString files;
	for (int index : m_selected)
	{
		if (index >= 0 && index < (int) m_items.size())
		{
			files.Add(m_items[index].fullPath);
		}
		setFilesToClipboard(files);
	}
}

void FileExplorerPanel::pasteFromClipboard()
{
	wxArrayString files = getFilesFromClipboard();
	if (files.IsEmpty()) return;

	for (const auto &f : files)
	{
		addFile(f);
	}
}

void FileExplorerPanel::deleteSelection()
{
	if (m_selected.empty()) return;

	// Colletct indices into vector and sort them in descending order
	std::vector<int> indices;

	for (int i : m_selected)
	{
		indices.push_back(i);
	}

	std::sort(indices.begin(), indices.end(), std::greater<int>());

	deleteIndices(indices);

	m_selected.clear();
	m_anchorIndex = -1;
	Refresh();
}

void FileExplorerPanel::deleteIndices(const std::vector<int> &indicesDesc)
{
	for (int index : indicesDesc)
	{
		if (index >= 0 && index < (int) m_items.size())
		{
			m_items.erase(m_items.begin() + index);
		}
	}
	// Update virtual size
	SetVirtualSize(-1, (int) m_items.size() * m_rowHeight);
}

// ------------------------Clipboard-----------------------------

void FileExplorerPanel::setFilesToClipboard(const wxArrayString &files)
{
	if (files.IsEmpty()) return;

	if (!wxTheClipboard->Open()) return;
	wxTheClipboard->Clear();

	wxFileDataObject *data = new wxFileDataObject();
	for (const auto &f : files)
	{
		data->AddFile(f);
	}

	// wxTheClipboard takes ownership of data
	wxTheClipboard->SetData(data);
	wxTheClipboard->Close();
}

wxArrayString FileExplorerPanel::getFilesFromClipboard()
{
	wxArrayString result;

	if (!wxTheClipboard->Open())
		return result;

	if (wxTheClipboard->IsSupported(wxDF_FILENAME))
	{
		wxFileDataObject data;

		if (wxTheClipboard->GetData(data))
		{
			result = data.GetFilenames();
		}
	}

	wxTheClipboard->Close();
	return result;
}

// ------------------------Icon loader + scaler ------------------

wxBitmap FileExplorerPanel::loadAndScaleIcon(const wxString &path, int size)
{
	if (path.IsEmpty()) return wxBitmap();

	// Get system icon with help MIME/FileType
	wxFileName fileName(path);
	wxString extension = fileName.GetExt();

	wxFileType *fileType = wxTheMimeTypesManager->GetFileTypeFromExtension(extension);
	if (!fileType) return wxBitmap();

	wxIconLocation location;
	if (!fileType->GetIcon(&location)) return wxBitmap();

	wxIcon icon(location);
	if (!icon.IsOk()) return wxBitmap();

	// Convert icon in Bitmap and scale this via wxImage
	wxBitmap bmp(icon);
	if (!bmp.IsOk()) return wxBitmap();

	wxImage image = bmp.ConvertToImage();
	if (!image.IsOk()) return wxBitmap();

	// Maintain proportions - scale to square 
	image = image.Scale(size, size, wxIMAGE_QUALITY_HIGH);
	wxBitmap scaled(image);

	return scaled;
}

// ------------------------ Events -------------------------------

int FileExplorerPanel::hitTestItem(const wxPoint &position) const
{
	// Get coordinates inside "virtual" area (include scrolling)
	int x, y;
	CalcUnscrolledPosition(position.x, position.y, &x, &y); // Return posistion without scroll

	if (y < 0) return -1;
	int index = y / m_rowHeight;
	if (index < 0 || index >= (int) m_items.size()) return -1;

	return index;
}

void FileExplorerPanel::onPaint(wxPaintEvent &evt)
{
	wxAutoBufferedPaintDC dc(this);
	PrepareDC(dc);

	// Background
	dc.SetBrush(*wxWHITE_BRUSH);
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.DrawRectangle(GetClientRect());

	wxRect clientRect = GetClientRect();

	// Calculate visible range so as not to draw outside the visible range
	int x0, y0;
	GetViewStart(&x0, &y0);

	int firstVisible = (y0 * m_rowHeight) / m_rowHeight;
	int h = clientRect.GetHeight();
	int lastVisible = std::min((int) m_items.size() - 1, firstVisible + h / m_rowHeight + 1);

	for (int i = 0; i < (int) m_items.size(); ++i)
	{
		int y = i * m_rowHeight;
		wxRect rc(0, y, clientRect.GetWidth(), m_rowHeight);
		drawItem(dc, i, rc);
	}
}

void FileExplorerPanel::drawItem(wxDC &dc, int index, const wxRect &rect)
{
	// Highlight - hover
	if (index == m_hoverIndex)
	{
		dc.SetBrush(wxColour(230, 240, 255));
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.DrawRectangle(rect);
	}
	// Selected
	if (m_selected.find(index) != m_selected.end())
	{
		dc.SetBrush(wxColour(200, 220, 255));
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.DrawRectangle(rect);
	}
	// Indent
	const int indent = 8;
	int iconX = rect.x + indent;
	int iconY = rect.y + (m_rowHeight - (m_items[index].iconBmp.IsOk() ? m_items[index].iconBmp.GetHeight() : 16)) / 2;

	// Draw icon if exists
	if (m_items[index].iconBmp.IsOk())
	{
		dc.DrawBitmap(m_items[index].iconBmp, iconX, iconY, true);
	}

	// Text
	dc.SetTextForeground(*wxBLACK);
	int textX = iconX + (m_items[index].iconBmp.IsOk() ? m_items[index].iconBmp.GetWidth() + indent : 24);
	int textY = rect.y + (m_rowHeight - dc.GetTextExtent(m_items[index].name).GetHeight()) / 2;
	dc.DrawText(m_items[index].name, textX, textY);
}

void FileExplorerPanel::onMouseMove(wxMouseEvent &evt)
{
	int index = hitTestItem(evt.GetPosition());
	if (index != m_hoverIndex)
	{
		m_hoverIndex = index;
		Refresh();
	}

	// Tooltip: show metadata on hover
	if (m_hoverIndex >= 0 && m_hoverIndex < (int) m_items.size())
	{
		const Item &item = m_items[m_hoverIndex];
		wxFileName fileName(item.fullPath);

		wxULongLong size = fileName.GetSize();
		wxString sizeStr;

		if (size.GetValue() > 1024 * 1024)
			sizeStr = wxString::Format("%.2f MB", size.ToDouble() / (1024.0 * 1024.0));
		else
			sizeStr = wxString::Format("%.2f KB", size.ToDouble() / 1024.0);

		wxString tip;
		tip << "Name: " << fileName.GetFullName() << "\n";
		tip << "Size: " << sizeStr << "\n";
		tip << "Extension: " << fileName.GetExt() << "\n";
		tip << "Modified: " << fileName.GetModificationTime().FormatISOCombined();

		SetToolTip(tip);
	}
	else
	{
		// If item is valid but a valid string could not be retrieved
		UnsetToolTip();
	}
	
	evt.Skip();
}

void FileExplorerPanel::onLeftDown(wxMouseEvent &evt)
{
}

void FileExplorerPanel::onLeaveWindow(wxMouseEvent &evt)
{
}

void FileExplorerPanel::onCharHook(wxKeyEvent &evt)
{
}









