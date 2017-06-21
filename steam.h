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

class UpdateWorkshop
{
private:
    PublishedFileId_t m_fileid;
    UGCUpdateHandle_t m_handle;

    AppId_t m_game;

    std::string m_changelog;

    bool m_finished;
    Result m_result;

    CCallResult<UpdateWorkshop, SubmitItemUpdateResult_t> m_update_result;

    void submit_callback(SubmitItemUpdateResult_t *result, bool failure);

public:
    UpdateWorkshop(PublishedFileId_t fileid, AppId_t game);

    void StartUpdateItem();

    bool SetTitle(std::string title);
    bool SetDescription(std::string description);
    bool SetPreviewImage(std::string image);
    bool SetLanguage(std::string language);
    bool SetVisibility(ERemoteStoragePublishedFileVisibility visibility);
    void SetChangelog(std::string changelog);

    void FinishUpdateItem(std::string changelog);

    bool IsFinished();
    Result GetResult();
};

class CreateWorkshop
{
private:
    PublishedFileId_t m_fileid;
    AppId_t m_game;

    bool m_finished;
    Result m_result;

    CCallResult<CreateWorkshop, CreateItemResult_t> m_create_result;

    void create_callback(CreateItemResult_t *result, bool failure);

public:
    CreateWorkshop(AppId_t game);

    void CreateItem();

    bool IsFinished();
    Result GetResult();

    PublishedFileId_t GetFileID();
};

AppId_t GetAppIdFromFile();
