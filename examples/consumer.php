<?php

use Azure\uAMQP\Message;

include_once __DIR__ . '/parameters.php';

$connection = include __DIR__ . '/connection.php';

$connection->consume(
    MY_SUBSCRIPTION,
    function (Message $message) {
        echo $message->getBody(), PHP_EOL;
        echo $message->getApplicationProperty('some-application-property', 'S'), PHP_EOL;
        echo $message->getMessageAnnotation('x-opt-scheduled-enqueue-time', 'T'), PHP_EOL;
/*
        echo PHP_EOL;
        echo PHP_EOL;
        echo PHP_EOL;
        echo $message->getBody(), PHP_EOL;
        echo $message->getApplicationProperty('some-application-property', 'S'), PHP_EOL;
        echo $message->getMessageAnnotation('x-opt-scheduled-enqueue-time', 'T'), PHP_EOL;
 */
    }
);
