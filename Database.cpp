#include "Database.h"

Database::Database(QObject *parent) :
    QObject(parent),
    mm_database(QSqlDatabase::addDatabase("QSQLITE", DB_NAME))
{
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
}

void Database::close()
{
    mm_database.close();
    qDebug() << "отключение экземпляра базы данных";

    sqlError(mm_database.lastError());
}

QMap<quint16, QString> Database::getData()
{

}

quint16 Database::getScore()
{

}

bool Database::saveData(QMap<quint16, QString> table, quint16 score)
{    //////////////////// добавляем запись в БД ////////////////////
    QSqlTableModel *tableDB = getTable(TABLE_DATA);

    tableDB->setData(tableDB->index(0, 0), 1);
    tableDB->setData(tableDB->index(0, 1), score);
//    tableDB->setData(tableDB->index(0, 2), table);

    bool result = commitTable(tableDB);

    return result;
}

void Database::initializeTable(TableStruct table)
{
    QSqlQuery query = QSqlQuery(mm_database);

    // инициализация таблиц для PSQL
    query.exec("SELECT table_schema || '.' || table_name FROM information_schema.tables "
            "WHERE table_type = 'BASE TABLE' AND table_name='" + table.name + "';");
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

    // имя таблицы для PSQL
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
