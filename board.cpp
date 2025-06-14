#include "board.h"
#include "QPainter.h"
#include <QResizeEvent>
#include <QString>
//public:
Board::Board(QWidget* BoardWidget ,QVector<QLabel*>&labels) {
    this->ruleController = new CChessRule(this);

    QVector<int> position(90);
    //reserve 31 means potential eating action might need 31 more new empty labels
    // this->addedEmptyLabels.reserve(31);
    for (int i = 0; i < 90; i++) {
        position[i] = i;
        //note that whenever utilizing container class objects provided by the
        //library,use the functions instead of original new delete and index access methods
        // this->position.push_back(i);
    }
    BoardWidget->installEventFilter(this);
    for(QLabel* label:labels){
        if(whichTypeLabel(label) == Label::Placeholder){
            label->setText(" ");
        }
        label->installEventFilter(this);
    }
    this->boardState = new BoardState(this,BoardWidget,labels,position);
    this->bgImage = new QPixmap(":/image/display/board.png");
}
//retur NULL if it is not a piece at all
QLabel* Board::labelAt(int column,int row){
    return findLabel(column,row);
}
bool Board::isPiece(int column,int row){
    QLabel* label = findLabel(column,row);
    return whichTypeLabel(label) == Label::Piece;
}
QVector<QLabel*> Board::findByName(QString string){
    QVector<QLabel*> vector;
    QVector<QLabel*> labels = boardState->getLabels();
    for (QLabel* label : labels) {
        //pieces got ate before are replaced by newEmpty in the labels
        QString tmp = label->objectName().toLower();
        if (tmp.contains(string.toLower())) {
            vector.push_back(label);
        }
    }
    return vector;
}
//return the coordinate of the label in the board
QPair<int,int> Board::coordinateOfPiece(QLabel* label){
    return posIndexToCoordinate(
        boardState->getPosition().at(indexOfLabel(label)));
}


//protected:
bool Board::eventFilter(QObject* obj,QEvent* event){
    QWidget* BoardWidget = this->boardState->getBoardWidget();
    if (obj == BoardWidget && event->type() == QEvent::Resize) {
        //triggers after size were changed
        QResizeEvent *resizeEvent = dynamic_cast<QResizeEvent *>(event);
        QSize newSize = resizeEvent->size();
        adjustPiecesPositions(newSize);
        return true;  // 表示事件已经被处理
    }
    else if(obj == BoardWidget && event->type() == QEvent::Paint){
        //triggers after update() being called
        paint();
        return true;
    }
    else if(event->type() == QEvent::MouseButtonRelease){
        //triggers after mouse clicked
        //potential clicked label case
        QLabel* clickedLabel = getClickedLabel(obj);
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
        switch (mouseEvent->button()) {
            case Qt::LeftButton:
                if(clickedLabel==NULL){
                    //did not click any labels
                    //this if clause is marked as :done
                    if(selected){
                        //already selected a piece
                        //this if clause is marked as :done
                        PieceUnselected();
                    }
                    else{
                        //did not selected anything before and this time
                        //do nothing lol
                        //this else clause is marked as :done
                    }
                }else{
                    //did click one label

                    if(selected){
                        //already selected a piece
                        //go somewhere
                        //this if clause is marked as : done
                        int indexOfTargetLabel = indexOfLabel(clickedLabel);
                        int positionOfTargetLabel = boardState->getPosition().at(indexOfTargetLabel);
                        QPair<int,int> coordOfTarget= posIndexToCoordinate(positionOfTargetLabel);

                        if(this->ruleController->isRuleEnabled()){
                            //chinese chess game rule is enabled
                            int indexOfSelectedLabel = indexOfLabel(selectedPieceLabel);
                            int positionOfSelectedLabel = boardState->getPosition().at(indexOfSelectedLabel);
                            QPair<int,int> coordOfSelected= posIndexToCoordinate(positionOfSelectedLabel);
                            int startColumn = coordOfSelected.first;
                            int startRow = coordOfSelected.second;
                            bool flag =this->ruleController->CChessRule::checkMove(
                                selectedPieceLabel,startColumn,startRow,coordOfTarget.first,coordOfTarget.second);
                            if(!flag){
                                //illegal move ,do nothing
                                //remeber to unselect
                                PieceUnselected();
                            }else{
                                //legal move,proceed
                                //note that there is no need to swap turn of the game from here,
                                //because it is a legal move,rule controller has already swapped it in checkMove function

                                //legal move that complies with the rule,go to second part
                                //which is checking if this move will get one's own side general or chief being checked

                                //remeber to save the current state of the board before calling movePiece function
                                //in case potential reversing action is taken in the if clause below
                                this->boardState->save();
                                Piece::Side currentTurn = this->ruleController->getTurn();

                                CChessRule::Check checkFlag = this->ruleController->getCheckFlag();
                                //most important part
                                movePiece(selectedPieceLabel,coordOfTarget.first,coordOfTarget.second);
                                //flip the turn flag to exchange turn
                                this->ruleController->swapTurn();


                                //check if it is a check
                                this->ruleController->updateCheckFlag();

                                CChessRule::Check newCheckFlag = this->ruleController->getCheckFlag();
                                 if((checkFlag == CChessRule::Check::NotChecking && newCheckFlag == CChessRule::Check::BlackCheckRed && currentTurn == Piece::Side::Red)
                                    //can not get red checked after red moving
                                    ||(checkFlag == CChessRule::Check::NotChecking && newCheckFlag == CChessRule::Check::RedCheckBlack && currentTurn == Piece::Side::Black)
                                    //can not get black checked after black moving
                                    ||(checkFlag == CChessRule::Check::RedCheckBlack && newCheckFlag == CChessRule::Check::RedCheckBlack && currentTurn == Piece::Side::Black)
                                    //can not get black still checked after black moving
                                    ||(checkFlag == CChessRule::Check::BlackCheckRed && newCheckFlag == CChessRule::Check::BlackCheckRed && currentTurn == Piece::Side::Red)
                                    ||(this->ruleController->isGeneralAndChiefEncounter())){
                                    //can not get red still checked after red moving
                                    //illegal move
                                    this->boardState->load();
                                    QSize size = BoardWidget->size();
                                    adjustPiecesPositions(size);
                                    //flip the turn flag to exchange turn back
                                    this->ruleController->swapTurn();
                                }
                                //remeber to unselect
                                PieceUnselected();

                            }
                        }else{
                            //rule is not applied,then call move directly
                            //most important part
                            movePiece(selectedPieceLabel,coordOfTarget.first,coordOfTarget.second);
                            //remeber to unselect
                            PieceUnselected();
                        }
                    }
                    else{
                        //have not selected a piece before
                        //this else clause is marked as : done
                        //select this piece label
                        if(whichTypeLabel(clickedLabel) == Label::Placeholder){
                            //do nothing
                        }else PieceSelected(clickedLabel);
                    }
                }
                break;
            case Qt::RightButton:
                // qDebug() << "Right button pressed";
                if(obj==BoardWidget){
                    PieceUnselected();
                }

                break;
            default:
                // qDebug() << "Other button pressed";
                break;
        }
        // event->accept();
        //tag the event as already handled,but this will not prevent the event from
        //propogating upwards
        return true;
        //this will do
    }
    return QObject::eventFilter(obj, event);
}
//private:
void Board::adjustPiecesPositions(const QSize& newSize){
    if(resizing){
        //refuse to resize when already resizing to avoid infinte resizing recursion
        return;
    }

    //not resizing then go to resizing
    resizing = true;

    //calculate right width and height with aspectRatio preserved
    int width = newSize.width();
    int height = newSize.height();
    if (width > ((float)height) * whRatio) {
        width = ((float)height) * whRatio;
    } else {
        height = ((float)width) / whRatio;
    }

    //update the length of side
    sideLength = width*(1-2*hRatio)/8;
    //update the radius of piece
    radius = sideLength*rlRatio;

    QVector<QLabel*> labels = boardState->getLabels();
    for(int i = 0;i<labels.size();i++){
        //note that here it is position[i] rather than i itself being passed as a parameter
        QPair<int,int> coordinate = Board::posIndexToCoordinate(boardState->getPosition().at(i));
        if(whichTypeLabel(labels.at(i)) == Label::Placeholder){
            //empty space label
            labels.at(i)->setGeometry(
                width*hRatio+coordinate.first*sideLength-radius/2,
                height*vRatio+coordinate.second*sideLength-radius/2,
                radius*1.5,radius*1.5);
        }
        else{
            //pieces
            labels.at(i)->setGeometry(
                width*hRatio+coordinate.first*sideLength-radius,
                height*vRatio+coordinate.second*sideLength-radius,
                radius*2,radius*2);
        }
    }

    //do not forget to set status as not "resizing"
    resizing = false;
}
void Board::paint(){
    QWidget* BoardWidget = this->boardState->getBoardWidget();
    QPainter painter(BoardWidget);
    painter.setRenderHint(QPainter::Antialiasing);
    // 将图片缩放到QWidget的大小，保持宽高比
    QPixmap scaledPixmap = bgImage->scaled(
        BoardWidget->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation);
    painter.drawPixmap(0, 0, scaledPixmap);
}
QLabel* Board::getClickedLabel(QObject* obj){
    QLabel* clickedLabel = NULL;
    QVector<QLabel*> labels = boardState->getLabels();
    for(QLabel* label:labels){
        if(obj==label){
            //triggers after clicked the piece label
            qDebug()<<label->objectName();
            clickedLabel = label;
            break;
        }
    }
    return clickedLabel;
}
void Board::PieceSelected(QLabel* piece){
    selected = true;
    selectedPieceLabel = piece;
    QString currentStyle = selectedPieceLabel->styleSheet();
    selectedPieceLabel->setStyleSheet(currentStyle + "border: 3px solid red; border-radius: 10px;");
    // qDebug()<<"selected "<<piece->objectName();
}
void Board::PieceUnselected(){
    selected = false;
    if(selectedPieceLabel){
        QString currentStyle = selectedPieceLabel->styleSheet();
        QString newStyle = currentStyle;
        newStyle.remove("border: 3px solid red; border-radius: 10px;");
        selectedPieceLabel->setStyleSheet(newStyle);
        // qDebug()<<"unselected"<<selectedPieceLabel->objectName();
        selectedPieceLabel = NULL;
        return;
    }

    // qDebug()<<"unselected called,but no thing selected before";
}
//note that the first parameter called label is guaranteed to be a piece
//hence no more checking is needed,higher level checking takes place in event filter function
//this is for serious hierarchical programming
void Board::movePiece(QLabel* label,int column,int row){
    // index of moving label in the labels vector
    int labelIndex = indexOfLabel(label);
    //read state
    QVector<int>& position =  boardState->getPosition();
    QVector<QLabel*>& labels =  boardState->getLabels();
    QWidget* BoardWidget = this->boardState->getBoardWidget();


    // position[0] = 500;
    //position of moving label in the board
    int labelPos = position.at(labelIndex);

    //convert target coordinate to position in the board
    int targetPos = Board::coordinateToPosIndex(column,row);

    //start point = end point
    if(labelPos == targetPos) return;

    //find target position label by coordinate
    QLabel* targetLabel = findLabel(column,row);
    //index of target position label in the labels vector
    int indexOfTarget = indexOfLabel(targetLabel);

    //flag for whether it is piece eat piece case
    bool flag = false;
    switch(whichTypeLabel(targetLabel)){
        case Label::Piece:
            //eat
            //note that the implementation of chinese chess rule
            //has a higher level,which is the reason why we do not
            //do other checking here
            //error: Cannot jump from switch statement to this case label
            flag = true;
            break;
        case Label::Placeholder:
            //move
            break;
    }
    QLabel* newEmptyLabel = NULL;
    if(flag){
        //newEmpty label should substitute the label where
        //moving label has been
        newEmptyLabel = new QLabel(" ", BoardWidget);
        int suffix= addedEmptyLabels.size();
        newEmptyLabel->setObjectName("label_newEmpty" + QString::number(suffix));
        newEmptyLabel->installEventFilter(this);
        //don't forget to reserve one more element space
        addedEmptyLabels.reserve(addedEmptyLabels.size() + 1);
        addedEmptyLabels.push_back(newEmptyLabel);

        //significantly important
        //actually replacing action
        targetLabel->setParent(NULL);
        //record ate pieces,future usage to realize undo function
        atePieces.reserve(atePieces.size()+1);
        atePieces.push_back(targetLabel);

        //update labels
        labels.replace(indexOfTarget,newEmptyLabel);
        //very important,by default it will not show,fking hell
        newEmptyLabel->show();
    }
    //remeber to update the position of two labels correctly
    position[indexOfTarget] = labelPos;
    position[labelIndex] = targetPos;

    //move two labels
    QSize size = BoardWidget->size();
    adjustPiecesPositions(size);
    return;
}
int Board::indexOfLabel(QLabel* label){
    int index = 0 ;
    QVector<QLabel*> labels =  boardState->getLabels();
    for(QLabel* ilabel:labels){
        if(ilabel == label){
            break;
        }
        index++;
    }
    return index;
}
QLabel* Board::findLabel(int column,int row){
    int pos= coordinateToPosIndex(column,row);
    QVector<int> position =  boardState->getPosition();
    QVector<QLabel*> labels =  boardState->getLabels();
    for(int i = 0;i<position.size();i++){
        if(pos == position.at(i)){
            return labels.at(i);
        }
    }
    return NULL;
}

Board::Label Board::whichTypeLabel(QLabel* label){
    if(label->objectName().startsWith("label")){
        return Label::Placeholder;
    }
    else return Label::Piece;
}


inline QPair<int,int> Board:: posIndexToCoordinate(int index){
    if(index<0 || index>89) return QPair<int,int>(-1, -1);
    else{
        int row = index/9;
        int column = index%9;
        return QPair<int,int>(column,row);
    }
}
inline int Board::coordinateToPosIndex(int column,int row){
    return row*9+column;
}
inline int Board::coordinateToPosIndex(QPair<int,int> coordinate){
    return Board::coordinateToPosIndex(coordinate.first,coordinate.second);
}
