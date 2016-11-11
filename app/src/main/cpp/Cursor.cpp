//
// Created by William Kamp on 10/10/16.
//

#include "Cursor.h"

//region Class Methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//endregion

//region Constructor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cursor::Cursor(sqlite3_stmt *statement) : statement(statement) {
    evaluateCount();
}

Cursor::Cursor(Cursor &&curs) {
    statement = curs.statement;
    count = curs.count;
    position = curs.position;
    curs.statement = nullptr;
    curs.count = 0;
    curs.position = 0;
}

Cursor::~Cursor() {
    if (statement) {
        sqlite3_finalize(statement);
    }
}

//endregion

//region Public Methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool Cursor::moveToFirst() {
    if (sqlite3_reset(statement) == SQLITE_OK && sqlite3_step(statement) == SQLITE_ROW) {
        position = 0;
        return true;
    }
    return false;
}

int Cursor::getCount() {
    return count;
}

bool Cursor::moveToPosition(int p) {
    if (p >= count) {
        return false;
    }
    if (p < position) {
        moveToFirst();
    }
    while (position < p && moveToNext());
    return position == p;
}

bool Cursor::moveToNext() {
    if (!isAfterLast()) {
        int stepResult = sqlite3_step(statement);
        if (stepResult == SQLITE_ROW || SQLITE_DONE) {
            ++position;
            return true;
        }
    }
    return false;
}

bool Cursor::isAfterLast() {
    return count > 0 && position >= count;
}

const std::string Cursor::getString(int columnIndex) const {
    const unsigned char* text = sqlite3_column_text(statement, columnIndex);
    if (text) {
        return std::string(reinterpret_cast<const char*>(text));
    }
    return "";
}

const std::vector<byte> Cursor::getBlob(int columnIndex) const {
    const void *blob = sqlite3_column_blob(statement, columnIndex);
    int sz = sqlite3_column_bytes(statement, columnIndex);
    const byte *charBuf = reinterpret_cast<const byte*>(blob);
    std::vector<byte> value(charBuf, charBuf + sz);
    return value;
}

int64_t Cursor::getInt(int columnIndex) {
    return (int64_t) sqlite3_column_int(statement, columnIndex);
}

double Cursor::getReal(int columnIndex) {
    return sqlite3_column_double(statement, columnIndex);
}

//endregion

//region Private Methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Cursor::evaluateCount() {
    while (sqlite3_step(statement) == SQLITE_ROW) {
        ++count;
    }
    sqlite3_reset(statement);
}

//endregion
