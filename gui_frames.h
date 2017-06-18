#pragma once

#include <wx/wxprec.h>
#include <wx/filepicker.h>
#include <map>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "steam.h"

enum
{
    BUTTON_Create = wxID_HIGHEST + 1,
    BUTTON_Update,
    TEXT_name,
    TEXT_description,
    FILE_preview,
    COMBO_language,
    COMBO_visibility,
    FILE_path,
    BUTTON_finish,
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
public:
//    WorkshopUploader m_uploader;

    wxTextCtrl *m_name;
    wxTextCtrl *m_description;
    wxTextCtrl *m_preview_path;
    wxButton *m_preview_select;
    wxComboBox *m_language;
    wxComboBox *m_visibility;
    wxTextCtrl *m_path_path;
    wxButton *m_path_select;
    wxButton *m_finish;

    void OnUpdateName(wxCommandEvent& event);
    void OnUpdateDescription(wxCommandEvent& event);
    void OnUpdatePreviewImage(wxCommandEvent& event);
    void OnUpdateLanguage(wxCommandEvent& event);
    void OnUpdateVisibility(wxCommandEvent& event);
    void OnUpdatePath(wxCommandEvent& event);

    void OnFinish(wxCommandEvent& event);

public:
    UpdateFrame();
};
