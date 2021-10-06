#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QAbstractTableModel>
#include <qqml.h>
#include <QRandomGenerator>

#include <QDebug>

#define WIDTH_SPACE 9
#define HEIGHT_SPACE 9

enum Color
{
    Red,
    Blue,
    Yellow,
    Black
};

class BackEnd : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_ADDED_IN_MINOR_VERSION(1)

    enum Roles {
        CellRole
    };

public:

    BackEnd(QObject *parent = nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE void click(const QModelIndex &indexFrom, const QModelIndex &indexTo);

    QString getValue(const QModelIndex &index) const;

    QString generateColor();
    quint16 generateIndex();

private:
    quint8 mm_width;
    quint8 mm_height;
    quint16 mm_size;

    QMap<quint16, QString>  mm_table;
    QRandomGenerator        mm_rand;            //!< объект рандома

    quint16 tmp;
};

#endif // BACKEND_H
