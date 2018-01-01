#pragma once

#include <wx/wxprec.h>

// Compile time. Assuming the game and uploading app are the same.
#define APP_ID 571880
#define APP_NAME "Angels with Scaly Wings"

// wxWidget IDs
enum
{
	// SplashFrame @ splash.cpp
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
	TEXT_changelog,
	TEXT_id,
	BUTTON_copy_id,
	COMBO_visibility,
};