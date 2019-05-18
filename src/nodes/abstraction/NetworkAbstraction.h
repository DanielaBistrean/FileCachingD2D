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

#ifndef NODES_ABSTRACTION_NETWORKABSTRACTION_H_
#define NODES_ABSTRACTION_NETWORKABSTRACTION_H_

#include <map>

#include "../UE/UE.h"
#include "../gNB/gNB.h"

class NetworkAbstraction {
public:
    static NetworkAbstraction& getInstance ();

public:
    NetworkAbstraction (const NetworkAbstraction &) = delete;
    NetworkAbstraction& operator= (const NetworkAbstraction &) = delete;

public:
    void registerUser (UE * pUser);
    void deregisterUser (UE * pUser);

    void registerBase (gNB * base);
    void deregisterBase ();

public:
    cGate * getNodeGate (int nodeId);
    cGate * getBaseGate ();

    int     getBaseId ();

public:
    std::vector <cGate *> getBroadcastGates (int senderId);

private:
    NetworkAbstraction () : m_base {nullptr} {};

private:
    std::map <int, UE *> m_users;
    gNB * m_base;
};

#endif /* NODES_ABSTRACTION_NETWORKABSTRACTION_H_ */
