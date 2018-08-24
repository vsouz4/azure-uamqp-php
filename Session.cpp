#include "Session.h"
#include "azure_uamqp_c/uamqp.h"
#include "Connection.h"

Session::Session(Connection *connection)
{
    this->connection = connection;

    session = session_create(connection->getConnectionHandler(), NULL, NULL);
    session_set_incoming_window(session, 2147483647);
    session_set_outgoing_window(session, 65536);
}

SESSION_HANDLE Session::getSessionHandler()
{
    return session;
}

Connection* Session::getConnection()
{
    return connection;
}

void Session::close()
{
    session_destroy(session);
}
