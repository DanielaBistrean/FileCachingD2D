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

#include "UE.h"

Define_Module(UE);

#include "../abstraction/NetworkAbstraction.h"
#include "../../configuration/CGlobalConfiguration.h"

void UE::initialize()
{
    m_sigVelocity = registerSignal("velocity");

    NetworkAbstraction::getInstance ().registerUser (this);
    CGlobalConfiguration &config = CGlobalConfiguration::getInstance ();

    m_pCache = std::unique_ptr <CFileStore> (new CFileStore (config.get("fileSize")));

    m_pMobility       = std::unique_ptr <CMobility>       (new CMobility       (this));
    m_pCacheManager   = std::unique_ptr <CCacheManager>   (new CCacheManager   (this, m_pCache.get()));
    m_pFileSink       = std::unique_ptr <CFileSink>       (new CFileSink       (this, m_pCacheManager.get ()));
    m_pFileSource     = std::unique_ptr <CFileSource>     (new CFileSource     (this, m_pCacheManager.get ()));
    m_pNetworkManager = std::unique_ptr <CNetworkManager> (new CNetworkManager (this, m_pFileSink.get (), m_pCacheManager.get ()));
    m_pMainLoop       = std::unique_ptr <CMainLoop>       (new CMainLoop       (this, m_pCacheManager.get (), m_pNetworkManager.get ()));
}

void UE::handleMessage(cMessage *msg)
try
{
    m_pMainLoop->process       (msg);
    m_pMobility->process       (msg);
    m_pFileSink->process       (msg);
    m_pFileSource->process     (msg);
    m_pCacheManager->process   (msg);
    m_pNetworkManager->process (msg);

    delete msg;
}
catch (std::exception &e)
{
    EV_ERROR << e.what () << std::endl;
    EV_ERROR << msg->getName () << std::endl;
}

void UE::finish()
{
    NetworkAbstraction::getInstance ().deregisterUser (this);
}

omnetpp::cSimpleModule * UE::getNode ()
{
    return this;
}
