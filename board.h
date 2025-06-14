#ifndef BOARD_H
#define BOARD_H

#include "boardstate.h"
#include "cchessrule.h"
#include "gamerecorder.h"
#include <qwidget.h>

#include <QEvent>

class Board:public QObject
{

    //judge the type of label
    //so far in this project there are pieces and empty space
    //potential future different label expected
    enum class Label{
        Piece,
        Placeholder
    };


public:

    Board(QWidget*,QVector<QLabel*>&);

    //return the label at the specified position if there is
    QLabel* labelAt(int column,int row);
    //return if the specified position
    bool isPiece(int column,int row);
    //return piece by checking if object name contains given string
    QVector<QLabel*> findByName(QString);

    //return index of the label
    int indexOfLabel(QLabel*);
    //return the coordinate of the piece in the board
    QPair<int,int> coordinateOfPiece(QLabel*);


protected:
    bool eventFilter(QObject*,QEvent*)override;

    //default board width over height ratio
    constexpr static double whRatio = 0.90629;
    //default horizontal ratio to board width for the gap from left side to first column
    constexpr static double hRatio=0.07064;
    //default vertical ratio to board height for the gap from top side to first row
    constexpr static double vRatio=0.06567;
    //default radius over length of side
    constexpr static double rlRatio = 0.47782;
    //current length of side
    double sideLength = 0.0;
    //current radius of piece
    double radius=0.0;

private:
    //reference to rule controller
    CChessRule* ruleController;
    //reference to game record controller
    GameRecorder* recordController;


    //reference to the labels and corresponding positions
    BoardState* boardState;
    //reference to pixmap of background image for board in the UI
    QPixmap* bgImage =NULL;



    //flag for judging if one piece is seleceted
    bool selected = false;
    //reference to the label being selected
    QLabel* selectedPieceLabel = NULL;
    //reference to the added empty labels during execution(happens when piece eat piece)
    QVector<QLabel*> addedEmptyLabels;
    //reference to the ate piece labels during execution
    QVector<QLabel*> atePieces;




    //calculate and update the size and relative position of all pieces in the Board
    void adjustPiecesPositions(const QSize&);
    //paint all elements within the board
    void paint();

    //return the label clicked including pieces and empty space label
    QLabel* getClickedLabel(QObject*);
    //do the work needed to response one label being selected successfully
    void PieceSelected(QLabel*);
    //do the work needed to response one label being unselected successfully
    void PieceUnselected();
    //move one piece to the desired grid point
    void movePiece(QLabel*,int column,int row);






    //find label by coordinate
    QLabel* findLabel(int column,int row);
    Label whichTypeLabel(QLabel*);



    //flag for judging if the BoardWidget is resizing
    bool resizing = false;

    inline static QPair<int,int> posIndexToCoordinate(int);
    inline static int coordinateToPosIndex(int column,int row);
    inline static int coordinateToPosIndex(QPair<int,int>);
};

#endif // BOARD_H
