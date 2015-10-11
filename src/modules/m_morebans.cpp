#include <stdlib.h>
#include "inspircd.h"

/* $ModDesc: Allows increasing the ban list limit for specific channels with channel mode +E (max) */

class LargeBanListMode : public ModeHandler
{
 public:
	LargeBanListMode(Module* Creator) : ModeHandler(Creator, "largebanlist", 'E', PARAM_SETONLY, MODETYPE_CHANNEL)
	{
		oper = true;
	}

	ModeAction OnModeChange(User* source, User* dest, Channel* channel, std::string &parameter, bool adding)
	{
		if (channel->IsModeSet(this) == adding)
			return MODEACTION_DENY;
        long newmax = strtol(parameter.c_str(), NULL, 10);
        if (newmax <= channel->GetMaxBans())
            return MODEACTION_DENY;
        ServerInstance->Config->maxbans[channel->name] = newmax;
        channel->ResetMaxBans();
		channel->SetModeParam(this, parameter);
		return MODEACTION_ALLOW;
	}
};

class ModuleLargeBanList : public Module
{
 private:
	LargeBanListMode mode;
 public:
	ModuleLargeBanList() : mode(this)
	{
	}

	void init()
	{
		ServerInstance->Modules->AddService(mode);

        Implementation eventlist[] = { I_OnRehash };
		ServerInstance->Modules->Attach(eventlist, this, sizeof(eventlist)/sizeof(Implementation));

		OnRehash(NULL);
	}

	~ModuleLargeBanList()
	{
	}

	void OnRehash(User* user)
	{
        for (chan_hash::iterator i = ServerInstance->chanlist->begin(); i != ServerInstance->chanlist->end(); )
        {
            Channel* channel = i->second;
            ++i;
            if (channel->IsModeSet('E'))
            {
                long newmax = strtol(channel->GetModeParameter('E').c_str(), NULL, 10);
                ServerInstance->Config->maxbans[channel->name] = newmax;
                channel->ResetMaxBans();
            }
        }
	}

	Version GetVersion()
	{
		return Version("Allows increasing the ban list limit for specific channels with channel mode +l (max)", VF_VENDOR);
	}
};

MODULE_INIT(ModuleLargeBanList)
