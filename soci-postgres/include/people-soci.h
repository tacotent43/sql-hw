#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>

struct PeoplePostgres {
    std::string DATABASE_URL;

    void initialize();
    void insert(soci::session session, std::string region, std::string employee, int amount);
    void drop();

    void solve();

    PeoplePostgres(const std::string &connectionString);
};