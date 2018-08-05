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
        
        Php::Class<Connection> connection("Azure\\uAMQP\\Connection");
        connection.method<&Connection::__construct>("__construct", {
            Php::ByVal("host", Php::Type::String),
            Php::ByVal("port", Php::Type::Numeric),
            Php::ByVal("useTls", Php::Type::Bool),
            Php::ByVal("keyName", Php::Type::String),
            Php::ByVal("key", Php::Type::String)
        });

        Php::Class<Session> session("Azure\\uAMQP\\Session");
        session.method<&Session::__construct>("__construct", {
            Php::ByVal("connection", "Azure\\uAMQP\\Connection")
        });

        Php::Class<Producer> producer("Azure\\uAMQP\\Producer");
        producer.method<&Producer::__construct>("__construct", {
            Php::ByVal("session", "Azure\\uAMQP\\Session"),
            Php::ByVal("resourceName", Php::Type::String)
        });
        producer.method<&Producer::publish>("publish", {
            Php::ByVal("message", "Azure\\uAMQP\\Message")
        });

        Php::Class<Consumer> consumer("Azure\\uAMQP\\Consumer");
        consumer.method<&Consumer::__construct>("__construct", {
            Php::ByVal("session", "Azure\\uAMQP\\Session"),
            Php::ByVal("resourceName", Php::Type::String)
        });
        consumer.method<&Consumer::consume>("consume", {
            Php::ByVal("callback", Php::Type::Callable)
        });

        Php::Class<Message> message("Azure\\uAMQP\\Message");
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
