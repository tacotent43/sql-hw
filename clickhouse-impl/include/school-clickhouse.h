#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <clickhouse/client.h>
#include <clickhouse/block.h>

struct SchoolClickhouse {
    std::unique_ptr<clickhouse::Client> client;

    void initialize();
    void drop();
    void insert();

    void solve();
    SchoolClickhouse(
        const std::string &host, 
        const std::string &user,
        const std::string &password,
        int port,
        bool ssl
    );
};