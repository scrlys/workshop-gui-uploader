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
END_EVENT_TABLE()

bool App::OnInit()
{
    MainFrame *main_frame = new MainFrame(GetAppIdFromFile());
    main_frame->Show(TRUE);
    SetTopWindow(main_frame);

    return true;
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    SteamAPI_Shutdown();
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
    m_create = new wxButton(this, BUTTON_Create, "Create a new mod", wxPoint(0, 50));
    m_update = new wxButton(this, BUTTON_Update, "Update a pre-existing mod");
}
