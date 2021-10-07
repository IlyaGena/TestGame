#include "TableSpace.h"

TableSpace::TableSpace(QObject *parent)
{
    mm_width = WIDTH_SPACE;
    mm_height = HEIGHT_SPACE;
    mm_size = mm_width * mm_height;

    for(auto i = 0; i < mm_size; i++)
        mm_table.insert(i, "white");
}

int TableSpace::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 9;
}

int TableSpace::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 9;
}

QVariant TableSpace::data(const QModelIndex &index, int role) const
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

bool TableSpace::setData(const QModelIndex &new_index, const QVariant &value, int role)
{
    quint16 new_index_list = new_index.row() * mm_width + new_index.column();
    QString color = value.toString();
    mm_table[new_index_list] = value.toString();
    emit dataChanged(new_index, new_index);
    return true;
}

void TableSpace::click()
{
    qDebug() << "Click!!!!";

    QModelIndex index_model;

    QList<QString> listVal = mm_table.values();

    if (!listVal.contains("white"))
    {
        emit gameEnd();
        return;
    }

    for(auto i = 0; i < COUNT_BALL; i ++)
    {
        quint16 index_list = generateIndex(index_model);
        QString value = generateColor();

        if (mm_table.value(index_list) == "white" || mm_table.value(index_list) == "#ffffff")
        {
            mm_table.insert(index_list, value);
            emit dataChanged(index_model, index_model);
        }
        else
            i--;
    }
}

QString TableSpace::getValue(const QModelIndex &index) const
{
    QString value = mm_table.value(index.row() * mm_width + index.column());
    return value;
}

QString TableSpace::generateColor()
{
    Color color = Color(mm_rand.generate() % 4);

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

quint16 TableSpace::generateIndex(QModelIndex& index)
{
    quint8 width = mm_rand.generateDouble() * mm_width;
    quint8 height = mm_rand.generateDouble() * mm_height;

    index = this->index(width, height);

    auto index_list = width * mm_width + height;

    qDebug() << "Generated Index:" << index_list;
    return index_list;
}
