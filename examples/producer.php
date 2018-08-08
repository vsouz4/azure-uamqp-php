<?php

use Azure\uAMQP\Producer;
use Azure\uAMQP\Message;

include_once __DIR__ . '/parameters.php';

$session = include __DIR__ . '/session.php';

$date = new DateTime("now", new \DateTimeZone("UTC"));
$date->add(new DateInterval('PT1H'));

$message = new Message(MY_MESSAGE);
$message->setApplicationProperty('some-application-property', 'some value');
$message->setApplicationProperty('operation', 'com.microsoft:schedule-message');
$message->setMessageAnnotation('x-opt-scheduled-enqueue-time', $date->format(DateTime::RFC850));
(new Producer($session, MY_TOPIC))->publish($message);
