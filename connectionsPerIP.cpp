#include "bzfsAPI.h"
#include "plugin_utils.h"

class connectionsPerIP : public bz_Plugin
{
public:
	virtual const char* Name (){return "Connections Per IP";}
	virtual void Init ( const char* config);
	virtual void Cleanup (void);

	virtual void Event ( bz_EventData * /* eventData */ );

	int maxConnectionsPerIP;
};

BZ_PLUGIN(connectionsPerIP);

void connectionsPerIP::Init(const char* commandLine)
{
	bz_debugMessage(4, "connectionsPerIP plugin loaded");

	if(commandLine != "" && atoi(commandLine))
		maxConnectionsPerIP = atoi(commandLine);
	else
		maxConnectionsPerIP = 2;

	Register(bz_eAllowPlayer);
}

void connectionsPerIP::Cleanup(void)
{
	bz_debugMessage(4, "connectionsPerIP plugin unloaded");

	Flush();
}

void connectionsPerIP::Event(bz_EventData* eventData)
{
	switch (eventData->eventType)
	{
		case bz_eAllowPlayer: // A player starts the connection to the server
		{
			bz_AllowPlayerEventData_V1* allowPlayer = (bz_AllowPlayerEventData_V1*)eventData;
			bz_APIIntList *playerlist = bz_newIntList();
			bz_getPlayerIndexList(playerlist);
			int conflicts = 0;

			for (int i = 0; i < playerlist->size(); i++)
			{
				if (allowPlayer->ipAddress == bz_getPlayerByIndex(playerlist->get(i))->ipAddress)
				{
					conflicts++;

					if (conflicts > maxConnectionsPerIP)
					{
						allowPlayer->allow = false;
						allowPlayer->reason = "You have exceeded the maximum connections allowed to this server per IP address.";
						bz_deleteIntList(playerlist);
						return;
					}
				}
			}

			bz_deleteIntList(playerlist);
		}
		break;

		default: break;
	}
}