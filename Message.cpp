#include "Message.h"

static void add_map_item(AMQP_VALUE map, const char* name, const char* value)
{
    AMQP_VALUE amqp_value_name;
    amqp_value_name = amqpvalue_create_symbol(name);
    AMQP_VALUE amqp_value_value = NULL;
    amqp_value_value = amqpvalue_create_string(value);
    amqpvalue_set_map_value(map, amqp_value_name, amqp_value_value);
    amqpvalue_destroy(amqp_value_value);
    amqpvalue_destroy(amqp_value_name);
}
static void add_amqp_message_annotation(MESSAGE_HANDLE message, AMQP_VALUE msg_annotations_map)
{
    AMQP_VALUE msg_annotations;
    msg_annotations = amqpvalue_create_message_annotations(msg_annotations_map);
    message_set_message_annotations(message, (annotations)msg_annotations);
    annotations_destroy(msg_annotations);
}

void Message::__construct(Php::Parameters &params)
{
    body = params[0].stringValue();

    message = message_create();

    unsigned char bodyCharArray[body.size()];
    for (unsigned int i = 0; i < body.size(); i++) {
        bodyCharArray[i] = body.at(i);
    }
    binary_data.bytes = bodyCharArray;
    binary_data.length = sizeof(bodyCharArray);
    message_add_body_amqp_data(message, binary_data);

    application_properties = amqpvalue_create_map();
    annotations_map = amqpvalue_create_map();

    message_set_application_properties(message, application_properties);
    add_amqp_message_annotation(message, annotations_map);
}

Php::Value Message::getBody()
{
    if (body.empty()) {
        message_get_body_amqp_data_in_place(message, 0, &binary_data);
        for (size_t i = 0; i < binary_data.length; ++i) {
            body += binary_data.bytes[i];
        }
    }

    return body;
}

void Message::setBody(std::string body)
{
    this->body = body;
}

Php::Value Message::getApplicationProperty(Php::Parameters &params)
{
    std::string key = params[0].stringValue();

    if (application_properties_map == NULL) {
        message_get_application_properties(message, &application_properties);
        application_properties_map = amqpvalue_get_inplace_described_value(application_properties);
    }

    const char* value = NULL;

    amqpvalue_get_string(amqpvalue_get_map_value(application_properties_map, amqpvalue_create_symbol(key.c_str())), &value);

    return value;
}

void Message::setApplicationProperty(Php::Parameters &params)
{
    std::string key = params[0].stringValue();
    std::string value = params[1].stringValue();

    add_map_item(application_properties, key.c_str(), value.c_str());
}

void Message::setMessageAnnotation(Php::Parameters &params)
{
    std::string key = params[0].stringValue();
    std::string value = params[1].stringValue();

    add_map_item(annotations_map, key.c_str(), value.c_str());
}

MESSAGE_HANDLE Message::getMessageHandler()
{
    return message;
}

void Message::setMessageHandler(MESSAGE_HANDLE message)
{
    this->message = message;
}

