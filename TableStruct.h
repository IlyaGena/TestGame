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


/// таблица данных игрового поля
static const TableStruct TABLE_DATA
(
        "data",

        {
            "id",           //!< 0 - id записи
            "number",       //!< 1 - номер ячейки
            "data",         //!< 2 - данные
        },

        {
            "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE", //!< 0 - id записи
            "TEXT NOT NULL",                            //!< 1 - номер ячейки
            "TEXT NOT NULL",                            //!< 2 - данные
        }
);


/// таблица счета
static const TableStruct TABLE_SCORE
(
        "score",

        {
            "id",           //!< 0 - id записи
            "score",        //!< 1 - счет
        },

        {
            "INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE",     //!< 0 - id записи
            "TEXT NOT NULL",                                //!< 1 - счет
        }
);


#endif // TABLESTRUCT_H
