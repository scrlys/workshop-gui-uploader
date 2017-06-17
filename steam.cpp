#include <iostream>
#include <fstream>

#include <steam_api.h>

#include "steam.h"

WorkshopUploader::WorkshopUploader(AppId_t game) : m_game(game), m_finished(false)
{
    // This will assume that Steam is init'd.
}

void WorkshopUploader::create_callback(CreateItemResult_t *result, bool failure)
{
    std::cerr << "Create callback called\n";

    if (failure) {
        m_result = EGeneral;
        m_finished = true;
        return;
    }

    if (result->m_bUserNeedsToAcceptWorkshopLegalAgreement) {
        m_result = ELegal;
        m_finished = true;
        return;
    }

    switch (result->m_eResult) {
        case k_EResultInsufficientPrivilege:
            m_result = EPermissions;
            m_finished = true;
            break;
        case k_EResultTimeout:
            m_result = ETimeout;
            m_finished = true;
            break;
        case k_EResultNotLoggedOn:
            m_result = ELoggedOut;
            m_finished = true;
            break;
        default:
            m_fileid = result->m_nPublishedFileId;
            StartUpdateItem();
            break;
    }
}

void WorkshopUploader::submit_callback(SubmitItemUpdateResult_t *result, bool failure)
{
    std::cerr << "Submit callback called\n";
    // Ensure that the program doesn't go any father if completed
    if (m_finished) {
        return;
    }

    if (failure) {
        m_result = EGeneral;
        m_finished = true;
        return;
    }

    if (result->m_bUserNeedsToAcceptWorkshopLegalAgreement) {
        m_result = ELegal;
        m_finished = true;
        return;
    }

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

    m_finished = true;
}

void WorkshopUploader::CreateItem()
{
    std::cerr << "Creating item\n";

    SteamAPICall_t result = SteamUGC()->CreateItem(m_game, k_EWorkshopFileTypeCommunity);
    m_create_result.Set(result, this, &WorkshopUploader::create_callback);
}

void WorkshopUploader::StartUpdateItem()
{
    std::cerr << "Starting update\n";
    m_handle = SteamUGC()->StartItemUpdate(m_game, m_fileid);
}

bool WorkshopUploader::SetTitle(std::string title)
{
    return SteamUGC()->SetItemTitle(m_handle, title.c_str());
}

bool WorkshopUploader::SetDescription(std::string description)
{
    return SteamUGC()->SetItemDescription(m_handle, description.c_str());
}

bool WorkshopUploader::SetPreviewImage(std::string image)
{
    return SteamUGC()->SetItemPreview(m_handle, image.c_str());
}

bool WorkshopUploader::SetLanguage(std::string language)
{
    return SteamUGC()->SetItemUpdateLanguage(m_handle, language.c_str());
}

bool WorkshopUploader::SetVisibility(ERemoteStoragePublishedFileVisibility visibility)
{
    return SteamUGC()->SetItemVisibility(m_handle, visibility);
}

void WorkshopUploader::FinishUpdateItem(std::string changelog)
{
    std::cerr << "Finishing update\n";
    SteamAPICall_t call = SteamUGC()->SubmitItemUpdate(m_handle, changelog.c_str());
    m_update_result.Set(call, this, &WorkshopUploader::submit_callback);
}

bool WorkshopUploader::IsFinished()
{
    return m_finished;
}

Result WorkshopUploader::GetResult()
{
    return m_result;
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
