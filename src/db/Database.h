// Database.h

#ifndef DATABASE_H

#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>
#include <QVariant>
#include <QSharedPointer>
#include <QAtomicInt>

class Database {
public:
  Database(QString id="");
  virtual void open(QString filename);
  virtual void close();
  virtual void clone(Database const &src);
  virtual ~Database();
  bool isOpen() const { return db.isOpen(); }
  QString name() const { return db.databaseName(); }
  void begin();
  void commit();
  void rollback();
  static void enableDebug();
  static void disableDebug();
  bool transactionsWaiting() const;
public:
  QSqlQuery query();
  // The following execute the query and return the value(0) from the
  // first result row. They abort the application if there is no result.
  QVariant simpleQuery(QString s) const;
  QVariant simpleQuery(QString s, QVariant a) const;
  QVariant simpleQuery(QString s, QVariant a, QVariant b) const;
  QVariant simpleQuery(QString s, QVariant a, QVariant b, QVariant c) const;
  QVariant simpleQuery(QString s, QVariant a, QVariant b, QVariant c,
                       QVariant d) const;
  // The following execute the query and return the value(0) from the
  // first result row. They return the provided default if there is no result.
  QVariant defaultQuery(QString s, QVariant dflt) const;
  QVariant defaultQuery(QString s, QVariant a, QVariant dflt) const;
  // The following execute the query. Result rows can be obtained by repeatedly
  // calling next(). The application is aborted if the query cannot execute.
  QSqlQuery query(QString s);
  QSqlQuery query(QString s, QVariant a);
  QSqlQuery query(QString s, QVariant a, QVariant b);
  QSqlQuery query(QString s, QVariant a, QVariant b, QVariant c);
  QSqlQuery query(QString s, QVariant a, QVariant b, QVariant c, QVariant d);
  QSqlQuery query(QString s, QVariant a, QVariant b, QVariant c, QVariant d,
		  QVariant e);
  QSqlQuery query(QString s, QVariant a, QVariant b, QVariant c, QVariant d,
		  QVariant e, QVariant f);
  // Following are the same, except that caller promises not to alter the db
  QSqlQuery constQuery(QString s) const;
  QSqlQuery constQuery(QString s, QVariant a) const;
  QSqlQuery constQuery(QString s, QVariant a, QVariant b) const;
  QSqlQuery constQuery(QString s, QVariant a, QVariant b, QVariant c) const;
  QSqlQuery constQuery(QString s, QVariant a, QVariant b, QVariant c,
                       QVariant d) const;
  QSqlQuery constQuery(QString s, QVariant a, QVariant b, QVariant c,
                       QVariant d, QVariant e) const;
  QSqlQuery constQuery(QString s, QVariant a, QVariant b, QVariant c,
                       QVariant d, QVariant e, QVariant f) const;
protected:
  QString id;
  QSqlDatabase db;
private:
  Database(Database const &) = delete;
  Database &operator=(Database const &) = delete;
  static bool &debugging();
protected:
  static QString autoid();
  QSharedPointer<QAtomicInt> transWait;
  friend class Transaction;
  friend class Untransaction;
};

class Transaction {
public:
  Transaction(Database *db);
  void commit();
  ~Transaction();
private:
  Database *db;
  bool cmt;
};

class Untransaction {
  // Not a transaction, but causes waiting transactions to wake up when done
public:
  Untransaction(Database *db);
  ~Untransaction();
private:
  Database *db;
};

#endif
