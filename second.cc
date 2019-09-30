#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
using namespace ns3;
// Default Network Topology
//
//       10.1.1.0                           5Mbps,2ms    
// n0 -------------- n1   n2   n3   n4    -------------- n5
//    point-to-point  |    |    |    |      
//                    ================
//              		LAN 10.1.2.0

//new program                   

NS_LOG_COMPONENT_DEFINE("secondscriptexample");
int main(int argc, char *argv[])
{
  bool verbose=true;
  uint32_t nCsma=2;    // changed valuie from 3 to 2
  CommandLine cmd;
  cmd.AddValue("nCsma","Number of\"extra\"CSMA nodes/devices",nCsma);
  cmd.AddValue("verbose","Tell echo applications to log if true",verbose);
  cmd.Parse(argc,argv);
  
  
  if(verbose)
  {
  LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
  LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
  }
  
  nCsma=nCsma==0?1:nCsma;
  
  NodeContainer p2pNodes;
  p2pNodes.Create(2);
  
  NodeContainer p2pNodes1;  // madea another node container 
  p2pNodes1.Create(2);      //
  
  NodeContainer csmaNodes;          //all the nodes use csms
  csmaNodes.Add(p2pNodes.Get(1));  
  csmaNodes.Create(nCsma);            
  csmaNodes.Add(p2pNodes1.Get(0));      // 
  
  PointToPointHelper pointtopoint;      // setup point to point connection with given datat rate and channel bandwidth
  pointtopoint.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
  pointtopoint.SetChannelAttribute("Delay",StringValue("2ms"));
  
  CsmaHelper csma;   //idk 
  csma.SetChannelAttribute("DataRate",StringValue("100Mbps"));
  csma.SetChannelAttribute("Delay",TimeValue(NanoSeconds(6560)));
  
  PointToPointHelper pointtopoint1;    //setup another point to pont connection 
  pointtopoint1.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
  pointtopoint1.SetChannelAttribute("Delay",StringValue("2ms"));
  
  
  NetDeviceContainer p2pdevices;
  p2pdevices=pointtopoint.Install(p2pNodes);  
  
  NetDeviceContainer csmadevices;
  csmadevices=csma.Install(csmaNodes); 
  
  NetDeviceContainer p2pdevices1; //
  p2pdevices1=pointtopoint1.Install(p2pNodes1); //
  
  InternetStackHelper stack;
  stack.Install(p2pNodes.Get(0));
  stack.Install(csmaNodes);
  stack.Install(p2pNodes1.Get(1)); //
  
  Ipv4AddressHelper address;    //setup base address for all of them
  address.SetBase("10.1.1.0","255.255.255.0");
  Ipv4InterfaceContainer p2pinterfaces;
  p2pinterfaces=address.Assign(p2pdevices);
  
  address.SetBase("10.1.2.0","255.255.255.0");
  Ipv4InterfaceContainer csmainterfaces;
  csmainterfaces=address.Assign(csmadevices);
  
  
  address.SetBase("10.1.3.0","255.255.255.0");  //
  Ipv4InterfaceContainer p2pinterfaces1;        //
  p2pinterfaces1=address.Assign(p2pdevices1);   //
  
   UdpEchoServerHelper echoserver(9);
   ApplicationContainer serverapps=echoserver.Install(csmaNodes.Get(nCsma));
   serverapps.Start(Seconds(1.0));
   serverapps.Stop(Seconds(10.0));
   
    UdpEchoServerHelper echoserver1(10);
    ApplicationContainer serverapps1=echoserver1.Install(csmaNodes.Get(nCsma));
    serverapps1.Start(Seconds(1.0));
   serverapps1.Stop(Seconds(10.0));
   
   UdpEchoClientHelper echoclient(csmainterfaces.GetAddress(nCsma),9);
   echoclient.SetAttribute("MaxPackets",UintegerValue(1));
   echoclient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
   echoclient.SetAttribute("PacketSize",UintegerValue(1024));
   
   ApplicationContainer clientapps=echoclient.Install(p2pNodes.Get(0));
   clientapps.Start(Seconds(2.0));
   clientapps.Stop(Seconds(10.0));
   
   UdpEchoClientHelper echoclient1(csmainterfaces.GetAddress(nCsma),10);
   echoclient1.SetAttribute("MaxPackets",UintegerValue(2));
   echoclient1.SetAttribute("Interval",TimeValue(Seconds(1.0)));
   echoclient1.SetAttribute("PacketSize",UintegerValue(512));
   
   ApplicationContainer clientapps1=echoclient1.Install(p2pNodes1.Get(1));
   clientapps1.Start(Seconds(2.0));
   clientapps1.Stop(Seconds(10.0));
   
   Ipv4GlobalRoutingHelper::PopulateRoutingTables();
   pointtopoint.EnablePcapAll("second");
   csma.EnablePcap("second",csmadevices.Get(1),true);
   pointtopoint1.EnablePcapAll("second");
   
   
     Simulator::Run();
     Simulator::Destroy();
   return 0;
     }
   
  
