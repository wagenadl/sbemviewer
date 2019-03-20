// Database.cpp

#include "Database.h"
#include "PDebug.h"
#include <QSqlError>
#include <system_error>
#include <QSqlQuery>
//#include <sqlite3.h>
#include <QSqlDriver>

Database::Database(QString id0): id(id0), transWait(new QAtomicInt()) {
  if (id.isEmpty())
    id = autoid();
}

void Database::open(QString filename) {
  if (db.isOpen())
    close();

  db = QSqlDatabase::addDatabase("QSQLITE", id);
  db.setDatabaseName(filename);
  db.setConnectOptions("QSQLITE_ENABLE_REGEXP=1");
  if (!db.open())
    CRASH("Could not open database " + filename);
  query("pragma foreign_keys = on");
}

void Database::close() {
  db.close();
  db = QSqlDatabase();
  QSqlDatabase::removeDatabase(id);
}

QString Database::autoid() {
  static int id=0;
  id++;
  return QString("db%1").arg(id);
}

void Database::clone(Database const &src) {
  if (db.isOpen())
    close();
  db = QSqlDatabase::cloneDatabase(src.db, id);
  db.setConnectOptions("QSQLITE_ENABLE_REGEXP=1");
  if (!db.open()) 
    CRASH("Could not open cloned database:" + db.databaseName());
  transWait = src.transWait;
}

Database::~Database() {
  if (db.isOpen()) {
    CRASH("Database destructed while still open: " + db.databaseName());
    close();
  }
}

void Database::begin() {
  if (!db.transaction()) 
    CRASHDB("Could not begin transaction");
}

void Database::commit() {
  if (!db.commit())
    CRASHDB("Could not commit transaction");
}

void Database::rollback() {
  if (!db.rollback()) 
    CRASHDB("Could not rollback transaction");
}

QVariant Database::defaultQuery(QString s, QVariant dflt) const {
  QSqlQuery q = constQuery(s);
  return q.next() ? q.value(0) : dflt;
}
 
QVariant Database::defaultQuery(QString s, QVariant a, QVariant dflt) const {
  QSqlQuery q = constQuery(s, a);
  return q.next() ? q.value(0) : dflt;
}
  
QVariant Database::simpleQuery(QString s) const {
  QSqlQuery q = constQuery(s);
  if (!q.next())
    CRASH("No result");
  return q.value(0);
}

QVariant Database::simpleQuery(QString s, QVariant a) const {
  QSqlQuery q = constQuery(s, a);
  if (!q.next())
    CRASH("No result");
  return q.value(0);
}
   
QVariant Database::simpleQuery(QString s, QVariant a, QVariant b) const {
  QSqlQuery q = constQuery(s, a, b);
  if (!q.next())
    CRASH("No result");
  return q.value(0);
}

QVariant Database::simpleQuery(QString s, QVariant a, QVariant b,
                               QVariant c) const {
  QSqlQuery q = constQuery(s, a, b, c);
  if (!q.next())
    CRASH("No result");
  return q.value(0);
}
   
QVariant Database::simpleQuery(QString s, QVariant a, QVariant b,
                               QVariant c, QVariant d) const {
  QSqlQuery q = constQuery(s, a, b, c, d);
  if (!q.next())
    CRASH("No result");
  return q.value(0);
}
   
QVariant Database::simpleQuery(QString s, QVariant a, QVariant b,
                               QVariant c, QVariant d, QVariant e) const {
  QSqlQuery q = constQuery(s, a, b, c, d, e);
  if (!q.next())
    CRASH("No result");
  return q.value(0);
}

QVariant Database::simpleQuery(QString s, QVariant a, QVariant b,
                               QVariant c, QVariant d, QVariant e,
                               QVariant f) const {
  QSqlQuery q = constQuery(s, a, b, c, d, e, f);
  if (!q.next())
    CRASH("No result");
  return q.value(0);
}
   
QVariant Database::simpleQuery(QString s, QVariant a, QVariant b,
                               QVariant c, QVariant d, QVariant e,
                               QVariant f, QVariant g) const {
  QSqlQuery q = constQuery(s, a, b, c, d, e, f, g);
  if (!q.next())
    CRASH("No result");
  return q.value(0);
}
   
QSqlQuery Database::query(QString s) {
  // Operationally, query and constQuery are identical.
  // Both are provided for easier understanding of code.
  return constQuery(s);
}

QSqlQuery Database::constQuery(QString s) const {
  if (debugging())
    pDebug() << "query" << (void*)this << s;
  QSqlQuery q(db);
  q.prepare(s);
  if (!q.exec())
    CRASHQ(q);
  if (debugging())
    pDebug() << "query" << (void*)this << "executed";
  return q;
}

QSqlQuery Database::query(QString s, QVariant a) {
  return constQuery(s, a);
}

QSqlQuery Database::constQuery(QString s, QVariant a) const {
  if (debugging())
    pDebug() << "query" << (void*)this << s;
  QSqlQuery q(db);
  q.prepare(s);
  q.bindValue(":a", a);
  if (!q.exec())
    CRASHQ(q);
  if (debugging())
    pDebug() << "query" << (void*)this << "executed";
  return q;
}

QSqlQuery Database::query(QString s, QVariant a, QVariant b) {
  return constQuery(s, a, b);
}

QSqlQuery Database::constQuery(QString s, QVariant a, QVariant b) const {
  if (debugging())
    pDebug() << "query" << (void*)this << s;
  QSqlQuery q(db);
  q.prepare(s);
  q.bindValue(":a", a);
  q.bindValue(":b", b);
  if (!q.exec())
    CRASHQ(q);
  if (debugging())
    pDebug() << "query" << (void*)this << "executed";
  return q;
}

QSqlQuery Database::query(QString s, QVariant a, QVariant b, QVariant c) {
  return constQuery(s, a, b, c);
}

QSqlQuery Database::constQuery(QString s, QVariant a, QVariant b,
                               QVariant c) const {
  if (debugging())
    pDebug() << "query" << (void*)this << s;
  QSqlQuery q(db);
  q.prepare(s);
  q.bindValue(":a", a);
  q.bindValue(":b", b);
  q.bindValue(":c", c);
  if (!q.exec())
    CRASHQ(q);
  if (debugging())
    pDebug() << "query" << (void*)this << "executed";
  return q;
}

QSqlQuery Database::query(QString s, QVariant a, QVariant b, QVariant c,
                          QVariant d) {
  return constQuery(s, a, b, c, d);
}

QSqlQuery Database::constQuery(QString s, QVariant a, QVariant b, QVariant c,
                               QVariant d) const {
  if (debugging())
    pDebug() << "query" << (void*)this << s;
  QSqlQuery q(db);
  q.prepare(s);
  q.bindValue(":a", a);
  q.bindValue(":b", b);
  q.bindValue(":c", c);
  q.bindValue(":d", d);
  if (!q.exec())
    CRASHQ(q);
  if (debugging())
    pDebug() << "query" << (void*)this << "executed";
  return q;
}

QSqlQuery Database::query(QString s, QVariant a, QVariant b, QVariant c,
                          QVariant d, QVariant e) {
  return constQuery(s, a, b, c, d, e);
}

QSqlQuery Database::constQuery(QString s, QVariant a, QVariant b, QVariant c,
                               QVariant d, QVariant e) const {
  if (debugging())
    pDebug() << "query" << (void*)this << s;
  QSqlQuery q(db);
  q.prepare(s);
  q.bindValue(":a", a);
  q.bindValue(":b", b);
  q.bindValue(":c", c);
  q.bindValue(":d", d);
  q.bindValue(":e", e);
  if (!q.exec())
    CRASHQ(q);
  if (debugging())
    pDebug() << "query" << (void*)this << "executed";
  return q;
}

QSqlQuery Database::query(QString s, QVariant a, QVariant b, QVariant c,
                          QVariant d, QVariant e, QVariant f) {
  return constQuery(s, a, b, c, d, e, f);
}

QSqlQuery Database::query(QString s, QVariant a, QVariant b, QVariant c,
                          QVariant d, QVariant e, QVariant f, QVariant g) {
  return constQuery(s, a, b, c, d, e, f, g);
}

QSqlQuery Database::constQuery(QString s, QVariant a, QVariant b, QVariant c,
                               QVariant d, QVariant e, QVariant f) const {
  if (debugging())
    pDebug() << "query" << (void*)this << s;
  QSqlQuery q(db);
  q.prepare(s);
  q.bindValue(":a", a);
  q.bindValue(":b", b);
  q.bindValue(":c", c);
  q.bindValue(":d", d);
  q.bindValue(":e", e);
  q.bindValue(":f", f);
  if (!q.exec())
    CRASHQ(q);
  if (debugging())
    pDebug() << "query" << (void*)this << "executed";
  return q;
}

QSqlQuery Database::constQuery(QString s, QVariant a, QVariant b, QVariant c,
                               QVariant d, QVariant e, QVariant f,
                               QVariant g) const {
  if (debugging())
    pDebug() << "query" << (void*)this << s;
  QSqlQuery q(db);
  q.prepare(s);
  q.bindValue(":a", a);
  q.bindValue(":b", b);
  q.bindValue(":c", c);
  q.bindValue(":d", d);
  q.bindValue(":e", e);
  q.bindValue(":f", f);
  q.bindValue(":g", g);
  if (!q.exec())
    CRASHQ(q);
  if (debugging())
    pDebug() << "query" << (void*)this << "executed";
  return q;
}

QSqlQuery Database::query() {
  if (debugging()) 
    pDebug() << "query " << (void*)this;
  return QSqlQuery(db);
}

void Database::enableDebug() {
  debugging() = true;
}

void Database::disableDebug() {
  debugging() = false;
}

bool &Database::debugging() {
  static bool dbg = false;
  return dbg;
}

//////////////////////////////////////////////////////////////////////

Transaction::Transaction(Database *db): db(db) {
  cmt = false;
  db->transWait->ref();
  db->begin();
  db->transWait->deref();
}

void Transaction::commit() {
  db->commit();
  cmt = true;
}

Transaction::~Transaction() {
  if (!cmt) {
    db->rollback();
  }
}

Untransaction::Untransaction(Database *db): db(db) {
  db->transWait->ref();
}

Untransaction::~Untransaction() {
  db->transWait->deref();
}

bool Database::transactionsWaiting() const {
  return *transWait > 0;
}
