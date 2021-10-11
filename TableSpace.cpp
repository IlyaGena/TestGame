#include "TableSpace.h"

TableSpace::TableSpace(QObject *parent) :
    QAbstractTableModel(parent),
    mm_score(0),
    mm_rand(),
    mm_db(this)
{
    // инициализация значений
    mm_score = 0;
    mm_width = WIDTH_SPACE;
    mm_height = HEIGHT_SPACE;
    mm_size = mm_width * mm_height;

    // получение данных из БД
    auto table = mm_db.getData();

    if (!table.isEmpty())
    {
        // заполнение данными из БД
        mm_score = mm_db.getScore();
        mm_table = table;

        // удаление старых данных в БД
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

    // сохранение данных в БД
    mm_db.saveData(mm_table, mm_score);
    mm_db.close();
}

int TableSpace::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return HEIGHT_SPACE;
}

int TableSpace::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return WIDTH_SPACE;
}

QVariant TableSpace::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    {
        //получение цвета и его возврат
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
    // вычестяем идентификатор изменяемого индекса в таблице mm_table
    quint16 new_index_list = new_index.row() * mm_width + new_index.column();
    // конвертируем в QString новое значение
    QString color = value.toString();
    // сохраняем в массив
    mm_table[new_index_list] = value.toString();
    // оповещаем GUI о изменении ячейки
    emit dataChanged(new_index, new_index);
    // проверяем по вертикали и горизонтали
    checkVert(new_index, color);
    checkHor(new_index, color);
    return true;
}

void TableSpace::step()
{
    QModelIndex index_model;

    // необходиомое количество генерируемы шариков
    quint16 count_new_ball = COUNT_BALL;

    QList<quint16> keys_white = mm_table.keys("#ffffff");

    // если нужно сгенерировать меньше 3-х, меняем
    if (count_new_ball > keys_white.count())
        count_new_ball = keys_white.count();

    for(auto i = 0; i < count_new_ball; i ++)
    {
        // генерируем индекс и цвета
        quint16 index_list = generateIndex(index_model);
        QString value = generateColor();

        // пероверяем на заполняемость данного индекса(если не пустой то декримент)
        if (mm_table.value(index_list) == "#ffffff")
        {
            // сохраняем в массив
            mm_table[index_list] = value;
            // оповещаем GUI о изменении ячейки
            emit dataChanged(index_model, index_model);

            // проверяем по вертикали и горизонтали
            checkVert(index_model, value);
            checkHor(index_model, value);
        }
        else
            i--;
    }

    // проверка на конец игры
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
    // переинициализируем массив mm_table
    for(auto i = 0; i < mm_size; i++)
        mm_table[i] = "#ffffff";

    // индекс начала и конца
    auto index_start = this->index(0, 0);
    auto index_end = this->index(WIDTH_SPACE-1, HEIGHT_SPACE-1);

    // оповещаем GUI о изменении ячеек
    emit dataChanged(index_start, index_end);

    // оповещаем GUI о изменении счета
    mm_score = 0;
    emit changeScore();
    emit newGameStart();
}

QString TableSpace::getValue(const QModelIndex &index) const
{
    QString value = mm_table.value(index.row() * mm_width + index.column());
    return value;
}

QString TableSpace::generateColor()
{
    // генерирование цвета
    Color color = Color(mm_rand.generate() % 4);

    // выбираем цвет
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
    // генерируем высоту и ширину
    quint8 width = mm_rand.bounded(0, WIDTH_SPACE);
    quint8 height = mm_rand.bounded(0, HEIGHT_SPACE);

    // получаем индекс и изменяем
    index = this->index(width, height);

    // получаем индекс в массиве mm_table
    auto index_list = width * mm_width + height;

    return index_list;
}

bool TableSpace::checkVert(const QModelIndex& index, const QString color)
{
    // если цвет для проверки = белому
    if (color == "#ffffff")
        return false;

    // номер строки
    auto col = index.column();
    // список ячеек
    QList<QModelIndex> list_index;

    for (auto i = 0; i < HEIGHT_SPACE; i++)
    {
        auto index_tmp = this->index(i,col);

        // в случае совпадения - добавляем в массив ячеек
        if (index_tmp.data() == color)
            list_index.append(index_tmp);
        // если не совпадает цвет и размер массива меньше 5
        else if (list_index.count() < 5) {
            list_index.clear();
        }
    }

    // если массив больше 5
    if (list_index.count() >= 5)
    {
        // меняем на белый
        for (auto i = 0; i < list_index.count(); i++)
            this->setData(list_index[i], "#ffffff");

        // увеличиваем счет
        mm_score +=100;
        emit changeScore();
    }

    return false;
}

// по аналогии с вертикалью
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
        emit changeScore();
    }

    return result_check;
}
