#include <phpcpp.h>
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/tlsio.h"
#include "azure_c_shared_utility/socketio.h"
#include "azure_uamqp_c/uamqp.h"
#include "Connection.h"

void Connection::__construct(Php::Parameters &params)
{
    host    = params[0].stringValue();
    keyName = params[1].stringValue();
    key     = params[2].stringValue();

    sasl_plain_config = { keyName.c_str(), key.c_str(), NULL };
    tls_io_config = { host.c_str(), 5671 };

    platform_init();

    /* create SASL PLAIN handler */
    sasl_mechanism_handle = saslmechanism_create(saslplain_get_interface(), &sasl_plain_config);

    /* create the TLS IO */
    tlsio_interface = platform_get_default_tlsio();
    tls_io = xio_create(tlsio_interface, &tls_io_config);

    /* create the SASL client IO using the TLS IO */
    sasl_io_config.underlying_io = tls_io;
    sasl_io_config.sasl_mechanism = sasl_mechanism_handle;
    sasl_io = xio_create(saslclientio_get_interface_description(), &sasl_io_config);

    /* create the connection, session and link */
    connection = connection_create(sasl_io, host.c_str(), "some", NULL, NULL);
}

std::string Connection::getHost()
{
    return host;
}

CONNECTION_HANDLE Connection::getConnectionHandler()
{
    return connection;
}

void Connection::doWork()
{
    connection_dowork(connection);
}

void Connection::close()
{
    connection_destroy(connection);
    xio_destroy(sasl_io);
    xio_destroy(tls_io);
    saslmechanism_destroy(sasl_mechanism_handle);
    platform_deinit();
}
