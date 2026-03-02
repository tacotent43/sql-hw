#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <memory>

#include <clickhouse/client.h>
#include <clickhouse/block.h>

struct Person {
    uint64_t id;
    std::string region;
    std::string employee;
    uint64_t amount;
};

struct PeopleClickhouse {
    std::unique_ptr<clickhouse::Client> client;

    void initialize();
    void insert(int id, std::string region, std::string employee, int amount);
    void drop();

    void solve();

    PeopleClickhouse(
        const std::string &host, 
        const std::string &user,
        const std::string &password,
        int port,
        bool ssl
    );
};