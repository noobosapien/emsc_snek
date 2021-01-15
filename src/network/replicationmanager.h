#ifndef REPLICATIONMANAGER_H
#define REPLICATIONMANAGER_H

#include "inoutstreams.h"

class ReplicationManager{

public:
    ReplicationManager();
    ~ReplicationManager();

    void read(InputStream& inStream);

};

#endif