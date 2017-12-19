#include <iostream>
#include <fstream>

#include <steam_api.h>

#include "steam.h"

void UpdateWorkshop::submit_callback(SubmitItemUpdateResult_t *result, bool failure)
{
    std::cerr << "Submit callback called\n";

    if (failure) {
        m_result = EGeneral;
    } else if (result->m_bUserNeedsToAcceptWorkshopLegalAgreement) {
        m_result = ELegal;
    } else {
        switch (result->m_eResult) {
            case k_EResultInsufficientPrivilege:
                m_result = EPermissions;
                break;
            case k_EResultTimeout:
                m_result = ETimeout;
                break;
            case k_EResultNotLoggedOn:
                m_result = ELoggedOut;
                break;
            default:
                m_result = Success;
                break;
        }
    }

    m_finished = true;
}

UpdateWorkshop::UpdateWorkshop(PublishedFileId_t fileid, AppId_t game) : m_fileid(fileid), m_game(game)
{
}

void UpdateWorkshop::StartUpdateItem()
{
    std::cerr << "Starting update\n";
    m_handle = SteamUGC()->StartItemUpdate(m_game, m_fileid);
}

bool UpdateWorkshop::SetTitle(std::string title)
{
    return SteamUGC()->SetItemTitle(m_handle, title.c_str());
}

bool UpdateWorkshop::SetDescription(std::string description)
{
    return SteamUGC()->SetItemDescription(m_handle, description.c_str());
}

bool UpdateWorkshop::SetPreviewImage(std::string image)
{
    return SteamUGC()->SetItemPreview(m_handle, image.c_str());
}

bool UpdateWorkshop::SetLanguage(std::string language)
{
    return SteamUGC()->SetItemUpdateLanguage(m_handle, language.c_str());
}

bool UpdateWorkshop::SetVisibility(ERemoteStoragePublishedFileVisibility visibility)
{
    return SteamUGC()->SetItemVisibility(m_handle, visibility);
}

void UpdateWorkshop::SetChangelog(std::string changelog)
{
    m_changelog = changelog;
}

void UpdateWorkshop::FinishUpdateItem()
{
    std::cerr << "Finishing update\n";
    SteamAPICall_t call = SteamUGC()->SubmitItemUpdate(m_handle, m_changelog.c_str());
    m_update_result.Set(call, this, &UpdateWorkshop::submit_callback);
}

bool UpdateWorkshop::IsFinished()
{
    return m_finished;
}

Result UpdateWorkshop::GetResult()
{
    return m_result;
}

void CreateWorkshop::create_callback(CreateItemResult_t *result, bool failure)
{
    std::cerr << "Create callback called\n";

    if (failure) {
        m_result = EGeneral;
    } else if (result->m_bUserNeedsToAcceptWorkshopLegalAgreement) {
        m_result = ELegal;
    } else {
        switch (result->m_eResult) {
            case k_EResultInsufficientPrivilege:
                m_result = EPermissions;
                break;
            case k_EResultTimeout:
                m_result = ETimeout;
                break;
            case k_EResultNotLoggedOn:
                m_result = ELoggedOut;
                break;
            case k_EResultOK:
                m_result = Success;
                m_fileid = result->m_nPublishedFileId;
                break;
			default:
				std::cerr << "What?\n";
				m_result = EGeneral;
				break;
        }
    }

    m_finished = true;
}

CreateWorkshop::CreateWorkshop(AppId_t game) : m_game(game)
{
}

void CreateWorkshop::CreateItem()
{
    std::cerr << "Creating item\na";

    SteamAPICall_t result = SteamUGC()->CreateItem(m_game, k_EWorkshopFileTypeCommunity);
    m_create_result.Set(result, this, &CreateWorkshop::create_callback);
}

bool CreateWorkshop::IsFinished()
{
    return m_finished;
}

Result CreateWorkshop::GetResult()
{
    return m_result;
}

PublishedFileId_t CreateWorkshop::GetFileID()
{
    return m_fileid;
}

AppId_t GetAppIdFromFile()
{
    AppId_t appid;
    char* buffer = new char[33]; // Include 1 more character in case of newline
    std::ifstream file_appid("steam_appid.txt", std::ios::in);

    // Read whatever we can
    file_appid >> buffer;

    // Get the app id. Note that AppId_t is typedef'd to uint32
    sscanf(buffer, "%zd", &appid);
    return appid;
}
