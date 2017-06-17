#pragma once

#include <steam_api.h>
#include <string>


enum Result
{
    Success,
    ELegal,
    EPermissions,
    ETimeout,
    ELoggedOut,
    EGeneral
};

class WorkshopUploader
{
private:
    AppId_t m_game;
    PublishedFileId_t m_fileid;
    UGCUpdateHandle_t m_handle;

    bool m_finished;
    Result m_result;

    // Call results
    // TODO: Is there a better way we can structure this?
    CCallResult<WorkshopUploader, CreateItemResult_t> m_create_result;
    CCallResult<WorkshopUploader, SubmitItemUpdateResult_t> m_update_result;

    // Callback functions
    void create_callback(CreateItemResult_t *result, bool failure);
    void submit_callback(SubmitItemUpdateResult_t *result, bool failure);

public:
    WorkshopUploader(AppId_t game);

    void CreateItem();
    void StartUpdateItem();

    bool SetTitle(std::string title);
    bool SetDescription(std::string description);
    bool SetPreviewImage(std::string image);
    bool SetLanguage(std::string language);
    bool SetVisibility(ERemoteStoragePublishedFileVisibility visibility);

    void FinishUpdateItem(std::string changelog);

    // Getters
    bool IsFinished();
    Result GetResult();
};

AppId_t GetAppIdFromFile();
