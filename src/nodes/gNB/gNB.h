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

#ifndef __FILECACHINGD2D_GNB_H_
#define __FILECACHINGD2D_GNB_H_

#include <memory>
#include <omnetpp.h>

#include "../abstraction/INode.h"
#include "../abstraction/CFileSource.h"
#include "../transfer/CFileStore.h"
#include "../abstraction/CCacheManager.h"

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class gNB : public cSimpleModule, public INode
{
public:
    gNB () : m_cache (true) {};

public:
    virtual omnetpp::cSimpleModule* getNode () override;

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

private:
    CFileStore m_cache;

    std::unique_ptr <CCacheManager> m_pCacheManager;
    std::unique_ptr <CFileSource>   m_pFileSource;
};

#endif
