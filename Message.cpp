#include "Message.h"

static Php::Value get_value_from_map(AMQP_VALUE map, const char* key, const char type)
{
    AMQP_VALUE amqp_value_value = amqpvalue_get_map_value(map, amqpvalue_create_symbol(key));
    Php::Value result;

    const char* valueString = NULL;
    int64_t valueTimestamp;
    int32_t valueInt;
    double valueDouble;

    switch (type) {
        case 'S':
            amqpvalue_get_string(amqp_value_value, &valueString);
            result = valueString;
            break;
        case 'T':
            amqpvalue_get_timestamp(amqp_value_value, &valueTimestamp);
            result = valueTimestamp;
            break;
        case 'I':
            amqpvalue_get_int(amqp_value_value, &valueInt);
            result = valueInt;
            break;
        case 'D':
            amqpvalue_get_double(amqp_value_value, &valueDouble);
            result = valueDouble;
            break;
    }

    return result;
}
static void add_map_item(AMQP_VALUE map, const char* name, AMQP_VALUE amqp_value_value)
{
    AMQP_VALUE amqp_value_name = amqpvalue_create_symbol(name);
    amqpvalue_set_map_value(map, amqp_value_name, amqp_value_value);
    amqpvalue_destroy(amqp_value_value);
    amqpvalue_destroy(amqp_value_name);
}
static void add_map_string(AMQP_VALUE map, const char* name, const char* value)
{
    AMQP_VALUE amqp_value_value = amqpvalue_create_string(value);
    add_map_item(map, name, amqp_value_value);
}
static void add_map_timestamp(AMQP_VALUE map, const char* name, int64_t value)
{
    AMQP_VALUE amqp_value_value = amqpvalue_create_timestamp(value);
    add_map_item(map, name, amqp_value_value);
}
static void add_map_int(AMQP_VALUE map, const char* name, int32_t value)
{
    AMQP_VALUE amqp_value_value = amqpvalue_create_int(value);
    add_map_item(map, name, amqp_value_value);
}
static void add_map_double(AMQP_VALUE map, const char* name, double value)
{
    AMQP_VALUE amqp_value_value = amqpvalue_create_double(value);
    add_map_item(map, name, amqp_value_value);
}
static void add_map_value(AMQP_VALUE map, const char* key, const char type, Php::Value value)
{
    switch (type) {
        case 'I':
            add_map_int(map, key, static_cast<int32_t>(value));
            break;
        case 'S':
            add_map_string(map, key, value.stringValue().c_str());
            break;
        case 'T':
            add_map_timestamp(map, key, static_cast<int64_t>(value));
            break;
        case 'D':
            add_map_double(map, key, static_cast<double>(value));
            break;
    }
}

static void add_amqp_message_annotation(MESSAGE_HANDLE message, AMQP_VALUE msg_annotations_map)
{
    AMQP_VALUE msg_annotations;
    msg_annotations = amqpvalue_create_message_annotations(msg_annotations_map);
    message_set_message_annotations(message, (annotations)msg_annotations);
    annotations_destroy(msg_annotations);
}

Message::Message()
{
    message = message_create();

    application_properties = amqpvalue_create_map();
    annotations_map = amqpvalue_create_map();

    message_set_application_properties(message, application_properties);
    add_amqp_message_annotation(message, annotations_map);
}

void Message::__construct(Php::Parameters &params)
{
    setBody(params[0].stringValue());
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

    unsigned char bodyCharArray[body.size()];
    for (unsigned int i = 0; i < body.size(); i++) {
        bodyCharArray[i] = body.at(i);
    }
    binary_data.bytes = bodyCharArray;
    binary_data.length = sizeof(bodyCharArray);
    message_add_body_amqp_data(message, binary_data);
}

static AMQP_VALUE application_properties_map;

Php::Value Message::getApplicationProperty(Php::Parameters &params)
{
    std::string key = params[0].stringValue();

    if (application_properties_map == NULL) {
        message_get_application_properties(message, &application_properties);
        application_properties_map = amqpvalue_get_inplace_described_value(application_properties);
    }

    return get_value_from_map(application_properties_map, key.c_str(), params[1].stringValue().at(0));
}

Php::Value Message::getApplicationProperties()
{
    if (applicationPropertiesMap != NULL) {
        return applicationPropertiesMap;
    }

    uint32_t property_count = 0;

    if (application_properties_map == NULL) {
        message_get_application_properties(message, &application_properties);
        application_properties_map = amqpvalue_get_inplace_described_value(application_properties);
    }

    amqpvalue_get_map_pair_count(application_properties_map, &property_count);
    for (uint32_t i = 0; i < property_count; i++) {
        AMQP_VALUE map_key_name = NULL;
        AMQP_VALUE map_key_value = NULL;
        const char *key_name;
        const char* valueString = NULL;
        int64_t valueTimestamp;
        int32_t valueInt;
        double valueDouble;

        amqpvalue_get_map_key_value_pair(application_properties_map, i, &map_key_name, &map_key_value);
        amqpvalue_get_symbol(map_key_name, &key_name);
        switch (amqpvalue_get_type(map_key_value)) {
            default:
                LogError("Unknown AMQP type");
                break;
            case AMQP_TYPE_INT:
                amqpvalue_get_int(map_key_value, &valueInt);
                applicationPropertiesMap[key_name] = valueInt;
                break;
            case AMQP_TYPE_DOUBLE:
                amqpvalue_get_double(map_key_value, &valueDouble);
                applicationPropertiesMap[key_name] = valueDouble;
                break;
            case AMQP_TYPE_TIMESTAMP:
                amqpvalue_get_timestamp(map_key_value, &valueTimestamp);
                applicationPropertiesMap[key_name] = valueTimestamp;
                break;
            case AMQP_TYPE_STRING:
                amqpvalue_get_string(map_key_value, &valueString);
                applicationPropertiesMap[key_name] = valueString;
                break;
        }

        amqpvalue_destroy(map_key_name);
        amqpvalue_destroy(map_key_value);
    }

    return applicationPropertiesMap;
}

Php::Value Message::getMessageAnnotation(Php::Parameters &params)
{
    std::string key = params[0].stringValue();

    // @todo add check for not doing this everytime
    message_get_message_annotations(message, &annotations_map);

    return get_value_from_map(annotations_map, key.c_str(), params[1].stringValue().at(0));
}

void Message::setApplicationProperty(Php::Parameters &params)
{
    add_map_value(application_properties, params[0].stringValue().c_str(), params[1].stringValue().at(0), params[2]);
}

void Message::setMessageAnnotation(Php::Parameters &params)
{
    add_map_value(annotations_map, params[0].stringValue().c_str(), params[1].stringValue().at(0), params[2]);
}

MESSAGE_HANDLE Message::getMessageHandler()
{
    return message;
}

void Message::setMessageHandler(MESSAGE_HANDLE message)
{
    this->message = message;
}

