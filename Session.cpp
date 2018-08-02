#include "Session.h"
#include "azure_uamqp_c/uamqp.h"
#include "Connection.h"

void Session::__construct(Php::Parameters &params)
{
    connection = (Connection*) params[0].implementation();

    session = session_create(connection->getConnectionHandler(), NULL, NULL);
    session_set_incoming_window(session, 2147483647);
    session_set_outgoing_window(session, 65536);

}

Connection* Session::getConnection()
{
    return connection;
}

SESSION_HANDLE Session::getSessionHandler()
{
    return session;
}

void Session::close()
{
    session_destroy(session);
}
