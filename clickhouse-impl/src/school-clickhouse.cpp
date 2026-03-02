#include "school-clickhouse.h"

// correct sqls?

void SchoolClickhouse::initialize() {
    try {
        this->client->Execute(
            "CREATE TABLE IF NOT EXISTS students ("
            "id UInt32,"
            "name String"
            ") ENGINE = MergeTree() "
            "ORDER BY id;"
        );
    } catch (const std::exception &e) {
        std::cerr << "Failed to create students table: \n" << e.what() << '\n';
    }
    try {
        this->client->Execute(
            "CREATE TABLE IF NOT EXISTS courses ("
            "id UInt32,"
            "title String"
            ") ENGINE = MergeTree() "
            "ORDER BY id;"
        );
    } catch (const std::exception &e) {
        std::cerr << "Failed to create courses table: \n" << e.what() << '\n';
    }
    try {
        this->client->Execute(
            "CREATE TABLE IF NOT EXISTS student_courses ("
            "student_id UInt32,"
            "course_id UInt32"
            ") ENGINE = MergeTree() "
            "ORDER BY (student_id, course_id);"
        );
    } catch (const std::exception &e) {
        std::cerr << "Failed to create student_courses table: \n" << e.what() << '\n';
    }
}

void SchoolClickhouse::drop() {
    try {
        this->client->Execute("DROP TABLE IF EXISTS students;");
        this->client->Execute("DROP TABLE IF EXISTS courses;");
        this->client->Execute("DROP TABLE IF EXISTS student_courses;");
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}

void SchoolClickhouse::insert() {
    try {
        this->client->Execute(
            "INSERT INTO students VALUES "
            "(1,'Alice'),"
            "(2,'Bob'),"
            "(3,'Charlie');"
        );

        this->client->Execute(
            "INSERT INTO courses VALUES "
            "(1,'Math'),"
            "(2,'Physics'),"
            "(3,'Chemistry');"
        );

        this->client->Execute(
            "INSERT INTO student_courses VALUES "
            "(1,1),(1,2);"
        );

        this->client->Execute(
            "INSERT INTO student_courses VALUES "
            "(2,2),(2,3);"
        );

        this->client->Execute(
            "INSERT INTO student_courses VALUES "
            "(3,1),(3,2),(3,3);"
        );
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}

void SchoolClickhouse::solve() {
    try {
        this->client->Select(
            "SELECT "
            "s.name, "
            "arrayStringConcat(arraySort(groupArray(c.title)), ', ') AS courses "
            "FROM students s "
            "JOIN student_courses sc ON s.id = sc.student_id "
            "JOIN courses c ON sc.course_id = c.id "
            "GROUP BY s.name "
            "ORDER BY s.name;",
            [](const clickhouse::Block &block){
                std::cout << "Query 4.1: \n";
                for (size_t row_idx = 0; row_idx < block.GetRowCount(); ++row_idx) {
                    auto name = block[0]->AsStrict<clickhouse::ColumnString>()->At(row_idx);
                    auto courses = block[1]->AsStrict<clickhouse::ColumnString>()->At(row_idx);
                    std::cout << "Name: " << name << ", courses: " << courses << '\n';
                }
            }
        );

        this->client->Select(
            "SELECT s.name "
            "FROM students s "
            "JOIN student_courses sc ON s.id = sc.student_id "
            "JOIN courses c ON sc.course_id = c.id "
            "WHERE c.title = 'Physics' "
            "ORDER BY s.name;",
            [](const clickhouse::Block &block) {
                std::cout << "Query 4.2: \n";
                for (size_t row_idx = 0; row_idx < block.GetRowCount(); ++row_idx) {
                    auto name = block[0]->AsStrict<clickhouse::ColumnString>()->At(row_idx);
                    std::cout << "Name: " << name << '\n';
                }
            }
        );
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}

SchoolClickhouse::SchoolClickhouse(
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