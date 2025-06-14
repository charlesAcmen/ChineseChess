#include "boardstate.h"

BoardState::BoardState(Board* board,QWidget* BoardWidget,QVector<QLabel*>& labels,QVector<int>& position):
    board(board),BoardWidget(BoardWidget),labels(labels),position(position){

    save();
}
void BoardState::save(){
    this->previousLabels = this->labels;
    this->previousPosition = this->position;
}
void BoardState::load(){
    for(QLabel* label:labels){
        label->setParent(NULL);
        // qDebug()<<"label: "<<label;
    }
    this->labels = this->previousLabels;
    this->position = this->previousPosition;
    // qDebug()<<"Board: "<<BoardWidget;

    for(QLabel* label:labels){
        label->setParent(BoardWidget);
        label->setVisible(true);
        // qDebug()<<"label: "<<label;
    }
}

QVector<int>& BoardState::getPosition(){
    return this->position;
}
QVector<QLabel*>& BoardState::getLabels(){
    return this->labels;
}

QWidget* BoardState::getBoardWidget(){
    return this->BoardWidget;
}

