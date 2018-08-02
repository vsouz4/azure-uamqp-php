#include <phpcpp.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Connection.h"
#include "Session.h"
#include "Consumer.h"
#include "Producer.h"
#include "Message.h"

extern "C" {

    PHPCPP_EXPORT void *get_module() 
    {
        static Php::Extension extension("uamqpphpbinding", "0.1");
        
        Php::Class<Connection> connection("AMQPConnection");
        connection.method<&Connection::__construct>("__construct", {
            Php::ByVal("host", Php::Type::String),
            Php::ByVal("keyName", Php::Type::String),
            Php::ByVal("key", Php::Type::String)
        });

        Php::Class<Session> session("AMQPSession");
        session.method<&Session::__construct>("__construct", {
            Php::ByVal("connection", "AMQPConnection")
        });

        Php::Class<Producer> producer("AMQPProducer");
        producer.method<&Producer::__construct>("__construct", {
            Php::ByVal("session", "AMQPSession"),
            Php::ByVal("resourceName", Php::Type::String)
        });
        producer.method<&Producer::publish>("publish", {
            Php::ByVal("message", "AMQPMessage")
        });

        Php::Class<Consumer> consumer("AMQPConsumer");
        consumer.method<&Consumer::__construct>("__construct", {
            Php::ByVal("session", "AMQPSession"),
            Php::ByVal("resourceName", Php::Type::String)
        });
        consumer.method<&Consumer::consume>("consume", {
            Php::ByVal("callback", Php::Type::Callable)
        });

        Php::Class<Message> message("AMQPMessage");
        message.method<&Message::__construct>("__construct", {
            Php::ByVal("body", Php::Type::String)
        });
        message.method<&Message::getBody>("getBody");

        extension.add(std::move(connection));
        extension.add(std::move(session));
        extension.add(std::move(producer));
        extension.add(std::move(consumer));
        extension.add(std::move(message));

        return extension;
    }
}
