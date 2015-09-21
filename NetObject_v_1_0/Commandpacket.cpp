#include "Commandpacket.h"

KCloud::CommandPacket::CommandPacket(QObject *parent) : NetObject(parent){

}

void KCloud::CommandPacket::clear(){

}

void KCloud::CommandPacket::prepareForSend(){

}

void KCloud::CommandPacket::prepareForRecv(){

}

void KCloud::CommandPacket::send(const qint64 block){

}

void KCloud::CommandPacket::recv(){

}

void KCloud::CommandPacket::behaviorOnSend(const qint64 dim){

}

qint64 KCloud::CommandPacket::calculateNetworkSize(){

}
