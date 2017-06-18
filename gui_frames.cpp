#include "gui_frames.h"

#include <map>
#include <vector>

#include <wx/wxprec.h>
#include <wx/filepicker.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "steam.h"

IMPLEMENT_APP(App)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(BUTTON_Create, MainFrame::OnCreateItem)
    EVT_BUTTON(BUTTON_Update, MainFrame::OnUpdateItem)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(UpdateFrame, wxFrame)
    EVT_TEXT(TEXT_name, UpdateFrame::OnUpdateName)
    EVT_TEXT(TEXT_description, UpdateFrame::OnUpdateDescription)
    EVT_TEXT(TEXT_preview, UpdateFrame::OnUpdatePreviewImage)
    EVT_BUTTON(BUTTON_preview, UpdateFrame::OnBrowsePreview)
    EVT_COMBOBOX(COMBO_language, UpdateFrame::OnUpdateLanguage)
    EVT_COMBOBOX(COMBO_visibility, UpdateFrame::OnUpdateVisibility)
    EVT_TEXT(TEXT_path, UpdateFrame::OnUpdatePath)
    EVT_BUTTON(BUTTON_path, UpdateFrame::OnBrowsePreview)
    EVT_BUTTON(BUTTON_finish, UpdateFrame::OnFinish)
END_EVENT_TABLE()

std::map<std::string, std::string> languages;

bool App::OnInit()
{
    SteamAPI_Init();
    if (!SteamUGC()) {
        // Ensure that Steam loaded
        wxMessageBox(_T("Steam failed to load. Please ensure Steam is running and steam_appid.txt exists in the same folder the application exists"), _T("Error!"), wxICON_ERROR);
        return false;
    }

    languages = std::map<std::string, std::string>();
    languages["English"] = "english";
    languages["Arabic"] = "arabic";
    languages["Bulgarian"] = "bulgarian";
    languages["Chinese (Simplified)"] = "schinese";
    languages["Chinese (Traditional)"] = "tchinese";
    languages["Czech"] = "czech";
    languages["Danish"] = "danish";
    languages["Dutch"] = "dutch";
    languages["Finnish"] = "finnish";
    languages["French"] = "french";
    languages["German"] = "german";
    languages["Greek"] = "greek";
    languages["Hungarian"] = "hungarian";
    languages["Italian"] = "italian";
    languages["Japanese"] = "japanese";
    languages["Korean"] = "koreana";
    languages["Norwegian"] = "norwegian";
    languages["Polish"] = "polish";
    languages["Portuguese"] = "portuguese";
    languages["Portuguese-Brazil"] = "brazilian";
    languages["Romanian"] = "romanian";
    languages["Russian"] = "russian";
    languages["Spanish"] = "spanish";
    languages["Swedish"] = "swedish";
    languages["Thai"] = "thai";
    languages["Turkish"] = "turkish";
    languages["Ukrainian"] = "ukrainian";

    MainFrame *main_frame = new MainFrame(GetAppIdFromFile());
    main_frame->Show(true);
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
    //wxMessageBox(wxT("Updated item"), wxT("Title"), wxICON_INFORMATION);
    UpdateFrame *update_frame = new UpdateFrame();
    update_frame->Show(true);
    //SetTopWindow(update_frame);
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

void UpdateFrame::OnUpdateName(wxCommandEvent& event)
{
    std::cerr << "name\n";
}

void UpdateFrame::OnUpdateDescription(wxCommandEvent& event)
{
    std::cerr << "desc\n";
}

void UpdateFrame::OnUpdatePreviewImage(wxCommandEvent& event)
{
    std::cerr << "prevew\n";
}

void UpdateFrame::OnUpdateLanguage(wxCommandEvent& event)
{
    std::cerr << "language\n";
}

void UpdateFrame::OnUpdateVisibility(wxCommandEvent& event)
{
    std::cerr << "visibility\n";
}

void UpdateFrame::OnUpdatePath(wxCommandEvent& event)
{
    std::cerr << "update\n";
}

void UpdateFrame::OnBrowsePreview(wxCommandEvent& event)
{
    std::cerr << "browse preview\n";
}

void UpdateFrame::OnBrowsePath(wxCommandEvent& path)
{
    std::cerr << "browse path\n";
}

void UpdateFrame::OnFinish(wxCommandEvent& event)
{
    std::cerr << "finish\n";
}

std::vector<wxString> get_keys(std::map<std::string, std::string> map)
{
    std::vector<wxString> result;

    std::map<std::string, std::string>::iterator iterator;
    for (iterator = languages.begin(); iterator != languages.end(); ++iterator) {
        result.push_back(wxString(iterator->first));
    }

    return result;
}

UpdateFrame::UpdateFrame() : wxFrame(NULL, wxID_ANY, "Steam Workshop Uploader")
{
    m_name = new wxTextCtrl(this, TEXT_name);
    m_description = new wxTextCtrl(this, TEXT_description, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    m_preview_path = new wxTextCtrl(this, TEXT_preview);
    m_preview_select = new wxButton(this, BUTTON_preview, "Browse...");
    m_language = new wxComboBox(this, COMBO_language, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, &get_keys(languages)[0], wxCB_READONLY);
    m_visibility = new wxComboBox(this, COMBO_visibility, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    m_path_path = new wxTextCtrl(this, TEXT_path);
    m_path_select = new wxButton(this, BUTTON_path, "Browse...");
    m_finish = new wxButton(this, BUTTON_finish, "Finish");

    wxBoxSizer *vertical = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *name = new wxBoxSizer(wxHORIZONTAL);
    name->Add(new wxStaticText(this, wxID_ANY, _T("Name:")));
    name->Add(m_name);

    wxBoxSizer *description = new wxBoxSizer(wxHORIZONTAL);
    description->Add(new wxStaticText(this, wxID_ANY, _T("Description:")));
    description->Add(m_description);

    wxBoxSizer *preview_file = new wxBoxSizer(wxHORIZONTAL);
    preview_file->Add(m_preview_path);
    preview_file->Add(m_preview_select);

    wxBoxSizer *preview = new wxBoxSizer(wxHORIZONTAL);
    preview->Add(new wxStaticText(this, wxID_ANY, _T("Preview image:")));
    preview->Add(preview_file);

    wxBoxSizer *language = new wxBoxSizer(wxHORIZONTAL);
    language->Add(new wxStaticText(this, wxID_ANY, _T("Language:")));
    language->Add(m_language);

    wxBoxSizer *path_file = new wxBoxSizer(wxHORIZONTAL);
    path_file->Add(m_path_path);
    path_file->Add(m_path_select);

    wxBoxSizer *path = new wxBoxSizer(wxHORIZONTAL);
    path->Add(new wxStaticText(this, wxID_ANY, _T("Path:")));
    path->Add(path_file);

    wxBoxSizer *visibility = new wxBoxSizer(wxHORIZONTAL);
    visibility->Add(new wxStaticText(this, wxID_ANY, _T("Visibility:")));
    visibility->Add(m_visibility);

    vertical->Add(name);
    vertical->Add(description);
    vertical->Add(preview);
    vertical->Add(language);
    vertical->Add(visibility);
    vertical->Add(path);
    vertical->Add(m_finish);

    SetSizer(vertical);
}
