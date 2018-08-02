#ifndef UAMQP_PHP_SESSION_H
#define UAMQP_PHP_SESSION_H
#include <phpcpp.h>
#include "azure_uamqp_c/uamqp.h"
#include "Connection.h"

class Session : public Php::Base
{
private:
    Connection *connection;
    SESSION_HANDLE session;

public:
    Session() = default;
    virtual ~Session() = default;

    void __construct(Php::Parameters &params);

    Connection* getConnection();
    SESSION_HANDLE getSessionHandler();
    void close();
};

#endif
