#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

int main(int argc, char *argv[]) {
    NodeContainer nodes;
    nodes.Create(30);

    NodeContainer busNodes;
    for (int i = 0; i <= 4; ++i) busNodes.Add(nodes.Get(i));
    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));
    NetDeviceContainer busDevices = csma.Install(busNodes);

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("2ms"));

    p2p.Install(nodes.Get(1), nodes.Get(5));
    for (int i = 6; i <= 10; ++i) {
        p2p.Install(nodes.Get(5), nodes.Get(i));
    }

    p2p.Install(nodes.Get(2), nodes.Get(11));
    for (int i = 11; i <= 16; ++i) {
        p2p.Install(nodes.Get(i), nodes.Get(11 + (i - 11 + 1) % 6));
    }

    p2p.Install(nodes.Get(3), nodes.Get(17));
    for (int i = 17; i <= 22; ++i) {
        for (int j = i + 1; j <= 22; ++j) {
            p2p.Install(nodes.Get(i), nodes.Get(j));
        }
    }

    p2p.Install(nodes.Get(4), nodes.Get(23));
    p2p.Install(nodes.Get(23), nodes.Get(24));
    p2p.Install(nodes.Get(23), nodes.Get(25));
    p2p.Install(nodes.Get(24), nodes.Get(26));
    p2p.Install(nodes.Get(24), nodes.Get(27));
    p2p.Install(nodes.Get(25), nodes.Get(28));
    p2p.Install(nodes.Get(25), nodes.Get(29));

    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(0.0), "MinY", DoubleValue(0.0),
                                  "DeltaX", DoubleValue(20.0), "DeltaY", DoubleValue(20.0),
                                  "GridWidth", UintegerValue(6),
                                  "LayoutType", StringValue("RowFirst"));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(nodes);

    AnimationInterface anim("hybrid_topology.xml");
    
    anim.UpdateNodeDescription(0, "Backbone-0");
    anim.UpdateNodeColor(nodes.Get(5), 255, 0, 0);   
    anim.UpdateNodeColor(nodes.Get(23), 0, 255, 0);  

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
