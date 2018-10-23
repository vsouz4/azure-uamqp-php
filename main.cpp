#include <phpcpp.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Connection.h"
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
            Php::ByVal("key", Php::Type::String),
            Php::ByVal("debug", Php::Type::Bool, false)
        });
        connection.method<&Connection::publish>("publish", {
            Php::ByVal("resourceName", Php::Type::String),
            Php::ByVal("message", "Azure\\uAMQP\\Message")
        });
        connection.method<&Connection::setCallback>("setCallback", {
            Php::ByVal("resourceName", Php::Type::String),
            Php::ByVal("callback", Php::Type::Callable),
            Php::ByVal("loopFn", Php::Type::Callable)
        });
        connection.method<&Connection::consume>("consume");
        connection.method<&Connection::close>("close");

        Php::Class<Message> message("Azure\\uAMQP\\Message");
        message.method<&Message::__construct>("__construct", {
            Php::ByVal("body", Php::Type::String)
        });
        message.method<&Message::getBody>("getBody");
        message.method<&Message::getApplicationProperty>("getApplicationProperty", {
            Php::ByVal("key", Php::Type::String),
            Php::ByVal("type", Php::Type::String)
        });
        message.method<&Message::getApplicationProperties>("getApplicationProperties");
        message.method<&Message::getMessageAnnotation>("getMessageAnnotation", {
            Php::ByVal("key", Php::Type::String),
            Php::ByVal("type", Php::Type::String)
        });
        message.method<&Message::setApplicationProperty>("setApplicationProperty", {
            Php::ByVal("key", Php::Type::String),
            Php::ByVal("type", Php::Type::String),
            Php::ByVal("value", Php::Type::String)
        });
        message.method<&Message::setMessageAnnotation>("setMessageAnnotation", {
            Php::ByVal("key", Php::Type::String),
            Php::ByVal("type", Php::Type::String),
            Php::ByVal("value", Php::Type::String)
        });

        extension.add(std::move(connection));
        extension.add(std::move(message));

        return extension;
    }
}
