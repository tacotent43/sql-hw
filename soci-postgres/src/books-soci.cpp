#include "books-soci.h"

void BooksPostgres::initialize() {
    try {
        soci::session sql(soci::postgresql, this->DATABASE_URL);
        sql << "CREATE TABLE IF NOT EXISTS books ("
            "id SERIAL PRIMARY KEY,"
            "title TEXT,"
            "author TEXT,"
            "year INTEGER)";
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}

void BooksPostgres::insertBook(std::string title, std::string author, int year) {
    soci::session sql(soci::postgresql, this->DATABASE_URL);
    sql << "INSERT INTO books(title, author, year) VALUES(:title,:author,:year)", 
                                                    soci::use(title, "title"), 
                                                    soci::use(author, "author"), 
                                                    soci::use(year, "year");
}

void BooksPostgres::getBooks() {
    try {
        soci::session sql(soci::postgresql, this->DATABASE_URL);

        soci::rowset<soci::row> rs = (sql.prepare << "SELECT title FROM books WHERE year > 2000");

        for (auto const &r : rs) {
            std::cout << r.get<std::string>(0) << '\n';
        }
    } catch (const std::exception &e) {
        std::cerr << "Caught expression: \n" << e.what() << '\n';
    }

}

void BooksPostgres::drop() {
    try {
        soci::session sql(soci::postgresql, this->DATABASE_URL);
        sql << "DROP TABLE IF EXISTS books";
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}

void BooksPostgres::solve() {
    drop();
    initialize();

    std::vector<std::future<void> >futures;
    futures.emplace_back(std::async(std::launch::async, &BooksPostgres::insertBook, this, "Book A", "Author 1", 2001));
    futures.emplace_back(std::async(std::launch::async, &BooksPostgres::insertBook, this, "Book B", "Author 2", 1999));
    futures.emplace_back(std::async(std::launch::async, &BooksPostgres::insertBook, this, "Book C", "Author 3", 2015));

    for (auto &f : futures) {
        try {
            f.get();
        } catch (const std::exception &e) {
            std::cerr << "Caught exception while getting futures: \n" << e.what() << '\n';
        }
    }
    getBooks();
}

BooksPostgres::BooksPostgres(const std::string &connectionString) {
    this->DATABASE_URL = connectionString;
}