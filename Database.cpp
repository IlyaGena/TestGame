#include "Database.h"

Database::Database(QObject *parent) :
    QObject(parent),
    mm_database(QSqlDatabase::addDatabase("QSQLITE", DB_NAME))
{
    mm_database.setDatabaseName(DB_NAME);
    open();
}

void Database::open()
{
    // открытие БД
    mm_database.open();

    qDebug() << "подключение экземпляра базы данных";

    if (!mm_database.isOpen() || !mm_database.isValid())
    {
        qDebug() << "критическая ошибка: ошибка подключения экземпляра базы данных";
        sqlError(mm_database.lastError());

        exit(EXIT_FAILURE);
    }

    // инициализация таблиц
    initializeTable(TABLE_DATA);
    initializeTable(TABLE_SCORE);
}

void Database::close()
{
    mm_database.close();
    qDebug() << "отключение экземпляра базы данных";

    sqlError(mm_database.lastError());
}

void Database::initializeTable(TableStruct table)
{
    QSqlQuery query = QSqlQuery(mm_database);

    // инициализация таблиц
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='" + table.name + "';");
    sqlError(query.lastError());

    if (!query.next())
    {
        QString createCommand = "CREATE TABLE " + table.name + " (";

        for (int i = 0; i < table.columns.count(); i++)
        {
            createCommand += table.columns.value(i) + " " + table.types.value(i);
            createCommand += ", ";
        }

        // удаляем ", "
        createCommand.remove(createCommand.length() - 2, 2);

        createCommand += ");";

        qDebug() << "создание таблицы: " + table.name;

        query.exec(createCommand);
        sqlError(query.lastError());
    }
    return;
}

void Database::sqlError(QSqlError error)
{
    if (!error.isValid())
        return;

    qDebug() << error;

    QString message = QString("ошибка выполнения SQL-запроса: "
                              "driverText(%1) databaseText(%2) nativeErrorCode(%3)").arg(
                error.driverText(),
                error.databaseText(),
                error.nativeErrorCode());

    qDebug() << message;
}

QSqlTableModel *Database::getTable(TableStruct tableStruct)
{
    QSqlTableModel *table = new QSqlTableModel(this, mm_database);

    // имя таблицы
    table->setTable(tableStruct.name);

    table->setEditStrategy(QSqlTableModel::OnManualSubmit);
    table->select();

    while (table->canFetchMore())
        table->fetchMore();

    return table;
}

bool Database::commitTable(QSqlTableModel *table)
{
    bool result = table->submitAll();
    sqlError(table->lastError());

    delete table;
    return result;
}

QMap<quint16, QString> Database::getData()
{
    // инициализация указателя на таблицу
    QSqlTableModel *tableDB = getTable(TABLE_DATA);

    // возвращаемы список
    QMap<quint16, QString> list;

    // пробегаем по таблице и формируем список
    for (auto i = 0; i < tableDB->rowCount(); i++)
    {
        auto key = tableDB->data(tableDB->index(i, 1)).toInt();
        auto value = tableDB->data(tableDB->index(i, 2)).toString();
        list.insert(key, value);
    }

    return list;
}

quint16 Database::getScore()
{
    // инициализация указателя на таблицу
    QSqlTableModel *table = getTable(TABLE_SCORE);

    // возврат значение счета
    return table->data(table->index(0, 1)).toInt();
}

bool Database::saveData(QMap<quint16, QString> table, quint16 score)
{
    // инициализация указателя на таблицу
    QSqlTableModel *tableDB = getTable(TABLE_DATA);

    QList<quint16> keys = table.keys();

    // добавление строки и сохранение данных
    for(auto i = 0; i < keys.count(); i++)
    {
        auto key = keys[i];

        // новая строка
        int row = tableDB->rowCount();
        tableDB->insertRow(row);

        // сохранение
        tableDB->setData(tableDB->index(row, 1), key);
        tableDB->setData(tableDB->index(row, 2), table.value(key));
    }

    // сохранение изменения в табл
    bool result = commitTable(tableDB);

    if (!result)
        return result;

    // инициализация указателя на таблицу
    QSqlTableModel *tableScoreDB = getTable(TABLE_SCORE);

    // добавление строки и сохранение данных
    tableScoreDB->insertRow(0);
    tableScoreDB->setData(tableScoreDB->index(0, 1), score);

    // сохранение изменения в табл
    result = commitTable(tableScoreDB);

    return result;
}

bool Database::deleteData()
{
    // инициализация указателя на таблицу
    QSqlTableModel *tableDB = getTable(TABLE_DATA);

    // удаление данных из табл
    tableDB->removeRows(0, tableDB->rowCount());

    // сохранение изменений
    bool result = commitTable(tableDB);

    if (!result)
        return result;

    // инициализация указателя на таблицу
    QSqlTableModel *tableScoreDB = getTable(TABLE_SCORE);

    // удаление данных из табл
    tableScoreDB->removeRows(0, tableScoreDB->rowCount());

    // сохранение изменений
    result = commitTable(tableScoreDB);

    return result;
}
