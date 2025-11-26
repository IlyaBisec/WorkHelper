#ifndef _FILE_EXPLORER_PANEL_H_
#define _FILE_EXPLORER_PANEL_H_
#endif // !_FILE_EXPLORER_PANEL_H_
#pragma once

// FileExplorerPanel.H - header that contains GUI object as a custom list of files
// 04/11/2025
// ilya_bisec (c) 2025-2026

#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/dnd.h>
#include <vector>
#include <set>

// File exlporer panel class
class FileExplorerPanel;

// Drag and drop handler, transfers info about
// dropped files to the list area
class FileDropTarget : public wxFileDropTarget
{
public:
	FileDropTarget(FileExplorerPanel *owner) : m_owner(owner){}
	
	// File drop event area
	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filanames) override;
	

private:
	FileExplorerPanel *m_owner;
};

// File explorer panel class
// Custom file exolorer contains:
//	- File icon
//  - File name
//  - File meta info
//  - Ctrl+Copy/Paste files(from win explorer) to list
//  - Delete selection files in list
class FileExplorerPanel : public wxScrolledWindow
{
public:
	explicit FileExplorerPanel(wxWindow *parent, int rowHeight = 40);
	~FileExplorerPanel() override = default;
	
	// Add file in list
	void addFile(const wxString &path);
	// Clear list
	void clear();
	// Ctrl + Copy files from win explorer
	void copySelectionToClipboard();
	// Ctrl + Paste files to custom list
	void pasteFromClipboard();
	// Delete selection
	void deleteSelection();


private:
	struct Item 
	{
		wxString fullPath;
		wxString name;
		wxBitmap iconBmp; // Sized Bitmap
	};

	std::vector<Item> m_items;
	std::set<int> m_selected; // Multiple index selection

	int m_rowHeight;
	int m_hoverIndex; // - 1 if no hover
	int m_anchorIndex; // For Shift selection

	// Events
	// Draw all rows
	void onPaint(wxPaintEvent &evt); 
	// Track hover and show tooltip
	void onMouseMove(wxMouseEvent &evt);
	// Left click - multi-select processing (Ctrl/Shift)
	void onLeftDown(wxMouseEvent &evt);
	void onLeaveWindow(wxMouseEvent &evt);
	// Keycodes Delete, Ctrl+C/V
	void onCharHook(wxKeyEvent &evt); 

	// Transform the event coordinates into a "non-scrolled" position and find the element index
	int hitTestItem(const wxPoint &position) const;
	// Get and scale icon
	wxBitmap loadAndScaleIcon(const wxString &path, int size);
	// Delete indices in descending order
	void deleteIndices(const std::vector<int> &indicesDesc);

	// Clipboard utils
	void setFilesToClipboard(const wxArrayString &files);
	wxArrayString getFilesFromClipboard();

	// Draw 1 element
	void drawItem(wxDC &dc, int index, const wxRect &rect);

	wxDECLARE_EVENT_TABLE();
};
