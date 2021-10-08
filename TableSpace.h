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

enum Color
{
    Red,
    Blue,
    Green,
    Black
};

class TableSpace: public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_ADDED_IN_MINOR_VERSION(1)

public:
    TableSpace(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE virtual bool setData(const QModelIndex &new_index, const QVariant &value, int role = Qt::EditRole) override;

    Q_INVOKABLE void step();

    Q_INVOKABLE void newGame();

signals:
    void gameEnd();
    void changeScore(quint16 score);
    void newGameStart();

private:
    QString getValue(const QModelIndex &index) const;

    QString generateColor() ;

    quint16 generateIndex(QModelIndex &index);

    bool checkVert(const QModelIndex &index, const QString color);
    bool checkHor(const QModelIndex &index, const QString color);

private:
    quint8 mm_width;
    quint8 mm_height;
    quint16 mm_size;
    quint16 mm_score;

    QMap<quint16, QString>  mm_table;
    QRandomGenerator        mm_rand;            //!< объект рандома

    Database mm_db;

};

#endif // TABLESPACE_H
