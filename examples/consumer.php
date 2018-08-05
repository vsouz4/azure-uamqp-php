<?php

use Azure\uAMQP\Consumer;
use Azure\uAMQP\Message;

include_once __DIR__ . '/parameters.php';

$session = include __DIR__ . '/session.php';

(new Consumer($session, MY_SUBSCRIPTION))
    ->consume(function (Message $message) {
        echo $message->getBody(), PHP_EOL;
    });
