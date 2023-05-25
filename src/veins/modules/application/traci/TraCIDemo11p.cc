//
// Copyright (C) 2006-2011 Christoph Sommer <christoph.sommer@uibk.ac.at>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/application/traci/TraCIDemo11p.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/messages/DemoSafetyMessage_m.h"
#include <sstream>
#include <chrono>
#include <thread>

#include "veins/base/utils/Coord.h"
//#include "string.h"
#include "veins/base/utils/Coord.h"
using namespace veins;
//string EMERGENCY_VEHICLE_NODE= "0";
bool sentMessage=false;
int bsmSentCount = 0;
Define_Module(veins::TraCIDemo11p);

void TraCIDemo11p::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);

    if (stage == 0) {
        // Initialize variables


        // Schedule the first BSM message to be sent after 0 seconds
        scheduleAt(simTime(), new cMessage("sendBSM"));

        EV_WARN << "Initialized TraCIDemo11p module" << endl;
    }
}

void TraCIDemo11p::onWSA(DemoServiceAdvertisment* wsa)
{
    if (currentSubscribedServiceId == -1) {
        mac->changeServiceChannel(static_cast<Channel>(wsa->getTargetChannel()));
        currentSubscribedServiceId = wsa->getPsid();
        if (currentOfferedServiceId != wsa->getPsid()) {
            stopService();
            startService(static_cast<Channel>(wsa->getTargetChannel()), wsa->getPsid(), "Mirrored Traffic Service");
        }
    }
}

void TraCIDemo11p::onBSM(DemoSafetyMessage* bsm)
{

        mobility = TraCIMobilityAccess().get(getParentModule());
        traci = mobility->getCommandInterface();
        traciVehicle = mobility->getVehicleCommandInterface();

        if (bsm != nullptr) {
            Coord senderPos = bsm->getSenderPos();


            // Do something with the position and speed, e.g., print them
           /* EV_INFO << "Received BSM message from vehicle "
                    << " at position (" << senderPos.x << ", " << senderPos.y << ", " << senderPos.z << ")"
                    << " with speed " << endl;*/
        int nodeID = getParentModule()->getIndex();
        if (nodeID==1){




            Coord receivePos = mobility->getPositionAt(simTime());

            /*EV_INFO << "Received BSM message from vehicle "<< nodeID
                    << " at position (" << receivePos.x << ", " << receivePos.y << ", " << receivePos.z << ")"
                    << " with speed " << endl;*/
            EV_INFO << "Distance between two vehicles is " << -(receivePos.x-senderPos.x)<<endl;
            if ((receivePos.x-senderPos.x)<20){
                traciVehicle->changeLane(1, 1);
                EV_INFO << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
            }

        }

}
}

void TraCIDemo11p::onWSM(BaseFrame1609_4* frame)
{   EV_INFO << "fkkklllllllllllll";
    bool success;
    int senderID;
    EV_WARN << "LANE EKA  sssssssssssssssssssssssssssssssssssssssssssss";
    //EV_WARN << "LANE EKA  "<< traciVehicle->getLaneId()<<'\n';
    //TraCIDemo11pMessage* wsm = check_and_cast<TraCIDemo11pMessage*>(frame);

    //enderID = wsm->getSenderAddress() ;
    //EV_WARN << "WSM message received from node[" << senderID << "]";
    //mobility = TraCIMobilityAccess().get(getParentModule());
    ///traci = mobility->getCommandInterface();
    //traciVehicle = mobility->getVehicleCommandInterface();
    //EV_INFO <<senderID;


    DemoSafetyMessage* bsm = dynamic_cast<DemoSafetyMessage*>(frame);
    if (bsm == nullptr) {
        // The received frame is not a BSM message, so ignore it
        return;
    }

    // Extract the position and speed of the sending vehicle
    Coord senderPos = bsm->getSenderPos();
    std::cout<<senderPos;
    EV_WARN<<"gfgfvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv";
    //double senderSpeed = bsm->getSenderSpeed();
}

void TraCIDemo11p::handleSelfMsg(cMessage* msg)
{
    if (msg->isName("sendBSM")) {
        // Create and send a BSM message
        DemoSafetyMessage* bsm = new DemoSafetyMessage();
        populateWSM(bsm);
        sendDown(bsm);



        // Increment the sent count

        bsmSentCount++;
        EV_WARN << "Sent BSM message"<< bsmSentCount << endl;

        // Schedule the next BSM message to be sent after 1 second
        scheduleAt(simTime() + 20, msg->dup());
    }
}
/*void TraCIDemo11p::sendBSM()
{
    // Create a new BSM message


        DemoSafetyMessage* bsm = new DemoSafetyMessage();
        //TraCIDemo11pMessage* wsm = new TraCIDemo11pMessage();//aluth message ekak hadanwa


        bsm->setSenderPos(curPosition);
        bsm->setSenderSpeed(curSpeed);

        populateWSM(bsm);
        int senderId;
        sendDown(dynamic_cast<cMessage*>(bsm));
        EV_WARN << "Sent BSM messageoooooooooooooooooooooooooooooooooooooooooo" << endl;


        scheduleAt(simTime()+5, bsm->dup());





}*/


void TraCIDemo11p::handlePositionUpdate(cObject* obj) // wahana set eka gana idea ekak gnnawa unge lanes, node ids etc

{   double lat,lon,Position;
    int nodeID;
    std::string laneid;
    DemoBaseApplLayer::handlePositionUpdate(obj); /// wahane speed position eka gnnawa
    mobility = TraCIMobilityAccess().get(getParentModule());
    traci = mobility->getCommandInterface();
    traciVehicle = mobility->getVehicleCommandInterface();
    laneid = traciVehicle->getLaneId();

    nodeID = getParentModule()->getIndex();
    if (nodeID==1){
        traciVehicle->setSpeed(10);
    }
    if (nodeID==0){
          traciVehicle->setSpeed(5);
      }






}
