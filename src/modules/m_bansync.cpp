/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2015 NickG365 <nick+inspircd@gameon365.net>
 *
 * This file is part of InspIRCd.  InspIRCd is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* $ModAuthor: NickG365 */
/* $ModAuthorMail: nick+inspircd@gameon365.net */
/* $ModDesc: Implements extban +b J: - check bans from other channels */

#include "inspircd.h"

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

