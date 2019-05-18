#include "CGlobalConfiguration.h"

CGlobalConfiguration::CGlobalConfiguration ()
: m_pSimulation {omnetpp::getSimulation ()}
{}

omnetpp::cPar &
CGlobalConfiguration::get (const std::string &sName)
{
    return m_pSimulation->getSystemModule ()->par (sName.c_str ());
}

CGlobalConfiguration &
CGlobalConfiguration::getInstance ()
{
    static CGlobalConfiguration s_instance;
    return s_instance;
}
