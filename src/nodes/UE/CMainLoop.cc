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

#include "CMainLoop.h"

#include "../messages/UENotification_m.h"

CMainLoop::CMainLoop(INode * pNode, CCacheManager * pCacheManager, CNetworkManager * pNetworkManager)
: m_pNode           {pNode}
, m_pCacheManager   {pCacheManager}
, m_pNetworkManager {pNetworkManager}
{
    if (m_pNode->getNode ()->getId () == 2)
        do_scheduleNextAttempt ();
}

void
CMainLoop::process (omnetpp::cMessage * pMsg)
{
    if (! pMsg->isSelfMessage ())
        return;

    UENotification * pNotification = dynamic_cast <UENotification *> (pMsg);

    if (! pNotification)
        return;

    switch (pNotification->getType ())
    {
    case N_SCHEDULE:
        do_attemptFileDownload ();
        break;
    default:
        break;
    }
}

void
CMainLoop::do_attemptFileDownload ()
{
    FileId fileId;
    if (m_pCacheManager->selectFileForDownload (fileId))
        m_pNetworkManager->requestFile(fileId);

    do_scheduleNextAttempt ();
}

void
CMainLoop::do_scheduleNextAttempt ()
{
    UENotification * pNotification = new UENotification ();
    pNotification->setType (N_SCHEDULE);

    omnetpp::cRNG * random = omnetpp::getSimulation()->getSystemModule()->getRNG(0);

    // New schedule can be in interval [10, 20).
    m_pNode->sendInternal (pNotification, 10 + (random->doubleRand () * 10));
}
