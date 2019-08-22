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

#include "gNB.h"

Define_Module(gNB);

#include "../abstraction/NetworkAbstraction.h"
#include "../../configuration/CGlobalConfiguration.h"

void gNB::initialize()
{
    NetworkAbstraction::getInstance ().registerBase (this);
    CGlobalConfiguration &config = CGlobalConfiguration::getInstance ();

    m_pCache = std::unique_ptr <CFileStore> (new CFileStore (config.get("fileSize"), true));

    m_pCacheManager = std::unique_ptr <CCacheManager> (new CCacheManager (this, m_pCache.get()));
    m_pFileSource   = std::unique_ptr <CFileSource>   (new CFileSource   (this, m_pCacheManager.get ()));
}

void gNB::handleMessage(cMessage *msg)
{
    m_pFileSource->process (msg);

    delete msg;
}

omnetpp::cSimpleModule* gNB::getNode ()
{
    return this;
}

void gNB::finish()
{
    NetworkAbstraction::getInstance ().deregisterBase ();
}
