#pragma once

#include <future>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <clickhouse/client.h>
#include <clickhouse/block.h>

struct BooksClickhouse {
    std::unique_ptr<clickhouse::Client> client;

    void initialize();
    void insertBook(int id, std::string title, std::string author, int year);
    void getBooks();
    void drop();
    void solve();

    BooksClickhouse(
        const std::string &host, 
        const std::string &user,
        const std::string &password,
        int port,
        bool ssl
    );
};