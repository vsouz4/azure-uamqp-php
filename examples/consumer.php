<?php

use Azure\uAMQP\Consumer;
use Azure\uAMQP\Message;

include_once __DIR__ . '/parameters.php';

$session = include __DIR__ . '/session.php';

(new Consumer($session, MY_SUBSCRIPTION))
    ->consume(function (Message $message) {
        echo $message->getBody(), PHP_EOL;
        echo $message->getApplicationProperty('some-application-property'), PHP_EOL;
        echo $message->getMessageAnnotation('x-opt-scheduled-enqueue-time'), PHP_EOL;
    });
