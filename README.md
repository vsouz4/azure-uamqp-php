# Azure uAMQP PHP

PHP binding for Azure uAMQP C (AMQP 1.0) - currently being used with Azure Service Bus (Topics and Subscriptions) but should work for everything else that works with AMQP 1.0.

This is a wrapper for the C library provided by Azure (Azure uAMQP C), builded as an extension in PHP, providing PHP classes so that PHP code can work with AMQP 1.0.

Currently being used with PHP 7.2.

# Building extension

A docker image is provided with everything that's needed to compile this extension. To build this docker image, you can run:

```
$ make image
```

# Compiling the extension

Run:

```
$ make
```

This will generate both .ini and .so files.

# Installing the extension

Just run:

```
$ make install
```

This will install the .so and .ini files in the correct places (you can also copy those two files if something goes wrong ;D).
