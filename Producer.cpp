#include "Producer.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/tlsio.h"
#include "azure_c_shared_utility/socketio.h"
#include "azure_uamqp_c/uamqp.h"
#include "Session.h"
#include "Message.h"

void Producer::__construct(Php::Parameters &params)
{
    session = (Session*) params[0].implementation();
    resourceName = params[1].stringValue();

    source = messaging_create_source("ingress");
    target = messaging_create_target(("amqps://" + session->getConnection()->getHost() + "/" + resourceName).c_str());
    link = link_create(session->getSessionHandler(), "sender-link", role_sender, source, target);
    link_set_snd_settle_mode(link, sender_settle_mode_unsettled);
    (void)link_set_max_message_size(link, 65536);

    amqpvalue_destroy(source);
    amqpvalue_destroy(target);
}

static unsigned int sent_messages = 0;

static void on_message_send_complete(void* context, MESSAGE_SEND_RESULT send_result, AMQP_VALUE delivery_state)
{
    (void)send_result;
    (void)context;
    (void)delivery_state;

    sent_messages++;
}

void Producer::publish(Php::Parameters &params)
{
    Message *msg = (Message*) params[0].implementation();

    MESSAGE_HANDLE message = msg->getMessageHandler();

    /* create a message sender */
    message_sender = messagesender_create(link, NULL, NULL);
    messagesender_open(message_sender);

    (void)messagesender_send_async(message_sender, message, on_message_send_complete, message, 10000);

    while (true)
    {
        session->getConnection()->doWork();

        if (sent_messages == 1)
        {
            break;
        }
    }

    message_destroy(message);
    messagesender_destroy(message_sender);

    link_destroy(link);
    session->close();
    session->getConnection()->close();
}
