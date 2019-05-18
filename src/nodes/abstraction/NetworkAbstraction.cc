//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "NetworkAbstraction.h"

#include "../../configuration/CGlobalConfiguration.h"

NetworkAbstraction&
NetworkAbstraction::getInstance ()
{
    static NetworkAbstraction s_instance;
    return s_instance;
}

void
NetworkAbstraction::registerUser (UE * pUser)
{
    if (! pUser)
        return;

    m_users [pUser->getId ()] = pUser;
}

void
NetworkAbstraction::deregisterUser (UE * pUser)
{
    if (! pUser)
        return;

    m_users.erase (pUser->getId ());
}

void
NetworkAbstraction::registerBase (gNB * base)
{
    m_base = base;
}

void
NetworkAbstraction::deregisterBase ()
{
    m_base = nullptr;
}

cGate *
NetworkAbstraction::getNodeGate (int nodeId)
{
    auto user = m_users.find (nodeId);
    if (user != m_users.end ())
        return user->second->gate ("radioIn$i");

    if (m_base && m_base->getId () == nodeId)
        return m_base->gate ("radioIn$i");

    return nullptr;
}

cGate *
NetworkAbstraction::getBaseGate ()
{
    if (! m_base)
        return nullptr;

    return m_base->gate ("radioIn$i");
}

int
NetworkAbstraction::getBaseId ()
{
    if (! m_base)
        return -1;

    return m_base->getId ();
}

std::vector <cGate *>
NetworkAbstraction::getBroadcastGates (int senderId)
{
    auto it = m_users.find (senderId);
    if (it == m_users.end ())
        return {};

    UE * senderNode = it->second;

    std::vector <cGate *> out;

    static double maxRange = CGlobalConfiguration::getInstance ().get ("D2DMaxRange");

    for (auto & user : m_users)
    {
        if (user.first == senderId)
            continue;

        UE * destNode = user.second;

        double dist = sqrt((destNode->getX() - senderNode->getX()) * (destNode->getX() - senderNode->getX()) + (destNode->getY() - senderNode->getY()) * (destNode->getY() - senderNode->getY()));
        if (dist < maxRange)
            out.push_back (destNode->gate ("radioIn$i"));
    }

    return out;
}
