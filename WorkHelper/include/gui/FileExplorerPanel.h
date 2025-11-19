#ifndef _FILE_EXPLORER_PANEL_H_
#define _FILE_EXPLORER_PANEL_H_
#endif // !_FILE_EXPLORER_PANEL_H_
#pragma once

// FileExplorerPanel.H - hedder for cpp, for main call func for init
// 04/11/2025
// ilya_bisec (c) 2025-2026

#include <wx/wx.h>
#
#include <wx/dnd.h>

// File exlporer panel class
class FileExplorerPanel;

// Drag and drop handler, передаёт информацию
// o сброшенных файлах в FileExplorerPanel
class FileDropTarget : public wxFileDropTarget
{
public:
	explicit FileDropTarget(FileExplorerPanel *owner);
	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filanames) override;
	wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult result) override;

private:
	FileExplorerPanel *m_owner;
};

// File explorer panel class
// Панель отображение файлов:
//	- Иконка файла
//  - Имя выбранного файла
//  - Кнопку добавить файл
//  - Всплывающую подсказку с метаинф
// 
// Поддерживает:
//	- Перетаскивание файла drag and drop
//  - Вставка из буфера обмена(Ctrl+V)
//  - Выбор через диалог
//

class FileExplorerPanel : public wxPanel
{
public:
	explicit FileExplorerPanel(wxWindow *parent);

	// Устанавливает путь к файлу и обновляет отображение
	void setFile(const wxString &path);
	// Подсвечивает панель при наведении файла
	void highLight(bool enable); 

private:
	wxStaticBitmap	*m_icon;
	wxStaticText	*m_filename;
	wxButton		*m_button;
	wxString		m_path; // Full file path

	bool m_highlighted = false;

	// Открывает диалог выбора файла (wxFileDialog)
	void onAddFile(wxCommandEvent &event);
	// Ctrl+V
	void onPaste(wxKeyEvent &event);
	// Обновляет отображение панели (иконка, имя, tooltip)
	void updateDisplay();

	wxString getFileToolTip(const wxString &path);

	friend class FileDropTarget;
};
