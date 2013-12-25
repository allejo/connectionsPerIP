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

    // Store all the current IPs on the server in a map along with their count
    // of connections
    std::map <std::string, int> currentIPs;

    // The number of maximum connections per IP
    int maxConnectionsPerIP;
};

BZ_PLUGIN(ConnectionsPerIP)

void ConnectionsPerIP::Init (const char* commandLine)
{
    // Register our events with Register()
    Register(bz_eAllowPlayer);
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

    bz_debugMessagef(2, "DEBUG :: Connections Per IP :: Maximum number of connections allowed set to: %i", maxConnectionsPerIP);
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

            // Check if we have their IP recorded
            if (currentIPs.find(allowPlayerData->ipAddress.c_str()) == currentIPs.end())
            {
                // If it's the first time an IP has joined, set their connection count to 1
                currentIPs[allowPlayerData->ipAddress.c_str()] = 1;
            }
            else
            {
                // If the IP is found in the list, then that means they already have at least
                // one connection, so simply increment their connection count
                currentIPs[allowPlayerData->ipAddress.c_str()]++;
            }

            // If an IP has reached it's limit of connections, do not allow them to join any more
            if (currentIPs[allowPlayerData->ipAddress.c_str()] >= maxConnectionsPerIP)
            {
                allowPlayerData->allow = false;
                allowPlayerData->reason = "You have exceeded the maximum connections allowed to this server per IP address.";
            }
        }
        break;

        case bz_ePlayerPartEvent: // This event is called each time a player leaves a game
        {
            bz_PlayerJoinPartEventData_V1* partData = (bz_PlayerJoinPartEventData_V1*)eventData;

            // Once a player leaves, that means that connection has been cut so we can safely
            // decrement the number of connections an IP has
            currentIPs[partData->record->ipAddress.c_str()]--;
        }
        break;

        default: break;
    }
}
