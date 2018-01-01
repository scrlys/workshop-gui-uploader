#pragma once

#include <functional>

#include <wx/wxprec.h>

#include <steam_api.h>

class LoadingDialog : public wxDialog
{
public:
	wxStaticText *not_static;
	std::function<bool()> func;
	std::function<uint64()> current_value;
	std::function<uint64()> max_value;

	LoadingDialog(wxWindow * parent, std::function<bool()> _func, std::function<uint64()> _current_value, std::function<uint64()> _max_value, const wxPoint & position = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

	void start();

	wxGauge *progress_bar;

private:
	//DECLARE_EVENT_TABLE();
};