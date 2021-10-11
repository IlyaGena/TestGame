#include "TableSpace.h"

TableSpace::TableSpace(QObject *parent) :
    QAbstractTableModel(parent),
    mm_score(0),
    mm_rand(),
    mm_db(this)
{
    mm_score = 0;
    mm_width = WIDTH_SPACE;
    mm_height = HEIGHT_SPACE;
    mm_size = mm_width * mm_height;

    auto table = mm_db.getData();

    if (!table.isEmpty())
    {
        mm_score = mm_db.getScore();
        mm_table = table;
        emit changeScore(mm_score);
        mm_db.deleteData();
    }
    else {
        for(auto i = 0; i < mm_size; i++)
            mm_table.insert(i, "#ffffff");
        this->step();
    }

}

TableSpace::~TableSpace()
{
    qDebug() << "Exit!";

    mm_db.saveData(mm_table, mm_score);
    mm_db.close();
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
    checkVert(new_index, color);
    checkHor(new_index, color);
    return true;
}

void TableSpace::step()
{
    QModelIndex index_model;

    quint16 count_new_ball = COUNT_BALL;

    QList<quint16> keys_white = mm_table.keys("#ffffff");
    if (count_new_ball > keys_white.count())
        count_new_ball = keys_white.count();

    for(auto i = 0; i < count_new_ball; i ++)
    {
        quint16 index_list = generateIndex(index_model);
        QString value = generateColor();

        if (mm_table.value(index_list) == "#ffffff")
        {
            mm_table[index_list] = value;
            emit dataChanged(index_model, index_model);

            checkVert(index_model, value);
            checkHor(index_model, value);
        }
        else
            i--;
    }

    QList<QString> listVal = mm_table.values();

    if (!listVal.contains("#ffffff"))
    {
        emit gameEnd();
        return;
    }
}

quint16 TableSpace::getScore()
{
    return mm_score;
}

void TableSpace::newGame()
{
    for(auto i = 0; i < mm_size; i++)
        mm_table[i] = "#ffffff";

    auto index_start = this->index(0, 0);
    auto index_end = this->index(WIDTH_SPACE-1, HEIGHT_SPACE-1);
    qDebug() << "index_start: " << index_start;
    qDebug() << "index_end: " << index_end;
    emit dataChanged(index_start, index_end);

    mm_score = 0;
    emit changeScore(mm_score);
    emit newGameStart();
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
        value = "#ff0000";
        break;
    case Color::Blue:
        value = "#0000ff";
        break;
    case Color::Green:
        value = "#00ff00";
        break;
    case Color::Black:
        value = "#000000";
        break;
    }
    return value;
}

quint16 TableSpace::generateIndex(QModelIndex& index)
{
    quint8 width = mm_rand.bounded(0, WIDTH_SPACE);
    quint8 height = mm_rand.bounded(0, HEIGHT_SPACE);

    index = this->index(width, height);

    auto index_list = width * mm_width + height;

    return index_list;
}

bool TableSpace::checkVert(const QModelIndex& index, const QString color)
{
    if (color == "#ffffff")
        return false;

    // номер строки
    auto col = index.column();
    QList<QModelIndex> list_index;

    for (auto i = 0; i < HEIGHT_SPACE; i++)
    {
        auto index_tmp = this->index(i,col);

        if (index_tmp.data() == color)
            list_index.append(index_tmp);
        else if (list_index.count() < 5) {
            list_index.clear();
        }
    }

    if (list_index.count() >= 5)
    {
        for (auto i = 0; i < list_index.count(); i++)
            this->setData(list_index[i], "#ffffff");

        mm_score +=100;
        emit changeScore(mm_score);
    }

    return false;
}
bool TableSpace::checkHor(const QModelIndex& index, const QString color)
{
    bool result_check = false;
    if (color == "#ffffff")
        return result_check;

    // номер строки
    auto row = index.row();
    QList<QModelIndex> list_index;

    for (auto i = 0; i < WIDTH_SPACE; i++)
    {
        auto index_tmp = this->index(row,i);

        if (index_tmp.data() == color)
            list_index.append(index_tmp);
        else if (list_index.count() < 5) {
            list_index.clear();
        }
    }

    if (list_index.count() >= 5)
    {
        result_check = true;

        for (auto i = 0; i < list_index.count(); i++)
            this->setData(list_index[i], "#ffffff");

        mm_score +=100;
        emit changeScore(mm_score);
    }

    return result_check;
}
