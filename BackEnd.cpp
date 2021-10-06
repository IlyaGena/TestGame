#include "BackEnd.h"

BackEnd::BackEnd(QObject *parent) :
    QAbstractTableModel(parent)
{
    mm_width = WIDTH_SPACE;
    mm_height = HEIGHT_SPACE;
    mm_size = mm_width * mm_height;

    for(auto i = 0; i < mm_size; i++)
        mm_table.insert(i, "white");
}

int BackEnd::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 9;
}

int BackEnd::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 9;
}

QVariant BackEnd::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    {
        QString value = getValue(index);
        return value;
    }
    default:
        break;
    }

    return QVariant();
}

void BackEnd::click(const QModelIndex &indexFrom, const QModelIndex &indexTo)
{
    qDebug() << "Click!!!!";

    quint16 indexCell = generateIndex();
    QString value = generateColor();

    mm_table.insert(indexCell, value);

    emit dataChanged(indexFrom, indexTo);
}

QString BackEnd::getValue(const QModelIndex &index) const
{
    auto undex = index.row() * mm_width + index.column();
    QString value = mm_table.value(index.row() * mm_width + index.column());
    qDebug() << "getValue:" << undex;
    if (undex == tmp)
        auto tn = 1;
    return value;
}

QString BackEnd::generateColor()
{
    Color color = Color(mm_rand.generate() % 4);
    qDebug() << "Generated int:" << color;

    QString value;
    switch (color) {
    case Color::Red:
        value = "red";
        break;
    case Color::Blue:
        value = "blue";
        break;
    case Color::Yellow:
        value = "yellow";
        break;
    case Color::Black:
        value = "black";
        break;
    }

    qDebug() << "Generated color:" << value;
    return value;
}

quint16 BackEnd::generateIndex()
{
    quint16 index = mm_rand.generateDouble() * mm_size;
    tmp = index;
    qDebug() << "Generated Index:" << index;
    return index;
}
