#include "update.h"

#include <wx/wxprec.h>
#include <wx/clipbrd.h>

#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>

#include "shared.h"
#include "loading.h"


BEGIN_EVENT_TABLE(UpdateDialog, wxDialog)
	EVT_BUTTON(BUTTON_preview, UpdateDialog::OnBrowsePreview)
	EVT_BUTTON(BUTTON_path, UpdateDialog::OnBrowsePath)
	EVT_BUTTON(BUTTON_finish, UpdateDialog::OnFinish)
	EVT_BUTTON(BUTTON_copy_id, UpdateDialog::OnCopy)
END_EVENT_TABLE()

std::vector<wxString> get_keys(std::map<std::string, std::string> map)
{
	std::vector<wxString> result;

	std::map<std::string, std::string>::iterator iterator;
	for (iterator = map.begin(); iterator != map.end(); ++iterator) {
		result.push_back(wxString(iterator->first));
	}

	return result;
}

void UpdateDialog::OnBrowsePreview(wxCommandEvent & event)
{
	wxFileDialog dialog(this, "Choose preview file", wxEmptyString, wxEmptyString, "Supported images (JPEG, PNG, GIF,)|*.jpg;*.png;*.gif");

	if (dialog.ShowModal() == wxID_CANCEL) {
		return;
	}

	m_preview_path->SetValue(dialog.GetPath());
}

void UpdateDialog::OnBrowsePath(wxCommandEvent & event)
{
	wxDirDialog dialog(this, "Choose path", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dialog.ShowModal() == wxID_CANCEL) {
		return;
	}

	m_path_path->SetValue(dialog.GetPath());
}

void UpdateDialog::OnFinish(wxCommandEvent & event)
{
	// PublishedFileId_t is typedef'd to uint64
	m_uploader = new UpdateWorkshop(std::stoull(m_id->GetValue().ToStdString()), APP_ID);
	m_uploader->StartUpdateItem();

	std::string name = m_name->GetValue();
	if (name == "" && m_initial) {
		wxMessageBox("Please put a title", "Error", wxICON_ERROR);
		return;
	}
	else if (name != "") {
		if (!m_uploader->SetTitle(name)) {
			wxMessageBox("There was an error saving the title.", "Error", wxICON_ERROR);
			return;
		}
	}

	std::string description = m_description->GetValue();
	if (description == "" && m_initial) {
		wxMessageBox("Please put a description", "Error", wxICON_ERROR);
		return;
	}
	else if (description != "") {
		if (!m_uploader->SetDescription(description)) {
			wxMessageBox("There was an error saving the description.", "Error", wxICON_ERROR);
			return;
		}
	}

	std::string preview = m_preview_path->GetValue();
	if (preview == "" && m_initial) {
		wxMessageBox("Please choose a preview image", "Error", wxICON_ERROR);
		return;
	}
	else if (preview != "") {
		if (!m_uploader->SetPreviewImage(preview)) {
			wxMessageBox("There was an error saving the preview.", "Error", wxICON_ERROR);
			return;
		}
	}

	std::string language = languages[std::string(m_language->GetString(m_language->GetSelection()))];
	if (language != "---" && !m_uploader->SetLanguage(language)) {
		wxMessageBox("There was an error saving the description.", "Error", wxICON_ERROR);
		return;
	}

	std::string visibilityStr = m_visibility->GetString(m_visibility->GetSelection());
	if (visibilityStr != "---") {
		ERemoteStoragePublishedFileVisibility visibility;

		if (visibilityStr == "Public") {
			visibility = k_ERemoteStoragePublishedFileVisibilityPublic;
		}
		else if (visibilityStr == "Friends Only") {
			visibility = k_ERemoteStoragePublishedFileVisibilityFriendsOnly;
		}
		else if (visibilityStr == "Private") {
			visibility = k_ERemoteStoragePublishedFileVisibilityPrivate;
		}

		if (!m_uploader->SetVisibility(visibility)) {
			wxMessageBox("There was an error saving the visibility.", "Error", wxICON_ERROR);
			return;
		}
	}

	std::string changelog = m_changelog->GetValue();
	if (changelog == "") {
		wxMessageBox("Please put a changelog message", "Error", wxICON_ERROR);
		return;
	}
	m_uploader->SetChangelog(changelog);

	std::string path = m_path_path->GetValue();
	if (path == "" && m_initial) {
		wxMessageBox("Please select the folder where your mod is located", "Error", wxICON_ERROR);
		return;
	} else if (path != "") {
		if (!m_uploader->SetItemContent(path)) {
			wxMessageBox("There was an error setting the path.", "Error", wxICON_ERROR);
			return;
		}
	}

	// Every error should've returned, so we're free to upload
	m_uploader->FinishUpdateItem();
	m_uploader->UpdateStats();
	
	LoadingDialog *dialog = new LoadingDialog(this, [=]() {
		SteamAPI_RunCallbacks();
		m_uploader->UpdateStats();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return !m_uploader->IsFinished();
	}, [=]() {
		return m_uploader->GetUploadedBytes();
	}, [=]() {
		return m_uploader->GetTotalBytes();
	});

	dialog->Show();
	dialog->start();

	// Now get the results
	switch (m_uploader->GetResult()) {
	case k_EResultOK:
		if (m_initial) {
			wxMessageBox("The mod has been successfully uploaded!", "Success!", wxICON_INFORMATION);
		} else {
			wxMessageBox("The mod has been successfully updated!", "Success!", wxICON_INFORMATION);
		}

		Close();
		return;
	case (EResult) 0:
		wxMessageBox("You haven't accepted the Steam Workshop legal agreement.", "Error", wxICON_ERROR);
		break;
	case k_EResultAccessDenied:
		wxMessageBox("You don't have permissions to update the mod.", "Error", wxICON_ERROR);
		break;
	case k_EResultFileNotFound:
		if (m_initial) {
			wxMessageBox("One or more files were not found. Verify that they exist", "Error", wxICON_ERROR);
		} else {
			wxMessageBox("One or more files were not found, or the specified ID was not found. Verify that both exist", "Error", wxICON_ERROR);
		}
		break;
	case k_EResultLimitExceeded:
		wxMessageBox("Either the preview image is too large (must be less than 1 megabyte) or you do not have enough storage in Steam Cloud to upload this mod", "Error", wxICON_ERROR);
		break;
	case k_EResultFail:
		wxMessageBox("A general error has occurred", "Error", wxICON_ERROR);
		break;
	default:
		std::ostringstream stream;
		stream << "A general error has occurred. Error code " << m_uploader->GetResult();
		wxMessageBox(stream.str(), "Error", wxICON_ERROR);
		break;
	}
}

void UpdateDialog::OnCopy(wxCommandEvent & event)
{
	if (wxTheClipboard->Open()) {
		wxTheClipboard->SetData(new wxTextDataObject(m_id->GetValue()));
		wxTheClipboard->Close();
	} else {
		wxMessageBox("Unable to copy to the clipboard", "Warning", wxICON_WARNING);
	}
}

UpdateDialog::UpdateDialog(wxWindow * parent, bool initial, PublishedFileId_t _id, const wxPoint & position, const wxSize & size, long style)
	: wxDialog(parent, wxID_ANY, wxString("Update Mod"), position, size, style), m_initial(initial)
{
	// Set up the languages
	
	languages = std::map<std::string, std::string>();
	if (!initial) {
		languages["---"] = "---";
	}
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
	

	wxPanel *panel = new wxPanel(this);
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *idSizer = new wxBoxSizer(wxHORIZONTAL);
	m_id = new wxTextCtrl(panel, TEXT_id);
	idSizer->Add(new wxStaticText(panel, wxID_ANY, "ID:"), 0, wxRIGHT, 8);
	idSizer->Add(m_id, 1);
	if (initial) {
		m_copy_id = new wxButton(panel, BUTTON_copy_id, "Copy");
		m_id->Disable();
		m_id->SetValue(std::to_string(_id));
		idSizer->Add(m_copy_id, 0);
	}

	vbox->Add(idSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
	
	vbox->AddSpacer(5);

	wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
	nameSizer->Add(new wxStaticText(panel, wxID_ANY, "Name:"), 0, wxRIGHT, 8);
	m_name = new wxTextCtrl(panel, TEXT_name);
	nameSizer->Add(m_name, 1);
	vbox->Add(nameSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	vbox->AddSpacer(5);

	wxBoxSizer *descSizer = new wxBoxSizer(wxHORIZONTAL);
	descSizer->Add(new wxStaticText(panel, wxID_ANY, "Description:"), 0);
	vbox->Add(descSizer, 0, wxLEFT, 10);

	vbox->AddSpacer(5);

	wxBoxSizer *descTextSizer = new wxBoxSizer(wxHORIZONTAL);
	m_description = new wxTextCtrl(panel, TEXT_description, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	descTextSizer->Add(m_description, 1, wxEXPAND);

	vbox->Add(descTextSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

	vbox->AddSpacer(5);

	wxBoxSizer *languageSizer = new wxBoxSizer(wxHORIZONTAL);
	languageSizer->Add(new wxStaticText(panel, wxID_ANY, "Language:"), 0, wxRIGHT, 8);
	m_language = new wxChoice(panel, COMBO_language, wxDefaultPosition, wxDefaultSize, get_keys(languages).size(), &get_keys(languages)[0]);
	if (initial) {
		m_language->SetSelection(7); // English
	} else {
		m_language->SetSelection(0);
	}
	languageSizer->Add(m_language, 1);

	vbox->Add(languageSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 10);

	vbox->AddSpacer(5);

	wxBoxSizer *visibilitySizer = new wxBoxSizer(wxHORIZONTAL);
	visibilitySizer->Add(new wxStaticText(panel, wxID_ANY, "Visibility:"), 0, wxRIGHT, 8);
	if (initial) {
		m_visibility = new wxChoice(panel, COMBO_visibility, wxDefaultPosition, wxDefaultSize, 3, new const wxString[3]{ wxT("Public"), wxT("Friends Only"), wxT("Private") });
	} else {
		m_visibility = new wxChoice(panel, COMBO_visibility, wxDefaultPosition, wxDefaultSize, 4, new const wxString[4]{ wxT("---"), wxT("Public"), wxT("Friends Only"), wxT("Private") });
	}
	m_visibility->SetSelection(0); // Public or ---
	visibilitySizer->Add(m_visibility, 1);

	vbox->Add(visibilitySizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 10);

	vbox->AddSpacer(5);

	
	wxBoxSizer *previewSizer = new wxBoxSizer(wxHORIZONTAL);
	previewSizer->Add(new wxStaticText(panel, wxID_ANY, "Preview Image:"), 0, wxRIGHT, 8);
	m_preview_path = new wxTextCtrl(panel, TEXT_preview);
	m_preview_select = new wxButton(panel, BUTTON_preview, "Browse...");
	previewSizer->Add(m_preview_path, 1);
	previewSizer->Add(m_preview_select, 0);
	
	vbox->Add(previewSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	vbox->AddSpacer(5);
	

	wxBoxSizer *pathSizer = new wxBoxSizer(wxHORIZONTAL);
	pathSizer->Add(new wxStaticText(panel, wxID_ANY, "Path:"), 0, wxRIGHT, 8);
	m_path_path = new wxTextCtrl(panel, TEXT_path);
	m_path_select = new wxButton(panel, BUTTON_path, "Browse...");
	pathSizer->Add(m_path_path, 1);
	pathSizer->Add(m_path_select, 0);

	vbox->Add(pathSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	vbox->AddSpacer(5);

	wxBoxSizer *changelogSizer = new wxBoxSizer(wxHORIZONTAL);
	changelogSizer->Add(new wxStaticText(panel, wxID_ANY, "Changelog:"), 0, wxRIGHT, 8);

	vbox->Add(changelogSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	vbox->AddSpacer(5);

	wxBoxSizer *changelogTextSizer = new wxBoxSizer(wxHORIZONTAL);
	m_changelog = new wxTextCtrl(panel, TEXT_changelog, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	changelogTextSizer->Add(m_changelog, 1, wxEXPAND);

	vbox->Add(changelogTextSizer, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

	vbox->AddSpacer(5);
	
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	m_finish = new wxButton(panel, BUTTON_finish, "Ok");
	buttonSizer->Add(m_finish, 0, wxLEFT | wxBOTTOM, 5);
	vbox->Add(buttonSizer, 0, wxALIGN_RIGHT | wxRIGHT, 10);

	panel->SetSizer(vbox);
}
