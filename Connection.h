#ifndef UAMQP_PHP_CONNECTION_H
#define UAMQP_PHP_CONNECTION_H
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/tlsio.h"
#include "azure_c_shared_utility/socketio.h"
#include "azure_uamqp_c/uamqp.h"
#include <phpcpp.h>

class Connection : public Php::Base
{
private:
    std::string host;
    int port;
    bool useTls;
    std::string keyName;
    std::string key;

    CONNECTION_HANDLE connection;
    XIO_HANDLE sasl_io;
    XIO_HANDLE socket_io;
    SASL_PLAIN_CONFIG sasl_plain_config;
    TLSIO_CONFIG tls_io_config;
    SOCKETIO_CONFIG socketio_config;
    const IO_INTERFACE_DESCRIPTION* tlsio_interface;
    SASLCLIENTIO_CONFIG sasl_io_config;
    SASL_MECHANISM_HANDLE sasl_mechanism_handle;
    XIO_HANDLE tls_io;

public:
    Connection() = default;
    virtual ~Connection() = default;

    void __construct(Php::Parameters &params);

    std::string getHost();
    CONNECTION_HANDLE getConnectionHandler();
    void doWork();
    void close();
};

#endif
