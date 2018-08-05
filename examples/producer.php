<?php

use Azure\uAMQP\Producer;
use Azure\uAMQP\Message;

include_once __DIR__ . '/parameters.php';

$session = include __DIR__ . '/session.php';

(new Producer($session, MY_TOPIC))->publish(new Message(MY_MESSAGE));
