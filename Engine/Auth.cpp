#include "auth.h"
std::string GetUrlData(std::string url)
{
	std::string request_data = "";

	HINTERNET hIntSession = InternetOpenA("", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (!hIntSession)
	{
		return request_data;
	}

	HINTERNET hHttpSession = InternetConnectA(hIntSession, "laifcheats.ru", 80, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);

	if (!hHttpSession)
	{
		return request_data;
	}

	HINTERNET hHttpRequest = HttpOpenRequestA(hHttpSession, "GET", url.c_str()
		, 0, 0, 0, INTERNET_FLAG_RELOAD, 0);

	if (!hHttpSession)
	{
		return request_data;
	}

	const char* szHeaders = ("");
	char szRequest[1024] = { 0 };

	if (!HttpSendRequestA(hHttpRequest, szHeaders, strlen(szHeaders), szRequest, strlen(szRequest)))
	{
		return request_data;
	}

	CHAR szBuffer[1024] = { 0 };
	DWORD dwRead = 0;

	while (InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead)
	{
		request_data.append(szBuffer, dwRead);
	}

	InternetCloseHandle(hHttpRequest);
	InternetCloseHandle(hHttpSession);
	InternetCloseHandle(hIntSession);

	return request_data;
}

namespace Auth
{
	string GetHWID()
	{
		HW_PROFILE_INFO hwProfileInfo;
		string szHwProfileGuid = "";

		if (GetCurrentHwProfileA(&hwProfileInfo) != NULL)
			szHwProfileGuid = hwProfileInfo.szHwProfileGuid;

		return szHwProfileGuid;
	}

	void Auth()
	{
		Credential::LoadConfig();
		string log, pass;
		log = Credential::login;
		pass = Credential::password;
		if (InternetCheckConnection("https://www.google.ru", FLAG_ICC_FORCE_CONNECTION, 0))
		{
			std::string hwid = GetHWID();
			std::string UrlRequest = "/";
			UrlRequest.append("licensecheck?binding=" + hwid + "&login=" + log + "&password=" + pass);
			std::string resCode = GetUrlData(UrlRequest);

			

			if (resCode == "oldversion")
			{
				MessageBoxA(NULL, "Your loader version is not up to date. Please download the loader from your personal account.", "MixtureCheats", MB_OK);
				exit(0);
			}

			if (resCode == "errorHWID")
			{
				MessageBoxA(NULL, "Wrong HWID. Please message to support.", "Loader cheats", MB_OK);
				exit(0);
			}

			if (resCode == "errorTIME")
			{
				MessageBoxA(NULL, "You doesn`t have subscription. Please reactive your subscription.", "Loader cheats", MB_OK);
				exit(0);
			}

			if (resCode == "banned")
			{
				MessageBoxA(NULL, "Your account has been banned.", "Loader cheats", MB_OK);
				exit(0);
			}

			if (resCode == "error0")
			{
				MessageBoxA(NULL, "Cheat has been disabled. Please check news.", "Loader cheats", MB_OK);
				exit(0);
			}

			if (resCode == "error1403")
			{
				MessageBoxA(NULL, "Whats wrong. Error - code: 1403", "Loader cheats", MB_OK);
				exit(0);
			}

			if (resCode == "error1440")
			{
				MessageBoxA(NULL, "Whats wrong. Error - code: 1440", "Loader cheats", MB_OK);
				exit(0);
			}

			if (resCode == "freez")
			{
				MessageBoxA(NULL, "Your subscription has been freezed.", "Loader cheats", MB_OK);
				exit(0);
			}

			
		}
	}
};