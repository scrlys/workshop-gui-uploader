#pragma once
#include <wx/wxprec.h>

#include "steam.h"

wxDECLARE_EVENT(wxEVT_THREAD_COMPLETE, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_THREAD_UPDATE, wxThreadEvent);

class SplashFrame : public wxFrame
{
public:
	CreateWorkshop *m_workshop;

	wxButton *m_create;
	wxButton *m_update;

	void OnCreateItem(wxCommandEvent& event);
	void OnUpdateItem(wxCommandEvent& event);

	bool Step();

	SplashFrame();

	wxDECLARE_EVENT_TABLE();
};