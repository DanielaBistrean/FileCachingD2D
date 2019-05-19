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

#ifndef __FILECACHINGD2D_UE_H_
#define __FILECACHINGD2D_UE_H_

#include <omnetpp.h>

#include <memory>

#include "../messages/DataPacket_m.h"
#include "../abstraction/INode.h"
#include "../abstraction/CFileSink.h"
#include "../abstraction/CFileSource.h"
#include "../abstraction/CCacheManager.h"
#include "../abstraction/CNetworkManager.h"
#include "../transfer/CFileStore.h"

#include "CMobility.h"
#include "CMainLoop.h"

using namespace omnetpp;

class UE : public cSimpleModule, public INode
{
public:
    virtual omnetpp::cSimpleModule* getNode () override;

public:
    double getX () { return m_pMobility->getX (); };
    double getY () { return m_pMobility->getY (); };

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

private:
    CFileStore m_cache;

    std::unique_ptr <CMainLoop>       m_pMainLoop;
    std::unique_ptr <CMobility>       m_pMobility;
    std::unique_ptr <CFileSink>       m_pFileSink;
    std::unique_ptr <CFileSource>     m_pFileSource;
    std::unique_ptr <CCacheManager>   m_pCacheManager;
    std::unique_ptr <CNetworkManager> m_pNetworkManager;
};

#endif
