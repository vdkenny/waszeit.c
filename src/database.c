#include "database.h"
#include <sqlite3.h>
#include <stdio.h>

sqlite3 *database_init() {
  sqlite3 *database;
  if (sqlite3_open("database.db", &database) != SQLITE_OK) {
    return NULL;
  }

  sqlite3_exec(database,
               "CREATE TABLE IF NOT EXISTS mouse_event("
               "id INTEGER PRIMARY KEY,"
               "time INTEGER NOT NULL"
               "message INTEGER NOT NULL"
               ");",
               NULL, NULL, NULL);

  sqlite3_exec(database,
               "CREATE TABLE IF NOT EXISTS keyboard_event("
               "id INTEGER PRIMARY KEY,"
               "vk_code INTEGER NOT NULL"
               "time INTEGER NOT NULL"
               "message INTEGER NOT NULL"
               ");",
               NULL, NULL, NULL);

  return database;
}
