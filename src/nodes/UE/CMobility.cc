#include "CMobility.h"

#include "../../configuration/CGlobalConfiguration.h"

#include "../messages/PositionUpdate_m.h"

CMobility::CMobility (INode * pNode)
: m_pNode {pNode}
{
    CGlobalConfiguration &config = CGlobalConfiguration::getInstance ();

    m_radius         = config.get ("radius");
    m_cx             = config.get ("cx");
    m_cy             = config.get ("cy");

    m_updateInterval = double (config.get ("updateInterval")) / 1000;

    initialize ();
}

CMobility::CMobility (INode * pNode, double x, double y, double velocity, double heading)
: CMobility {pNode}
{
    initialize (x, y, velocity, heading);
}


void
CMobility::process (omnetpp::cMessage * pMsg)
{
    PositionUpdate *pUpdate = dynamic_cast <PositionUpdate *> (pMsg);

    if (pUpdate)
        do_update ();
}


void
CMobility::initialize (double x, double y, double velocity, double heading)
{
    m_posX = x;
    m_posY = y;
    m_velocity = velocity;
    m_heading = heading;

    do_update ();
}

void
CMobility::initialize ()
{
    omnetpp::cRNG * random = omnetpp::getSimulation()->getSystemModule()->getRNG(0);

    CGlobalConfiguration &config = CGlobalConfiguration::getInstance ();

    double deltaHeadingMax = config.get ("deltaHeadingMax");
    double velocityMax     = config.get ("velocityMax");

    // Dir is an angle, distance is between 1 and radius
    double dir      = random->doubleRand() * 2 * PI;
    double distance = random->doubleRand() * (m_radius - 1) + 1;

    double headingCenter = PI - dir;

    m_posX = m_cx + (distance * cos(dir));
    m_posY = m_cy + (distance * sin(dir));

    m_velocity = random->doubleRand() * (velocityMax - 1) + 1;
    m_heading = headingCenter + random->doubleRand() * (deltaHeadingMax * 2) - deltaHeadingMax;

    do_update ();
}

void
CMobility::do_update ()
{
    m_posX += m_velocity * cos (m_heading);
    m_posY += m_velocity * sin (m_heading);

    double dist = sqrt ((m_cx - m_posX) * (m_cx - m_posX) + (m_cy - m_posY) * (m_cy - m_posY));
    if (dist > m_radius)
    {
        // node is outside of radius, replace it at the other side of the circle
        m_posX = (2 * m_cx) - m_posX;
        m_posY = (2 * m_cy) - m_posY;
    }

    m_pNode->getNode ()->getDisplayString ().setTagArg ("p", 0, long (m_posX));
    m_pNode->getNode ()->getDisplayString ().setTagArg ("p", 1, long (m_posY));

    PositionUpdate *pUpdate = new PositionUpdate ();
    m_pNode->sendInternal (pUpdate, m_updateInterval);
}
