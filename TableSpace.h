#ifndef TABLESPACE_H
#define TABLESPACE_H

#include <QObject>
#include <QAbstractTableModel>
#include <qqml.h>
#include <QRandomGenerator>
#include <QDebug>

#include "Database.h"

#define WIDTH_SPACE 9
#define HEIGHT_SPACE 9
#define COUNT_BALL 3

/*!
 * \brief The Color enum - цвета
 */
enum Color
{
    Red,
    Blue,
    Green,
    Black
};

/*!
 * \brief The TableSpace class - главный класс для работы с моделью
 */
class TableSpace: public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_ADDED_IN_MINOR_VERSION(1)

public:
    TableSpace(QObject *parent = nullptr);
    ~TableSpace();

    /*!
     * \brief rowCount - получение количества строк
     * \return - количество строк
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /*!
     * \brief columnCount - получение количества столбцов
     * \return - количество столбцов
     */
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    /*!
     * \brief data - получение информации
     * \param index - запрашиваемый индекс
     * \param role - роль
     * \return - данные
     */
    QVariant data(const QModelIndex &index, int role) const override;

    /*!
     * \brief setData - установка нового значения
     * \param new_index - индекс для нового значения
     * \param value - значение (цвет)
     * \param role - роль
     * \return результат изменения
     */
    Q_INVOKABLE virtual bool setData(const QModelIndex &new_index, const QVariant &value, int role = Qt::EditRole) override;

    /*!
     * \brief step - метод генерации новых шариков
     */
    Q_INVOKABLE void step();

    /*!
     * \brief getScore - получение счета
     * \return - значение счета
     */
    Q_INVOKABLE quint16 getScore();

    /*!
     * \brief newGame - очистка игрового поля и начало новой игры
     */
    Q_INVOKABLE void newGame();

signals:
    /*!
     * \brief gameEnd - конец игры
     */
    void gameEnd();
    /*!
     * \brief changeScore - изменение счета
     */
    void changeScore();
    /*!
     * \brief newGameStart - сигнал начала новой игры
     */
    void newGameStart();

private:
    /*!
     * \brief getValue - получение цвета из списка значений (mm_table)
     * \param index - запрашиваемый индекс
     * \return - цвет
     */
    QString getValue(const QModelIndex &index) const;

    /*!
     * \brief generateColor - генератор нового цвета
     * \return - цвет
     */
    QString generateColor();

    /*!
     * \brief generateIndex - генератор индекса
     * \param index - индекс в формате QModelIndex
     * \return - индекс в mm_table
     */
    quint16 generateIndex(QModelIndex &index);

    /*!
     * \brief checkVert - проверка столбца по вертикали
     * \param index - индекс последнего вставленного щарика в столбце
     * \param color - цвет шарика
     * \return - результат проверки
     */
    bool checkVert(const QModelIndex &index, const QString color);

    /*!
     * \brief checkHor - проверка столбца по горизонтали
     * \param index - индекс последнего вставленного щарика в столбце
     * \param color - цвет шарика
     * \return - результат проверки
     */
    bool checkHor(const QModelIndex &index, const QString color);

private:

    quint8 mm_width;                            //!< ширина поля
    quint8 mm_height;                           //!< высота поля
    quint16 mm_size;                            //!< размер поля(размера массивая mm_table)
    quint16 mm_score;                           //!< счет игры

    QMap<quint16, QString>  mm_table;           //!< таблица хранения идентификатора клетки и его цвета
    QRandomGenerator        mm_rand;            //!< объект рандома

    Database mm_db;                             //!< объект БД

};

#endif // TABLESPACE_H
