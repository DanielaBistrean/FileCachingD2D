#ifndef D2D_CGLOBALCONFIGURATION_H
#define D2D_CGLOBALCONFIGURATION_H 1

#include <omnetpp.h>

class CGlobalConfiguration
{
public:
    static CGlobalConfiguration& getInstance ();

    omnetpp::cPar& get (const std::string &sName);

public:
    CGlobalConfiguration (CGlobalConfiguration const&) = delete;
    void operator=       (CGlobalConfiguration const&) = delete;

private:
    CGlobalConfiguration ();

private:
    omnetpp::cSimulation * m_pSimulation;
};

#endif // D2D_CGLOBALCONFIGURATION_H
