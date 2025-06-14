#ifndef BOARDSTATE_H
#define BOARDSTATE_H

class Board;//forward declaration

#include "qlabel.h"
class BoardState
{
public:
    BoardState(Board*,QWidget*,QVector<QLabel*>&,QVector<int>&);
    QVector<int>& getPosition();
    QVector<QLabel*> &getLabels();
    QWidget* getBoardWidget();
    void save();
    void load();
private:
    //reference to the board
    Board* board;


    //reference to Board widgetin the UI
    QWidget* BoardWidget =NULL;
    //reference to labels in the UI including pieces and placeholders
    QVector<QLabel*> labels;
    //index arranged by rows of each pieces,starting from 0
    QVector<int> position;

    //reference to backup
    QVector<QLabel*> previousLabels;
    QVector<int> previousPosition;
};

#endif // BOARDSTATE_H
