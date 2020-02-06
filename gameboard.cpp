#include "gameboard.h"
#include <algorithm>
#include <random>
#include<QDebug>
#include <windows.h>

Gameboard::Gameboard(const size_t boardDimension,
    QObject* parent):QAbstractListModel{parent},
    m_dimension{boardDimension},
    m_boardSize{m_dimension*m_dimension}
{
    m_rawBoard.resize(m_boardSize);
    std::iota(m_rawBoard.begin(), m_rawBoard.end(),1);
    shuffle();

}

int Gameboard::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rawBoard.size();
}

QVariant Gameboard::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role != Qt::DisplayRole){
        return {};
    }
    const int rowIndex {index.row()};
    return QVariant::fromValue(m_rawBoard[rowIndex].value);

}

void Gameboard::shuffle()
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 generator(seed);
    do{
    std::shuffle(m_rawBoard.begin(), m_rawBoard.end(), generator);
    }while(!isBoardValid());
}

bool Gameboard::isPositionValid(const size_t position) const
{
    return position < m_boardSize;
}

bool Gameboard::isBoardValid() const
{
    int inv{0};
    for(size_t i {0}; i < m_boardSize; ++i){
        for(size_t j = 0; j < i; ++j){
            if(m_rawBoard[j].value > m_rawBoard[i].value){
                ++inv;
            }
        }
    }
    const size_t start_point = 1;
    for(size_t i = 0; i < m_boardSize; ++i){
        if(m_rawBoard[i].value == m_boardSize){
            inv += start_point + i / m_dimension;
        }
    }
    return (inv % 2) == 0;
}

Gameboard::Position Gameboard::getRowCol(const size_t index) const
{
    Q_ASSERT(m_dimension > 0);
    size_t row = index / m_dimension;
    size_t column = index % m_dimension;

    return std::make_pair(row, column);
}


size_t Gameboard::boardSize() const
{
    return m_boardSize;
}


bool Gameboard::move(int index)
{
    if(!isPositionValid(static_cast<size_t>(index))){
        return false;
    }
    const Position elementPosition {getRowCol(index)};
    auto hiddenElementIterator = std::find(m_rawBoard.begin(), m_rawBoard.end(), boardSize());
    Q_ASSERT(hiddenElementIterator != m_rawBoard.end());
    Position hiddenElementPosition {getRowCol(std::distance(m_rawBoard.begin(),hiddenElementIterator))};

    if(!isAdjacent(elementPosition, hiddenElementPosition)){
        return false;
    }
    int boardIndex = 0;
    std::swap(hiddenElementIterator->value, m_rawBoard[index].value);
    moveNumberChanged(++m_moveNumber);
    while(boardIndex!=static_cast<int>(m_rawBoard.size()) && (boardIndex == static_cast<int>(m_rawBoard[boardIndex].value))){
           boardIndex++;
           if(boardIndex == static_cast<int>(m_rawBoard.size()-1)){
               QMessageBox msgBox;
               msgBox.setText("You are win.");
               msgBox.setStandardButtons(QMessageBox::Ok);
           }
    }
    return true;
}

void Gameboard::setDirectionStand()
{
    directionChanged(m_direct = STAND);
}

bool Gameboard::isAdjacent(const Gameboard::Position f, const Gameboard::Position s)
{
    if(f == s){
       movebleChanged(m_moveble = false);
       directionChanged(m_direct = STAND);
       qDebug() << "something else";
       return false;
    }
    const auto calcDistance = [](const size_t pos1, size_t pos2){
    int distance = static_cast<int>(pos1);
    distance -= static_cast<int>(pos2);
    distance = std::abs(distance);
    return distance;
    };

    const auto calcDirection = [](const size_t pos1, size_t pos2){
    int dir = static_cast<int>(pos1);
    dir -= static_cast<int>(pos2);
    return dir;
    };


    bool result {false};
    if(f.first == s.first){
        int distance = calcDistance(f.second, s.second);
        if(distance == 1){
            int dir = calcDirection(f.second, s.second);
            if(dir == -1){
                directionChanged(m_direct = RIGHT);
            } else if (dir == 1){
              directionChanged(m_direct = LEFT);
            }
            result = true;
        }
    }else if (f.second == s.second){
        int distance = calcDistance(f.first, s.first);
        if(distance == 1){
            int dir = calcDirection(f.first, s.first);
            if(dir == -1){
                directionChanged(m_direct = DOWN);
            } else if (dir == 1){
              directionChanged(m_direct = UP);
            }
            result = true;

        }
    }
    if(!result){
        directionChanged(m_direct = STAND);
    }
    qDebug() << result;
    movebleChanged(m_moveble = result);
    return result;
}


size_t Gameboard::dimension() const
{
    return m_dimension;
}
