#include "Engine.h"
#pragma once


#define ENCRYPTION_KEY 0xA67D7

namespace Credential
{
	extern bool save_credential;
	extern char login[256];
	extern char password[256];

	void SaveConfig();
	void LoadConfig();
};