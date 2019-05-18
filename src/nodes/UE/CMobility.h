#ifndef D2D_CMOBILITY_HPP
#define D2D_CMOBILITY_HPP 1

#include "../abstraction/IProcessor.h"
#include "../abstraction/INode.h"

class CMobility : public IProcessor
{
public:
    CMobility (INode * pNode);
    CMobility (INode * pNode, double x, double y, double velocity, double heading);

public:
    virtual void process (omnetpp::cMessage * pMsg) override;

public:
    void initialize (double x, double y, double velocity, double heading);
    void initialize ();

    double getX () { return m_posX; };
    double getY () { return m_posY; };

private:
    void do_update ();

private:
    INode * m_pNode;

    double  m_posX;
    double  m_posY;

    double  m_heading;
    double  m_velocity;

    double  m_updateInterval;
    double  m_cx;
    double  m_cy;
    double  m_radius;
};

#endif // D2D_CMOBILITY_HPP
