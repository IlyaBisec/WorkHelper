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

// File exlporer panel class
class FileExplorerPanel;

// Drag and drop handler, transfers info about
// dropped files to the list area
class FileDropTarget : public wxFileDropTarget
{
public:
	FileDropTarget(FileExplorerPanel *owner) : m_owner(owner){}
	
	// Cursor with files enters area
	wxDragResult OnEnter(wxCoord x, wxCoord y, wxDragResult result) override;
	// Cursor with files leave area
	void OnLeave() override;
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
class FileExplorerPanel : public wxDataViewListCtrl
{
public:
	FileExplorerPanel(wxWindow *parent);
	
	// Add file in list
	void addFile(const wxString &path);
	// Highlights panel when hovering over file
	void highLight(bool enable); 
	// Get full file path
	wxString getFilePath(unsigned row) const;

private:
	// Get system icon of file by extension
	wxIcon getFileIcon(const wxString &path);
	// Display file metadata
	void onMouseMove(wxMouseEvent &evt);

	// Storing full file paths
	std::vector<wxString> m_files;
};
