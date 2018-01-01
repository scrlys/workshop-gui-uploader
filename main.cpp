// This file is free software under the GPLv3 license

#include <wx/wxprec.h>
#include <steam/steam_api.h>

#include "splash.h"

/*
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
*/

class App : public wxApp
{
public:
	virtual bool OnInit() {
		SteamAPI_Init();
		if (!SteamUGC()) {
			// Ensure that Steam loaded
			wxMessageBox("Steam failed to load. Please ensure Steam is running and steam_appid.txt exists in the same folder the application exists", "Error!", wxICON_ERROR);
			return false;
		}

		SplashFrame *frame = new SplashFrame();
		frame->Show();
		SetTopWindow(frame);
	}

	virtual int OnExit() {
		SteamAPI_Shutdown();
		return 0;
	}
};

IMPLEMENT_APP(App)
