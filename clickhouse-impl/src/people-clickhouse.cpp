#include "people-clickhouse.h"

#include <memory>

void PeopleClickhouse::initialize() {
    try {
        this->client->Execute(
            "CREATE TABLE IF NOT EXISTS sales ("
            "    id UInt64,"
            "    region String,"
            "    employee String,"
            "    amount UInt64) "
            "ENGINE = MergeTree "
            "ORDER BY id"
        );
    } catch (const std::exception &e) {
        std::cerr << "Initialization error: " << e.what() << '\n';
    }
}

void PeopleClickhouse::insert(const std::vector<Person*> &people) {
    try {
        auto idColumn = std::make_shared<clickhouse::ColumnUInt64>();
        auto regionColumn = std::make_shared<clickhouse::ColumnString>();
        auto employeeColumn = std::make_shared<clickhouse::ColumnString>();
        auto amountColumn = std::make_shared<clickhouse::ColumnUInt64>();

        static const size_t psize = people.size();
        idColumn->Reserve(psize);
        regionColumn->Reserve(psize);
        employeeColumn->Reserve(psize);
        amountColumn->Reserve(psize);

        for (size_t i = 0; i < psize; ++i) {
            const Person* person = people[i];
            idColumn->Append(person->id);
            regionColumn->Append(person->region);
            employeeColumn->Append(person->employee);
            amountColumn->Append(person->amount);
        }

        clickhouse::Block block{};
        block.AppendColumn("id", idColumn);
        block.AppendColumn("region", regionColumn);
        block.AppendColumn("employee", employeeColumn);
        block.AppendColumn("amount", amountColumn);

        this->client->Insert("sales", block);
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}

void PeopleClickhouse::drop() {
    try {
        this->client->Execute("DROP TABLE IF EXISTS sales");
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}

void PeopleClickhouse::solve() {
    drop();
    initialize();

    this->client->Select(
        // !!! here
        "SELECT "
        "    region,"
        "    employee,"
        "    amount,"
        "    sum(amount) OVER (PARTITION BY region) AS total_region_sales,"
        "    rank() OVER (PARTITION BY region ORDER BY amount DESC) AS rank "
        "FROM sales "
        "ORDER BY region, amount DESC;",
        [](const clickhouse::Block &block) {
            for (size_t row_idx = 0; row_idx < block.GetRowCount(); ++row_idx) {
                auto region = block[0]->AsStrict<clickhouse::ColumnString>()->At(row_idx);
                auto employee = block[1]->AsStrict<clickhouse::ColumnString>()->At(row_idx);
                auto amount = block[2]->AsStrict<clickhouse::ColumnUInt64>()->At(row_idx);
                auto total_region_sales = block[3]->AsStrict<clickhouse::ColumnUInt64>()->At(row_idx);
                auto rank = block[4]->AsStrict<clickhouse::ColumnUInt64>()->At(row_idx);
                std::cout << '\t' << region << '\t' << employee << '\t' << amount << '\t' << total_region_sales << '\t' << rank << ' ';
            }
        }
    );
}

PeopleClickhouse::PeopleClickhouse(
    const std::string &host, 
    const std::string &user,
    const std::string &password,
    int port,
    bool ssl
) {
    try {
        if (ssl) {
            this->client = std::make_unique<clickhouse::Client>(
                clickhouse::ClientOptions()
                .SetHost(host)
                .SetUser(user)
                .SetPassword(password)
                .SetSSLOptions({})
                .SetPort(port)
                .SetDefaultDatabase("default")
            );
        } else {
            this->client = std::make_unique<clickhouse::Client>(
                clickhouse::ClientOptions()
                .SetHost(host)
                .SetUser(user)
                .SetPassword(password)
                .SetPort(port)
                .SetDefaultDatabase("default")
            );
        }
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}