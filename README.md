# Azure uAMQP PHP

PHP binding for Azure uAMQP C (AMQP 1.0) - currently being used with Azure Service Bus (Topics and Subscriptions) but should work for everything else that works with AMQP 1.0.

This is a wrapper for the C library provided by Azure (Azure uAMQP C), builded as an extension in PHP, providing PHP classes so that PHP code can work with AMQP 1.0.

Currently being used with PHP 7.2.

# Pre-requisites

In order to compile this extension, you need three dependencies to be installed (Azure C Shared Utility, Azure uAMQP C and PHP-CPP). You can install them this way if you're using Ubuntu (tested against Ubuntu docker image ubuntu:18.04).

```
$ apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y \
        # php-cpp
        wget \
        make \
        g++ \
        php7.2-dev \
        # azure-uamqp-c
        git \
        cmake \
        build-essential \
        curl \
        libcurl4-openssl-dev \
        libssl-dev uuid-dev

# Install Azure C Shared Utility
$ git clone --recursive https://github.com/Azure/azure-c-shared-utility.git \
        && cd azure-c-shared-utility \
        && mkdir cmake \
        && cd cmake \
        && cmake -Duse_installed_dependencies=ON ../ \
        && cmake --build . --target install

# Install Azure UAMQP C
$ git clone --recursive https://github.com/Azure/azure-uamqp-c.git \
        && cd azure-uamqp-c \
        && mkdir cmake \
        && cd cmake \
        && cmake -Duse_installed=ON ../ \
        && cmake --build . --target install

# Install PHP-CPP
$ cd /tmp \
        && wget https://github.com/CopernicaMarketingSoftware/PHP-CPP/archive/v2.1.0.tar.gz \
        && tar xzf v2.1.0.tar.gz \
        && cd PHP-CPP-2.1.0 \
        && make \
        && make install
```

# Compiling and installing the extension

```
$ make
$ make install
```
