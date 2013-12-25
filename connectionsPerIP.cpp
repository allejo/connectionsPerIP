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

#include <memory>

#include "bzfsAPI.h"
#include "plugin_utils.h"

class ConnectionsPerIP : public bz_Plugin
{
public:
    virtual const char* Name () {return "Connections Per IP";}
    virtual void Init (const char* config);
    virtual void Event (bz_EventData *eventData);
    virtual void Cleanup (void);

    std::map <std::string, int> currentIPs;
    int maxConnectionsPerIP;
};

BZ_PLUGIN(ConnectionsPerIP)

void ConnectionsPerIP::Init (const char* commandLine)
{
    // Register our events with Register()
    Register(bz_eAllowPlayer);
    Register(bz_ePlayerJoinEvent);
    Register(bz_ePlayerPartEvent);

    // Check to see if the command line has a parameter so we can change
    // the default amount of max connections. Make sure that the specified
    // value is greater than or equal to one or else it would be silly
    if (commandLine != "" && atoi(commandLine) >= 1)
    {
        maxConnectionsPerIP = atoi(commandLine);
    }
    else // The user didn't specify an amount of connections so use the default
    {
        maxConnectionsPerIP = 2;
    }
}

void ConnectionsPerIP::Cleanup (void)
{
    Flush(); // Clean up all the events
}

void ConnectionsPerIP::Event(bz_EventData *eventData)
{
    switch (eventData->eventType)
    {
        case bz_eAllowPlayer: // This event is called each time a player connects to the server
        {
            bz_AllowPlayerEventData_V1* allowPlayerData = (bz_AllowPlayerEventData_V1*)eventData;

            if (currentIPs[allowPlayerData->ipAddress] >= maxConnectionsPerIP)
            {
                allowPlayerData->allow = false;
                allowPlayerData->reason = "You have exceeded the maximum connections allowed to this server per IP address.";
            }
        }
        break;

        case bz_ePlayerJoinEvent: // This event is called each time a player joins the game
        {
            bz_PlayerJoinPartEventData_V1* joinData = (bz_PlayerJoinPartEventData_V1*)eventData;

            if (currentIPs.find(joinData->record->ipAddress.c_str()) == grade_list.end())
            {
                currentIPs[joinData->record->ipAddress] = 1;
            }
            else
            {
                currentIPs[joinData->record->ipAddress]++;
            }
        }
        break;

        case bz_ePlayerPartEvent: // This event is called each time a player leaves a game
        {
            bz_PlayerJoinPartEventData_V1* partData = (bz_PlayerJoinPartEventData_V1*)eventData;

            currentIPs[partData->record->ipAddress]--;
        }
        break;

        default: break;
    }
}
