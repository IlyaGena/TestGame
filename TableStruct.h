#ifndef TABLESTRUCT_H
#define TABLESTRUCT_H

#include <QStringList>

/*!
 * \brief The TableStruct struct - общая структура таблицы в БД
 */
struct TableStruct
{
    /*!
     * \brief TableStruct - конструктор таблицы
     * \param name - имя таблицы
     * \param columns - имена столбцов
     * \param types - список типов данных в столбцах
     */
    TableStruct(QString name, QStringList columns, QStringList types) :
        name(name),
        columns(columns),
        types(types)
    {}

    QString name;           //!< имя таблицы
    QStringList columns;    //!< имена столбцов
    QStringList types;      //!< список типов данных в столбцах
};


/// таблица учетных записей пользователей
static const TableStruct TABLE_DATA
(
        "data",

        {
            "id",           //!< 0 - id записи
            "score",        //!< 1 - счет
            "data",         //!< 2 - таблица
        },

        {
            "PRIMARY KEY UNIQUE",           //!< 0 - id записи
            "TEXT NOT NULL",                //!< 1 - счет
            "BLOB NOT NULL",                //!< 2 - таблица
        }
);

#endif // TABLESTRUCT_H
