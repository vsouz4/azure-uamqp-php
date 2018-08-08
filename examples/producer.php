<?php

use Azure\uAMQP\Producer;
use Azure\uAMQP\Message;

include_once __DIR__ . '/parameters.php';

$session = include __DIR__ . '/session.php';

$message = new Message(MY_MESSAGE);
$message->setApplicationProperty('some-application-property', 'some value');
(new Producer($session, MY_TOPIC))->publish($message);
