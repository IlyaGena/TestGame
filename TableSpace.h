#ifndef TABLESPACE_H
#define TABLESPACE_H

#include <QObject>
#include <QAbstractTableModel>
#include <qqml.h>
#include <QRandomGenerator>
#include <QDebug>

#define WIDTH_SPACE 9
#define HEIGHT_SPACE 9
#define COUNT_BALL 3

enum Color
{
    Red,
    Blue,
    Yellow,
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
    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_INVOKABLE void click();

signals:
    void gameEnd();

private:
    QString getValue(const QModelIndex &index) const;

    QString generateColor();

    quint16 generateIndex(QModelIndex &index);

private:
    quint8 mm_width;
    quint8 mm_height;
    quint16 mm_size;

    QMap<quint16, QString>  mm_table;
    QRandomGenerator        mm_rand;            //!< объект рандома

};

#endif // TABLESPACE_H
