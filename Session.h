#ifndef UAMQP_PHP_SESSION_H
#define UAMQP_PHP_SESSION_H
#include <phpcpp.h>
#include "Connection.h"

class Connection;

class Session
{
private:
    SESSION_HANDLE session;
    Connection *connection;

public:
    Session(Connection *connection);
    virtual ~Session() = default;

    SESSION_HANDLE getSessionHandler();
    Connection* getConnection();
    void close();
};

#endif
