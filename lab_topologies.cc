#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"

using namespace ns3;

int main(int argc, char *argv[]) {
    int topologyType = 1; 
    CommandLine cmd;
    cmd.AddValue("type", "1: Bus, 2: Star, 3: Ring, 4: Mesh, 5: Tree", topologyType);
    cmd.Parse(argc, argv);

    NodeContainer nodes;
    std::string animFile = "";
    MobilityHelper mobility;
    
    // Position settings
    double deltaX = 20.0;
    double deltaY = 20.0;
    uint32_t gridWidth = 5;

    if (topologyType == 1) { // 1. BUS
        animFile = "bus_topology.xml";
        nodes.Create(20);
        CsmaHelper csma;
        csma.Install(nodes);
        gridWidth = 20; // Long line
    } 
    else if (topologyType == 2) { // 2. STAR
        animFile = "star_topology.xml";
        nodes.Create(21);
        PointToPointHelper p2p;
        for (uint32_t i = 1; i < 21; ++i) p2p.Install(nodes.Get(0), nodes.Get(i));
        gridWidth = 5; 
    }
    else if (topologyType == 3) { // 3. RING
        animFile = "ring_topology.xml";
        nodes.Create(25);
        PointToPointHelper p2p;
        for (uint32_t i = 0; i < 25; ++i) p2p.Install(nodes.Get(i), nodes.Get((i + 1) % 25));
        gridWidth = 5;
    }
    else if (topologyType == 4) { // 4. MESH
        animFile = "mesh_topology.xml";
        nodes.Create(15);
        PointToPointHelper p2p;
        for (uint32_t i = 0; i < 15; ++i) 
            for (uint32_t j = i + 1; j < 15; ++j) 
                p2p.Install(nodes.Get(i), nodes.Get(j));
        gridWidth = 4;
    }
    else if (topologyType == 5) { // 5. TREE
        animFile = "tree_topology.xml";
        nodes.Create(21);
        PointToPointHelper p2p;
        for (int i = 1; i <= 4; i++) p2p.Install(nodes.Get(0), nodes.Get(i));
        int childIdx = 5;
        for (int p = 1; p <= 4; p++) 
            for (int c = 0; c < 4; c++) 
                p2p.Install(nodes.Get(p), nodes.Get(childIdx++));
        gridWidth = 5;
    }

    mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue (0.0), "MinY", DoubleValue (0.0),
                                   "DeltaX", DoubleValue (deltaX), "DeltaY", DoubleValue (deltaY),
                                   "GridWidth", UintegerValue (gridWidth),
                                   "LayoutType", StringValue ("RowFirst"));
    
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (nodes);

    AnimationInterface anim(animFile);
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
