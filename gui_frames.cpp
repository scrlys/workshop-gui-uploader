#include "gui_frames.h"

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>

#include <wx/wxprec.h>
#include <wx/event.h>
#include <wx/msgdlg.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "steam.h"

IMPLEMENT_APP(App)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(BUTTON_Create, MainFrame::OnCreateItem)
    EVT_BUTTON(BUTTON_Update, MainFrame::OnUpdateItem)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(UpdateFrame, wxFrame)
    EVT_BUTTON(BUTTON_preview, UpdateFrame::OnBrowsePreview)
    EVT_BUTTON(BUTTON_path, UpdateFrame::OnBrowsePath)
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

int App::OnExit()
{
    SteamAPI_Shutdown();
    return 0;
}

void MainFrame::OnCreateItem(wxCommandEvent& event)
{
    m_workshop->CreateItem();
    while (!m_workshop->IsFinished()) {
        SteamAPI_RunCallbacks();
    }

    switch (m_workshop->GetResult()) {
    case Success: {
        std::stringstream message;

        message << "Your item ID is " << m_workshop->GetFileID() << ", please write the number down.";
        wxMessageBox(message.str(), "Success!", wxOK | wxICON_INFORMATION);
        break;
    }
    case ELegal: {
        wxMessageDialog *message = new wxMessageDialog(this, "You need to agree to the Steam Workshop legal agreement to continue. Would you like to open the browser to the legal agreement?", "Error", wxYES_NO | wxCANCEL | wxYES_DEFAULT | wxICON_ERROR);
        if (message->ShowModal() == wxID_YES) {
            wxLaunchDefaultBrowser("http://steamcommunity.com/sharedfiles/workshoplegalagreement");
        }

        break;
    }
    case EPermissions: {
        wxMessageBox("You do not have permission to upload the mod.", "Error", wxICON_ERROR);
        break;
    }
    case ETimeout: {
        wxMessageBox("A timeout occurred. Check your internet and Steam's status and try again in a few minutes.", "Error", wxICON_ERROR);
        break;
    }
    case ELoggedOut: {
        wxMessageBox("Please log on to Steam.", "Error", wxICON_ERROR);
        break;
    }
    case EGeneral: {
        wxMessageBox("A generic error has occured.", "Error", wxICON_ERROR);
        break;
    }
    }
}

void MainFrame::OnUpdateItem(wxCommandEvent& event)
{
    wxTextEntryDialog *dialog = new wxTextEntryDialog(this, "Please enter the item ID", "Item ID");
    dialog->SetTextValidator(wxFILTER_DIGITS);
    if (dialog->ShowModal() == wxID_OK) {
        std::stringstream string_to_publishedfileid;
        PublishedFileId_t fileid;

        string_to_publishedfileid << dialog->GetValue();
        string_to_publishedfileid >> fileid;

        UpdateFrame *update_frame = new UpdateFrame(fileid, GetAppIdFromFile(), false);
        update_frame->Show(true);
        this->Close();
    }
}

MainFrame::MainFrame(AppId_t app) : wxFrame(NULL, wxID_ANY, "Steam Workshop Uploader"), m_workshop(new CreateWorkshop(app))
{
    m_create = new wxButton(this, BUTTON_Create, "Create a new mod");
    m_update = new wxButton(this, BUTTON_Update, "Update a pre-existing mod");

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_create);
    sizer->Add(m_update);
    SetSizer(sizer);
}

void UpdateFrame::OnBrowsePreview(wxCommandEvent& event)
{
    wxFileDialog dialog(this, _T("Choose preview file"), wxEmptyString, wxEmptyString, "JPG image (*.jpg)|*.jpg|PNG image (*.png)|*.png|GIF image (*.gif)|*.gif");

    if (dialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    m_preview_path->SetValue(dialog.GetPath());
}

void UpdateFrame::OnBrowsePath(wxCommandEvent& path)
{
    wxDirDialog dialog(this, _T("Choose path"), wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (dialog.ShowModal() == wxID_CANCEL) {
        return;
    }

    m_path_path->SetValue(dialog.GetPath());
}

void UpdateFrame::OnFinish(wxCommandEvent& event)
{
    if (!m_uploader->SetTitle(std::string(m_name->GetValue().mb_str()))) {
        wxMessageBox("There was an error saving the title.", "Error", wxICON_ERROR);
    }
    if (!m_uploader->SetDescription(std::string(m_description->GetValue().mb_str()))) {
        wxMessageBox("There was an error saving the description.", "Error", wxICON_ERROR);
    }
    if (!m_uploader->SetPreviewImage(std::string(m_preview_path->GetValue().mb_str()))) {
        wxMessageBox("There was an error saving the preview.", "Error", wxICON_ERROR);
    }
    std::string language = languages[std::string(m_language->GetString(m_language->GetSelection()))];
    std::cout << language << "\n";
    if (!m_uploader->SetLanguage(language)) {
        wxMessageBox("There was an error saving the description.", "Error", wxICON_ERROR);
    }
    if (m_initial) {
        m_uploader->SetChangelog("Initial upload");
    } else {
        m_uploader->SetChangelog(std::string(wxGetTextFromUser("Please enter a changelog message", "Info")));
    }

    m_uploader->FinishUpdateItem();

    while (!m_uploader->IsFinished()) {
        SteamAPI_RunCallbacks();

        // Fuck multithreading
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    switch (m_uploader->GetResult()) {
    case Success: {
        wxMessageBox("The mod has been successfully updated!", "Success!", wxICON_INFORMATION);
        break;
    }
    case ELegal: {
        wxMessageBox("You haven't accepted the Steam Workshop legal agreement.", "Error", wxICON_ERROR);
        break;
    }
    case EPermissions: {
        wxMessageBox("You don't have permissions to update the mod.", "Error", wxICON_ERROR);
        break;
    }
    case ETimeout: {
        wxMessageBox("A timeout occurred. Check your internet and Steam's status and try again in a few minutes.", "Error", wxICON_ERROR);
        break;
    }
    case ELoggedOut: {
        wxMessageBox("Please log on to Steam.", "Error", wxICON_ERROR);
        break;
    }
    case EGeneral: {
        wxMessageBox("A general error has occurred.", "Error", wxICON_ERROR);
        break;
    }
    }
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

UpdateFrame::UpdateFrame(PublishedFileId_t fileid, AppId_t app, bool initial) : wxFrame(NULL, wxID_ANY, "Steam Workshop Uploader"), m_uploader(new UpdateWorkshop(fileid, app)), m_initial(initial)
{
    m_uploader->StartUpdateItem();

    std::vector<wxString> display_languages = get_keys(languages);

    m_name = new wxTextCtrl(this, TEXT_name);
    m_description = new wxTextCtrl(this, TEXT_description, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    m_preview_path = new wxTextCtrl(this, TEXT_preview);
    m_preview_select = new wxButton(this, BUTTON_preview, "Browse...");
    m_language = new wxChoice(this, COMBO_language, wxDefaultPosition, wxDefaultSize, display_languages.size(), &display_languages[0]);
    m_path_path = new wxTextCtrl(this, TEXT_path);
    m_path_select = new wxButton(this, BUTTON_path, "Browse...");
    m_finish = new wxButton(this, BUTTON_finish, "Finish");

    m_name->SetMaxLength(k_cchPublishedDocumentTitleMax);

    // TODO: Submit bug report
    //m_description->SetMaxLength(k_cchPublishedDocumentDescriptionMax);

    m_language->SetSelection(m_language->FindString("English"));

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

    vertical->Add(name);
    vertical->Add(description);
    vertical->Add(preview);
    vertical->Add(language);
    vertical->Add(path);
    vertical->Add(m_finish);

    SetSizer(vertical);
}
