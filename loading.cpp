#include "loading.h"

#include <functional>
#include <sstream>

#include <wx/wxprec.h>
#include <steam_api.h>

LoadingDialog::LoadingDialog(wxWindow * parent, std::function<bool()> _func, std::function<uint64()> _current_value, std::function<uint64()> _max_value, const wxPoint & position, const wxSize & size, long style)
	: wxDialog(parent, wxID_ANY, wxString("Loading"), position, size, style), not_static(new wxStaticText(this, wxID_ANY, wxString("Loading..."))),
	func(_func), current_value(_current_value), max_value(_max_value)
{
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	progress_bar = new wxGauge(this, wxID_ANY, max_value());

	sizer->AddStretchSpacer(4);
	sizer->Add(not_static, 0, wxALIGN_CENTER_HORIZONTAL);
	sizer->AddStretchSpacer(1);
	sizer->Add(progress_bar, 0, wxALIGN_CENTER_HORIZONTAL);
	sizer->AddStretchSpacer(4);

	SetSizer(sizer);

	//start();
}

void LoadingDialog::start()
{
	while (func()) {
		std::ostringstream string_stream;
		string_stream << "Loading...\n" << current_value() << "/" << max_value();
		not_static->SetLabel(wxString(string_stream.str()));
		progress_bar->SetValue(current_value());
		progress_bar->SetRange(max_value());
	}

	Close();
}