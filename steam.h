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
    EGeneral,
	ENotFound,
	ELimit,
};

class UpdateWorkshop
{
private:
    PublishedFileId_t m_fileid;
    UGCUpdateHandle_t m_handle;

    AppId_t m_game;

    std::string m_changelog;

    bool m_finished = false;
    EResult m_result;

	EItemUpdateStatus m_status;
	uint64 m_bytes_uploaded;
	uint64 m_bytes_total;

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
	bool SetItemContent(std::string path);
    void SetChangelog(std::string changelog);

    void FinishUpdateItem();

    bool IsFinished();
    EResult GetResult();

	void UpdateStats();
	uint64 GetUploadedBytes();
	uint64 GetTotalBytes();
	EItemUpdateStatus GetStatus();
};

class CreateWorkshop
{
private:
    PublishedFileId_t m_fileid;
    AppId_t m_game;

    bool m_finished = false;
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
