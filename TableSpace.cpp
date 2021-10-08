#include "TableSpace.h"

TableSpace::TableSpace(QObject *parent)
{

    mm_score = 0;
    mm_width = WIDTH_SPACE;
    mm_height = HEIGHT_SPACE;
    mm_size = mm_width * mm_height;

    for(auto i = 0; i < mm_size; i++)
        mm_table.insert(i, "#ffffff");
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
        quint16 new_index_list = index.row() * mm_width + index.column();
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
    return true;
}

void TableSpace::step()
{
    qDebug() << "Step!!!!";

    QModelIndex index_model;

    for(auto i = 0; i < COUNT_BALL; i ++)
    {
        quint16 index_list = generateIndex(index_model);
        QString value = generateColor();

        if (mm_table.value(index_list) == "#ffffff")
        {
            mm_table[index_list] = value;
            emit dataChanged(index_model, index_model);
        }
        else
        {
            qDebug() << "Index fail: " << index_list << " Val: " << mm_table.value(index_list);
            i--;
        }
    }

    QList<QString> listVal = mm_table.values();

    if (!listVal.contains("#ffffff"))
    {
        emit gameEnd();
        return;
    }
    mm_score += 100;
    emit changeScore(mm_score);
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

    qDebug() << "Generated color:" << value;
    return value;
}

quint16 TableSpace::generateIndex(QModelIndex& index)
{
    quint8 width = mm_rand.bounded(0, WIDTH_SPACE);
    quint8 height = mm_rand.bounded(0, HEIGHT_SPACE);

    index = this->index(width, height);

    auto index_list = width * mm_width + height;

    qDebug() << "Generated Index:" << index_list;
    return index_list;
}

bool TableSpace::checkVert(const QModelIndex& index, const QString color)
{
    if (color == "#ffffff")
        return false;
//    QList<quint16> listNum;
    QList<QModelIndex*> list;
    QModelIndex* newIndex;

    // номер строки
    auto row = index.row();
    auto col = index.column();

    // ищем первый совпадающий элемент
    while (row >= 0) {
        auto index_tmp = this->index(row,col);

        if(index_tmp.data() == color)
            newIndex = &index_tmp;

        row--;
    }

    while (row < HEIGHT_SPACE) {

        if()
        list.append(newIndex);

        row++;
    }


//    // номер первого элемента столбца в списке
//    auto index_tmp = index - (row*HEIGHT_SPACE);
//    // количество совпадений
//    auto countColor = 0;
//    while(index_tmp <= mm_size)
//    {
//        if(mm_table.value(index_tmp) == color)
//        {
//            listNum.append(index_tmp);
//            countColor++;
//        }

//        index_tmp += WIDTH_SPACE;
//    }
//    if(countColor == 5)
//    {
//        bool checkNum = true;
//        for(auto i = 0; i < listNum.count()-1; i++)
//        {
//            if((listNum[i]+WIDTH_SPACE) != listNum[i+1])
//                checkNum = false;
//        }
//        if(checkNum)
//        {
//            qDebug() << "Win!";
//        }
//    }
    return false;
}
