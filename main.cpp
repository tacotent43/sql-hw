#include <iostream>

#ifdef HAVE_SOCI
#include "books-soci.h"
#include "people-soci.h"
#include "school-soci.h"
#endif

#ifdef HAVE_CLICKHOUSE
#include "books-clickhouse.h"
#include "people-clickhouse.h"
#include "school-clickhouse.h"
#endif

int main(int argc, char** argv) {
    // if (argc != 2) {
    //     printf("Given %d arguments. Using SOCI by default...\n", argc - 1);
    //     printf("Usage example: ./sqlab --soci | ./sqlab --clickhouse\n");
    // }

#ifdef HAVE_SOCI
    BooksPostgres secondTask = BooksPostgres("dbname=postgres user=postgres password=pass host=localhost port=5432");
    secondTask.solve();

    PeoplePostgres thirdTask = PeoplePostgres("dbname=postgres user=postgres password=pass host=localhost port=5432");
    thirdTask.solve();

    SchoolPostgres fourthTask = SchoolPostgres("dbname=postgres user=postgres password=pass host=localhost port=5432");
    fourthTask.solve();
#endif

#ifdef HAVE_CLICKHOUSE
    BooksClickhouse secondTask = BooksClickhouse("127.0.0.1", "default", "", 9000, false);
    PeopleClickhouse thirdTask = PeopleClickhouse("127.0.0.1", "default", "", 9000, false);
    SchoolClickhouse fourthTask = SchoolClickhouse("127.0.0.1", "default", "", 9000, false);
    
    secondTask.solve();
    thirdTask.solve();
    fourthTask.solve();
#endif

    return 0;
}