<?php

use Azure\uAMQP\Connection;

include_once __DIR__ . '/parameters.php';

$connection = new Connection(HOST, PORT, USE_TLS, KEY_NAME, ACCESS_KEY, true);
return $connection;
