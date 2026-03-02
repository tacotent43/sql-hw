#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>

struct SchoolPostgres {
    std::string DATABASE_URL;

    void initialize();
    void drop();
    void insert();

    void solve();
    SchoolPostgres(const std::string &connectionString);
};