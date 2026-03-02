#include "school-soci.h"

void SchoolPostgres::initialize() {
    try {
        soci::session sql(soci::postgresql, this->DATABASE_URL);
        sql << 
            "CREATE TABLE IF NOT EXISTS students ("
            "id SERIAL PRIMARY KEY,"
            "name TEXT NOT NULL);";
        sql << 
            "CREATE TABLE IF NOT EXISTS courses ("
            "id SERIAL PRIMARY KEY,"
            "title TEXT UNIQUE NOT NULL);";
        sql << 
            "CREATE TABLE IF NOT EXISTS student_courses ("
            "student_id INTEGER NOT NULL,"
            "course_id INTEGER NOT NULL,"
            "PRIMARY KEY (student_id, course_id),"
            "FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,"
            "FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE);";
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}

void SchoolPostgres::drop() {
    try {
        soci::session sql(soci::postgresql, this->DATABASE_URL);
        sql << "DROP TABLE IF EXISTS students;";
        sql << "DROP TABLE IF EXISTS courses;";
        sql << "DROP TABLE IF EXISTS student_courses;";
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}

void SchoolPostgres::insert() {
    try {
        soci::session sql(soci::postgresql, this->DATABASE_URL);
        sql << "INSERT INTO students (name) VALUES ('Alice'), ('Bob'), ('Charlie');";
        sql << "INSERT INTO courses (title) VALUES ('Math'), ('Physics'), ('Chemistry');";

        sql << "INSERT INTO student_courses (student_id, course_id) VALUES (1, 1), (1, 2);";
        sql << "INSERT INTO student_courses (student_id, course_id) VALUES (2, 2), (2, 3);";
        sql << "INSERT INTO student_courses (student_id, course_id) VALUES (3, 1), (3, 2), (3, 3);";
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}

void SchoolPostgres::solve() {
    try {
        soci::session sql(soci::postgresql, this->DATABASE_URL);

        std::string query_sc = 
            "SELECT s.name, STRING_AGG(c.title, ', ' ORDER BY c.title) as courses "
            "FROM students s "
            "JOIN student_courses sc ON s.id = sc.student_id "
            "JOIN courses c ON sc.course_id = c.id "
            "GROUP BY s.name "
            "ORDER BY s.name;";
        std::string name, courses;
        soci::statement sc = (sql.prepare << query_sc, soci::into(name), soci::into(courses));
        sc.execute();
        std::cout << "Query 4.1: \n";
        while (sc.fetch()) {
            std::cout << name << " | " << courses << '\n';
        }

        std::string query_phys = 
            "SELECT s.name "
            "FROM students s "
            "JOIN student_courses sc ON s.id = sc.student_id "
            "JOIN courses c ON sc.course_id = c.id "
            "WHERE c.title = 'Physics' "
            "ORDER BY s.name;";
        soci::statement phys = (sql.prepare << query_phys, soci::into(name));
        phys.execute();
        std::cout << "Query 4.2: \n";
        while (phys.fetch()) {
            std::cout << name << '\n';
        }
    } catch (const std::exception &e) {
        std::cerr << "Caught exception: \n" << e.what() << '\n';
    }
}

SchoolPostgres::SchoolPostgres(const std::string& connectionString) {
    this->DATABASE_URL = connectionString;
}