#ifndef APPIMP_H
#define APPIMP_H

#include "zlog.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "topkg.h"
#include "recssl.h"
#include "ckafkaproducer.h"
#include "configpara.h"
#include "czmq.h"


class appManager;
class kafkaProduce;


class appManager
{
public:
    appManager();
    void init();
    static appManager* instance();

    void zlog_InitStep();
    void kafka_InitStep();
    int  pushKafkaData(char * recmsg);
    bool verifyHeaders(char * recMsg,RecHead &headers);

    char * rebuildTrajData(char * recMsgbody,RecHead &head);
    std::vector<std::string> split(const  std::string& s, const std::string& delim);
    ToPkg *pkg ;
    zsock_t *push;


private:
    static appManager* appMan;
    RecHead head;
    PackTraj Traj[20];
    PackAlarm alarm;
    CKafkaProducer kp;
    KafkaPara kafkaPar;
    LaneToFTPMap*  m_planetoftpmap;
};




#endif // APPIMP_H
