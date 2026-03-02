#pragma once

#include <future>
#include <iostream>
#include <vector>
#include <string>

#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>

struct BooksPostgres {
    std::string DATABASE_URL;

    void initialize();
    void insertBook(std::string title, std::string author, int year);
    void getBooks();
    void drop();
    void solve();

    BooksPostgres(const std::string &connectionString);
};