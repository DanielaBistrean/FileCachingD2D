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
NetworkAbstraction::getUserGate (int userId)
{
    auto user = m_users.find (userId);
    if (user == m_users.end ())
        return nullptr;

    return user->second->gate ("radioIn");
}

cGate *
NetworkAbstraction::getBaseGate ()
{
    if (! m_base)
        return nullptr;

    return m_base->gate ("radioIn");
}
