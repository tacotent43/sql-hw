#include "books-clickhouse.h"

void BooksClickhouse::initialize() {
    try {
        if (!this->client) {
            std::cerr << "Client is null!" << '\n';
            return;
        }
        this->client->Execute(
            "CREATE TABLE IF NOT EXISTS books ("
            "    id UInt64,"
            "    title String,"
            "    author String,"
            "    year UInt16)" 
            "ENGINE = MergeTree "
            "ORDER BY id");
        std::cout << "Table created successfully\n";
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in initialize: \n" << e.what() << '\n';
    }
}

void BooksClickhouse::insertBook(int id, std::string title, std::string author, int year) {
    auto idColumn = std::make_shared<clickhouse::ColumnUInt64>();
    auto titleColumn = std::make_shared<clickhouse::ColumnString>();
    auto authorColumn = std::make_shared<clickhouse::ColumnString>();
    auto yearColumn = std::make_shared<clickhouse::ColumnUInt16>();

    idColumn->Append(id);
    titleColumn->Append(title);
    authorColumn->Append(author);
    yearColumn->Append(year);

    clickhouse::Block books{};
    books.AppendColumn("id", idColumn);
    books.AppendColumn("title", titleColumn);
    books.AppendColumn("author", authorColumn);
    books.AppendColumn("year", yearColumn);

    this->client->Insert("books", books);
}

void BooksClickhouse::getBooks() {
    this->client->Select(
        "SELECT title FROM books WHERE year > 2000",
        [](const clickhouse::Block &block) {
            for (size_t row_idx = 0; row_idx < block.GetRowCount(); ++row_idx) {
                auto title = block[0]->AsStrict<clickhouse::ColumnString>()->At(row_idx);
                std::cout << title << '\n';
            }
        }
    );
}

void BooksClickhouse::drop() {
    try {
        this->client->Execute("DROP TABLE IF EXISTS books");
    } catch (const std::exception &e) {
        std::cout << "Caught exception: \n" << e.what() << '\n';
    }
}

void BooksClickhouse::solve() {
    drop();
    initialize();

    std::vector<std::future<void> >futures;
    futures.emplace_back(std::async(std::launch::async, &BooksClickhouse::insertBook, this, 1, "Book A", "Author 1", 2001));
    futures.emplace_back(std::async(std::launch::async, &BooksClickhouse::insertBook, this, 2, "Book B", "Author 2", 1999));
    futures.emplace_back(std::async(std::launch::async, &BooksClickhouse::insertBook, this, 3, "Book C", "Author 3", 2015));

    // mutex is unnecessary
    std::mutex m;
    for (auto &f : futures) {
        std::lock_guard<std::mutex> lock(m); // ?
        f.get();
    }
    getBooks();
}

BooksClickhouse::BooksClickhouse(
    const std::string &host, 
    const std::string &user,
    const std::string &password,
    int port,
    bool ssl
) {
    try {
        std::cout << "Connecting to ClickHouse at " << host << ":" << port << std::endl;
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
        std::cout << "Connected successfully!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in constructor: \n" << e.what() << '\n';
    }
}