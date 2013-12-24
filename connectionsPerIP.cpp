/*
Connections Per IP
    Copyright (C) 2013 Vladimir "allejo" Jimenez

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "bzfsAPI.h"
#include "plugin_utils.h"

class connectionsPerIP : public bz_Plugin
{
public:
    virtual const char* Name (){return "Connections Per IP";}
    virtual void Init (const char* config);
    virtual void Event (bz_EventData *eventData);
    virtual void Cleanup (void);

    int maxConnectionsPerIP;
};

BZ_PLUGIN(connectionsPerIP)

void connectionsPerIP::Init(const char* commandLine)
{
    bz_debugMessage(4, "connectionsPerIP plugin loaded");

    if (commandLine != "" && atoi(commandLine))
    {
        maxConnectionsPerIP = atoi(commandLine);
    }
    else
    {
        maxConnectionsPerIP = 2;
    }

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
        case bz_eAllowPlayer:
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