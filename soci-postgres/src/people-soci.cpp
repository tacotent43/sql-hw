#include "people-soci.h"

void PeoplePostgres::initialize() {
    try {
        soci::session sql(soci::postgresql, this->DATABASE_URL);
        sql << "CREATE TABLE IF NOT EXISTS sales ("
            "id SERIAL PRIMARY KEY,"
            "region TEXT,"
            "employee TEXT,"
            "amount INTEGER)";
    } catch (const std::exception &e) {
        std::cerr << "Initialization error: " << e.what() << '\n';
    }
}

void PeoplePostgres::insert(soci::session session, std::string region, std::string employee, int amount) {
    try {
        soci::session sql(soci::postgresql, this->DATABASE_URL);
        sql << "INSERT INTO sales(region, employee, amount) VALUES(:region,:employee,:amount)",
                                                                soci::use(region, "region"),
                                                                soci::use(employee, "employee"),
                                                                soci::use(amount, "amount");
    } catch (const std::exception &e) {
        std::cerr << "Connection error: " << e.what() << '\n';
    }
}

void PeoplePostgres::drop() {
    try {
        soci::session sql(soci::postgresql, this->DATABASE_URL);
        sql << "DROP TABLE IF EXISTS sales";
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}

void PeoplePostgres::solve() {
    drop();
    initialize();

    soci::session sql(soci::postgresql, this->DATABASE_URL);
    
    soci::rowset<soci::row> table = (sql.prepare << "SELECT region, employee, amount, "
            "SUM(amount) OVER (PARTITION BY region) AS total_region_sales, "
            "RANK() OVER (PARTITION BY region ORDER BY amount DESC) AS rank "
            "FROM sales "
            "ORDER BY region, amount DESC");
    
    for (auto const &row : table) {
        std::cout << 
        row.get<std::string>(0) << " | " <<
        row.get<std::string>(1) << " | " <<
        row.get<int>(2) << " | " <<
        row.get<int>(3) << " | " <<
        row.get<long long>(4) << '\n';
    }
    
}

PeoplePostgres::PeoplePostgres(const std::string &connectionString) {
    this->DATABASE_URL = connectionString;
}