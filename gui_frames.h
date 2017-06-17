#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "steam.h"

enum
{
    BUTTON_Create = wxID_HIGHEST + 1,
    BUTTON_Update,
};

class App : public wxApp
{
public:
    virtual bool OnInit();
};

class MainFrame : public wxFrame
{
public:
    WorkshopUploader m_uploader;
    wxButton *m_create;
    wxButton *m_update;

    void OnExit(wxCommandEvent& event);
    void OnCreateItem(wxCommandEvent& event);
    void OnUpdateItem(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

public:
    MainFrame(AppId_t app);
};

class UpdateFrame : public wxFrame
{
private:
    void OnUpdateName(wxCommandEvent& event);
    void OnUpdateDescription(wxCommandEvent& event);
    void OnUpdatePreviewImage(wxCommandEvent& event);
    void OnUpdateLanguage(wxCommandEvent& event);
    void OnUpdateVisibility(wxCommandEvent& event);

    void OnFinish(wxCommandEvent& event);

public:
    UpdateFrame();
};

//BEGIN_EVENT_TABLE(MainFrame, wxFrame)
//    EVT_BUTTON(BUTTON_Create, MainFrame::OnCreateItem)
//    EVT_BUTTON(BUTTON_Update, MainFrame::OnUpdateItem)
//END_EVENT_TABLE()
