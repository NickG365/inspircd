#include "inspircd.h"

/* $ModDesc: Implements extban +b J: - check bans from other channels */

class ModuleBanSyncExtban : public Module
{
 private:
 public:
	void init()
	{
		Implementation eventlist[] = { I_OnCheckBan, I_On005Numeric };
		ServerInstance->Modules->Attach(eventlist, this, sizeof(eventlist)/sizeof(Implementation));
	}

	~ModuleBanSyncExtban()
	{
	}

	Version GetVersion()
	{
		return Version("Extban 'J' - check bans from other channels", VF_OPTCOMMON|VF_VENDOR);
	}

	ModResult OnCheckBan(User *user, Channel *c, const std::string& mask)
	{
		if ((mask.length() > 2) && (mask[0] == 'J') && (mask[1] == ':'))
		{
			std::string rm = mask.substr(2);
			if (ServerInstance->FindChan(rm)->IsBanned(user))
				return MOD_RES_DENY;
		}
		return MOD_RES_PASSTHRU;
	}

	void On005Numeric(std::string &output)
	{
		ServerInstance->AddExtBanChar('J');
	}
};

MODULE_INIT(ModuleBanSyncExtban)

