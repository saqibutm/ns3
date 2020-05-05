/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1   n2   n3   n4
//    point-to-point  |    |    |    |
//                    ================
//                      LAN 10.1.2.0

// Important things to note about the given topology..

/* There are 2 p2p nodes + 4 CSMA nodes. 
   However, n1 is part of both p2p and CSMA network.
   Therefore, total nodes in the network are 5. */


/* CSMA stands for Carrier Sense Multiple Access. CSMA/CA and CSMA/CD are commomly used. 
   However, in this example, we are simply using CSMA. */


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nCsma = 3;  // Decleate a variable for three CSMA nodes in the network

  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  nCsma = nCsma == 0 ? 1 : nCsma;  // Makee sure to create at least on CSMA node in the network.

  // Create two p2p nodes i.e., n0 and n1
  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  // Create three CSMA nodes
  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (1));  // Append p2p node to CSMA container
  csmaNodes.Create (nCsma); // the value of CSMA is 3. Thus, total CSMA nodes becomes 4.

  // p2p channel creation and attribute specification
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  // Net device creation and installtion of p2p channel
  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  // csma channel creation and attribute specificaions
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  // net device creation and installtion of csma channel
  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  // Installation of protocol stack
  InternetStackHelper stack;
  stack.Install (p2pNodes.Get (0));
  stack.Install (csmaNodes);
  
  // IP address base class specifications
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  // Create a server application 
  UdpEchoServerHelper echoServer (9);

  // Installation of server application on node n4
  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  // Create a client application + getting the address of server which is listening at port 9
  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  // Insatalltion of client application on p2p node n0
  ApplicationContainer clientApps = echoClient.Install (p2pNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  // Populating routing tables
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // enabling pcap trace files whcih can be read through wireshark or tcpdump
  pointToPoint.EnablePcapAll ("p2p");
  csma.EnablePcap ("csma1", csmaDevices.Get (1), true);
  csma.EnablePcap ("csma3", csmaDevices.Get (3), true);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
