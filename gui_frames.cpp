#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "steam.h"
#include "gui_frames.h"

IMPLEMENT_APP(App)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(BUTTON_Create, MainFrame::OnCreateItem)
    EVT_BUTTON(BUTTON_Update, MainFrame::OnUpdateItem)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
END_EVENT_TABLE()

bool App::OnInit()
{
    SteamAPI_Init();
    if (!SteamUGC()) {
        // Ensure that Steam loaded
        wxMessageBox(_T("Steam failed to load. Please ensure Steam is running and steam_appid.txt exists in the same folder the application exists"), _T("Error!"), wxICON_ERROR);
        return false;
    }

    MainFrame *main_frame = new MainFrame(GetAppIdFromFile());
    main_frame->Show(TRUE);
    SetTopWindow(main_frame);

    return true;
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    SteamAPI_Shutdown();
    Close(true);
}

void MainFrame::OnCreateItem(wxCommandEvent& event)
{
    wxMessageBox(wxT("Created item"), wxT("Title"), wxICON_INFORMATION);
}

void MainFrame::OnUpdateItem(wxCommandEvent& event)
{
    wxMessageBox(wxT("Updated item"), wxT("Title"), wxICON_INFORMATION);
}

MainFrame::MainFrame(AppId_t app) : wxFrame(NULL, wxID_ANY, "Steam Workshop Uploader"), m_uploader(app)
{
    m_create = new wxButton(this, BUTTON_Create, "Create a new mod");
    m_update = new wxButton(this, BUTTON_Update, "Update a pre-existing mod");

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_create);
    sizer->Add(m_update);
    SetSizer(sizer);
}
