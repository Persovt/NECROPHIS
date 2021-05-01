#include "Credental.h"

namespace Credential
{
	bool save_credential = false;
	char login[256] = "";
	char password[256] = "";

	void LoadConfig()
	{
		DWORD dwBufLen = MAX_PATH;
		RegGetValue(HKEY_CURRENT_USER, "Software\\MIXTURE", ("login"), RRF_RT_REG_SZ, NULL, (BYTE*)login, &dwBufLen);
		RegGetValue(HKEY_CURRENT_USER, "Software\\MIXTURE", ("password"), RRF_RT_REG_SZ, NULL, (BYTE*)password, &dwBufLen);
	}
}
