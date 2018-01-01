#include "splash.h"

#include <chrono>
#include <thread>

#include <wx/wxprec.h>
#include <wx/event.h>
#include <wx/msgdlg.h>

#include "shared.h"
#include "loading.h"
#include "update.h"

BEGIN_EVENT_TABLE(SplashFrame, wxFrame)
	EVT_BUTTON(BUTTON_Create, SplashFrame::OnCreateItem)
	EVT_BUTTON(BUTTON_Update, SplashFrame::OnUpdateItem)
END_EVENT_TABLE()

SplashFrame::SplashFrame() : wxFrame(NULL, wxID_ANY, "Steam Workshop Uploader"), m_workshop(new CreateWorkshop(APP_ID))
{
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL,	_("panel"));

	panel->SetSizer(buttonSizer);

	m_create = new wxButton(panel, BUTTON_Create, "Create a new mod");
	m_update = new wxButton(panel, BUTTON_Update, "Update a pre-existing mod");

	buttonSizer->AddStretchSpacer(4);
	//TODO: Maybe an image?
	buttonSizer->Add(new wxStaticText(panel, wxID_ANY, wxString(APP_NAME)), 0, wxALIGN_CENTER_HORIZONTAL);
	buttonSizer->AddStretchSpacer(1);
	buttonSizer->Add(m_create, 0, wxALIGN_CENTER_HORIZONTAL);
	buttonSizer->AddStretchSpacer(1);
	buttonSizer->Add(m_update, 0, wxALIGN_CENTER_HORIZONTAL);
	buttonSizer->AddStretchSpacer(4);
}

int progress;

void SplashFrame::OnCreateItem(wxCommandEvent& event)
{
	m_workshop->CreateItem();

	LoadingDialog *dialog = new LoadingDialog(this, [=]() {
		SteamAPI_RunCallbacks();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return !m_workshop->IsFinished();
	}, [=]() {
		return m_workshop->IsFinished() ? 1 : 0;
	}, []() {
		return 1;
	});

	dialog->Show();

	// For some reason, there's a pause before the progress bar actually displays; however, it's beneficial to us.
	dialog->start();
	
	switch (m_workshop->GetResult()) {
	case Success: {
		
		break;
	}
	case ELegal: {
		wxMessageDialog *message = new wxMessageDialog(this, "You need to agree to the Steam Workshop legal agreement to continue. Would you like to open the browser to the legal agreement?", "Error", wxYES_NO | wxCANCEL | wxYES_DEFAULT | wxICON_ERROR);
		if (message->ShowModal() == wxID_YES) {
			wxLaunchDefaultBrowser("http://steamcommunity.com/sharedfiles/workshoplegalagreement");
		}

		std::cout << "Legal\n";
		break;
	}
	case EPermissions: {
		wxMessageBox("You do not have permission to upload the mod.", "Error", wxICON_ERROR);
		std::cout << "Permissions\n";
		break;
	}
	case ETimeout: {
		wxMessageBox("A timeout occurred. Check your internet and Steam's status and try again in a few minutes.", "Error", wxICON_ERROR);
		std::cout << "Timeout\n";
		break;
	}
	case ELoggedOut: {
		wxMessageBox("Please log on to Steam.", "Error", wxICON_ERROR);
		std::cout << "Logged Out\n";
		break;
	}
	case EGeneral: {
		wxMessageBox("A generic error has occured.", "Error", wxICON_ERROR);
		std::cout << "Generic\n";
		break;
	}
	}

	UpdateDialog *update = new UpdateDialog(this, true, m_workshop->GetFileID());
	update->ShowModal();
}

void SplashFrame::OnUpdateItem(wxCommandEvent& event)
{
	UpdateDialog *dialog = new UpdateDialog(this, false);
	dialog->ShowModal();
}

bool SplashFrame::Step()
{
	SteamAPI_RunCallbacks();
	Sleep(100);
	return m_workshop->IsFinished();
}