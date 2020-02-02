#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <QAbstractListModel>
#include <vector>
#include <QPropertyAnimation>
class Gameboard : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int dimension READ dimension CONSTANT)
    Q_PROPERTY(int hiddenElementValue READ  boardSize CONSTANT)
    Q_PROPERTY(int moveNumber MEMBER m_moveNumber NOTIFY moveNumberChanged)
    Q_PROPERTY(bool moveble MEMBER m_moveble NOTIFY movebleChanged)
    Q_PROPERTY(direction direct MEMBER m_direct NOTIFY directionChanged)


public:
    enum direction{
        STAND = 0, LEFT, RIGHT, UP, DOWN
    };
    Q_ENUM(direction)
    static constexpr size_t  defaultPuzzleDimension {4};
    Gameboard(const size_t boardDimension = defaultPuzzleDimension,
              QObject* parent = nullptr);

    struct Tile {
        size_t value {};
        Tile& operator=(const size_t newValue){
            value = newValue;
            return *this;
        }
        bool operator==(const size_t other){
            return other == value;
        }
    };
    int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    size_t dimension() const;
    size_t boardSize() const;
    Q_INVOKABLE bool move(int index);
    Q_INVOKABLE void setDirectionStand();
    using Position = std::pair<size_t, size_t>;
    bool isAdjacent(const Gameboard::Position f, const Gameboard::Position s);

signals:
    void moveNumberChanged(int moveNumber);
    void movebleChanged(bool moveble);
    void directionChanged(direction dir);
private:
    void shuffle();
    bool isPositionValid(const size_t position) const;
    bool isBoardValid() const;
    Position getRowCol(size_t index) const;
    direction m_direct = STAND;

    std::vector<Tile> m_rawBoard;
    const size_t m_dimension;
    int m_moveNumber = 0;
    bool m_moveble = false;
    const size_t m_boardSize;
};

#endif // GAMEBOARD_H
