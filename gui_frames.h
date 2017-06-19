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
    TEXT_preview,
    BUTTON_preview,
    COMBO_language,
    TEXT_path,
    BUTTON_path,
    BUTTON_finish,
};

class App : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
};

class MainFrame : public wxFrame
{
public:
    WorkshopUploader* m_uploader;
    wxButton *m_create;
    wxButton *m_update;

    void OnCreateItem(wxCommandEvent& event);
    void OnUpdateItem(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

public:
    MainFrame(AppId_t app);
};

class UpdateFrame : public wxFrame
{
public:
    WorkshopUploader *m_uploader;
    bool m_initial;

    wxTextCtrl *m_name;
    wxTextCtrl *m_description;
    wxTextCtrl *m_preview_path;
    wxButton *m_preview_select;
    wxChoice *m_language;
    wxTextCtrl *m_path_path;
    wxButton *m_path_select;
    wxButton *m_finish;

    void OnBrowsePreview(wxCommandEvent& event);
    void OnBrowsePath(wxCommandEvent& event);

    void OnFinish(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

public:
    UpdateFrame(WorkshopUploader *uploader, bool initial);
};
