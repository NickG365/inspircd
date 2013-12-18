#include "inspircd.h"

class BridgeChannel : public Module
{
    public:

        void init()
        {
            ServerInstance->Modules->Attach(I_OnUserPreJoin, this);
        }

        virtual ModResult OnUserPreJoin(User *user, Channel *chan, const char *cname, std::string &privs, const std::string &keygiven)
        {
            if ( cname[ 1 ]  == '^' )
            {
                user->WriteNumeric( 384, "%s :Cannot join channel, reserved for bridges", cname );
                ServerInstance->SNO->WriteGlobalSno( 'a', "%s tried to join bridge channel %s", user->nick.c_str(), cname );
                return MOD_RES_DENY;
            }
            return MOD_RES_PASSTHRU;
        }

        Version GetVersion()
        {
            return Version( "Prevents clients from creating channels starting with ^" );
        }
};

MODULE_INIT(BridgeChannel)
