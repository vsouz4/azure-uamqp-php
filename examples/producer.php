<?php

$conn = new AMQPConnection(
    'something-here.servicebus.windows.net',
    'SomeAccessKey',
    '12encse213t98vdr/1USYZ+3mnQETa2Pp027sjaRY='
);
$session = new AMQPSession($conn);
$producer = new AMQPProducer($session, 'my.topic');
$message = new AMQPMessage('my message');
$producer->publish($message);
