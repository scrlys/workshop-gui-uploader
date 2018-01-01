#pragma once

#include <map>
#include <wx/wxprec.h>

#include "steam.h"


class UpdateDialog : public wxDialog
{
public:
	UpdateWorkshop *m_uploader;
	bool m_initial;

	wxTextCtrl *m_name;
	wxTextCtrl *m_description;
	wxTextCtrl *m_preview_path;
	wxButton *m_preview_select;
	wxChoice *m_language;
	wxTextCtrl *m_path_path;
	wxButton *m_path_select;
	wxButton *m_finish;
	wxTextCtrl *m_changelog;
	wxTextCtrl *m_id;
	wxButton *m_copy_id;
	wxChoice *m_visibility;

	std::map<std::string, std::string> languages;

	void OnBrowsePreview(wxCommandEvent& event);
	void OnBrowsePath(wxCommandEvent& event);
	void OnFinish(wxCommandEvent& event);
	void OnCopy(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();

public:
	UpdateDialog(wxWindow * parent, bool initial, PublishedFileId_t _id = 0, const wxPoint & position = wxDefaultPosition, const wxSize & size = wxSize(400, 500), long style = wxDEFAULT_DIALOG_STYLE);
};
