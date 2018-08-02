FROM ubuntu:18.04

RUN apt-get update \
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
RUN git clone --recursive https://github.com/Azure/azure-c-shared-utility.git \
        && cd azure-c-shared-utility \
        && mkdir cmake \
        && cd cmake \
        && cmake -Duse_installed_dependencies=ON ../ \
        && cmake --build . --target install

# Install Azure UAMQP C
RUN git clone --recursive https://github.com/Azure/azure-uamqp-c.git \
        && cd azure-uamqp-c \
        && mkdir cmake \
        && cd cmake \
        && cmake -Duse_installed=ON ../ \
        && cmake --build . --target install

# Install PHP-CPP
RUN cd /tmp \
        && wget https://github.com/CopernicaMarketingSoftware/PHP-CPP/archive/v2.1.0.tar.gz \
        && tar xzf v2.1.0.tar.gz \
        && cd PHP-CPP-2.1.0 \
        && make \
        && make install
