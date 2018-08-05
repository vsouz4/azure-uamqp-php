<?php

use Azure\uAMQP\Connection;
use Azure\uAMQP\Session;

include_once __DIR__ . '/parameters.php';

$connection = new Connection(HOST, PORT, USE_TLS, KEY_NAME, ACCESS_KEY);

return new Session($connection);
