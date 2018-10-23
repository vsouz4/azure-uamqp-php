<?php

use Azure\uAMQP\Message;

include_once __DIR__ . '/parameters.php';

$connection = include __DIR__ . '/connection.php';
try {
    $nOfMessages = 0;
    $connection->setCallback(
        MY_SUBSCRIPTION,
        function (Message $message) use (&$nOfMessages) {
            $nOfMessages++;
            echo $message->getBody(), PHP_EOL;
            echo $message->getApplicationProperty('some-application-property', 'S'), PHP_EOL;
            echo $message->getMessageAnnotation('x-opt-scheduled-enqueue-time', 'T'), PHP_EOL;

            var_dump($message->getApplicationProperties());
        },
        function () use (&$nOfMessages, $connection) {
            while ($nOfMessages < 2) {
                $connection->consume();
                time_nanosleep(0, 100000000);
            }
        }
    );
    $connection->close();
} catch (\Exception $e) {
    echo $e->getMessage();
}
