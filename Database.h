#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QDebug>

#include "TableStruct.h"

#define DB_NAME "db.sdb"

/*!
 * \brief The Database class - класс для работы с БД
 */
class Database : public QObject
{
    Q_OBJECT

public:
    Database(QObject *parent);

    /*!
     * \brief open - открыть файл БД
     */
    void open();

    /*!
     * \brief open - закрыть файл БД
     */
    void close();

public:
    /*!
     * \brief getData - получить данные из таблицы данных
     * \return - QMap<quint16, QString>
     */
    QMap<quint16, QString> getData();

    /*!
     * \brief getScore - получаем счет
     * \return - число
     */
    quint16 getScore();

    /*!
     * \brief saveData - сохранение данных и счета в БД
     * \param table - данные
     * \param score - счет
     * \return - результат сохранения
     */
    bool saveData(QMap<quint16, QString> table, quint16 score);

    /*!
     * \brief deleteData - удаление старых данных и счета
     * \return - результат удаления
     */
    bool deleteData();

private:
    /*!
     * \brief initializeTable проверка существовнаия таблицы, если такой нет - создать
     * \param table имя таблицы
     */
    void initializeTable(TableStruct table);
    /*!
     * \brief sqlError ошибка БД
     * \param error описание ошибки
     */
    void sqlError(QSqlError error);
    /*!
     * \brief getTable - динамическое создание указателя на таблицу
     * \param tableStruct - внутренняя структура таблицы
     * \return - указатель на объект таблицы
     */
    QSqlTableModel *getTable(TableStruct tableStruct);
    /*!
     * \brief commitTable - применение изменений таблицы и удаление объекта таблицы
     * \param table - указатель на объект таблицы
     * \return - результат сохранения изменений
     */
    bool commitTable(QSqlTableModel *table);

private:
    QSqlDatabase mm_database;       //!< объект БД
};

#endif // DATABASE_H
