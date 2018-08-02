<?php

$conn = new AMQPConnection(
    'something-here.servicebus.windows.net',
    'SomeAccessKey',
    '12encse213t98vdr/1USYZ+3mnQETa2Pp027sjaRY='
);
$session = new AMQPSession($conn);
$consumer = new AMQPConsumer($session, 'my.topic/subscriptions/my.subscription');
$consumer->consume(function (AMQPMessage $message) {
    echo $message->getBody(), PHP_EOL;
});
