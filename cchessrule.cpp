#include "cchessrule.h"
#include "board.h"

CChessRule::CChessRule(Board *board, bool enabled,CChessRule::Check checkFlag):
    isEnabeld(enabled),turn(Piece::Side::Red),checkFlag(checkFlag){
    this->board = board;
}
bool CChessRule::isRuleEnabled() const{
    return this->isEnabeld;
}
void CChessRule::enableRule(){
    this->isEnabeld = true;
}
void CChessRule::disableRule(){
    this->isEnabeld = false;
}
//do nothing but check if the move it self complies with the rule,for instance,horse moves in a manner of 日
//parameter:label is guarenteed to be a piece label not placeholder
bool CChessRule::checkMove(
    QLabel* label,int startColumn,int startRow,int targetColumn,int targetRow){
    if(startColumn == targetColumn && startRow == targetRow) return false;
    Piece::PieceType labelType = Piece::typeOfPiece(label);
    Piece::Side labelSide = Piece::sideOfPiece(label);


    Piece::Side targetSide = Piece::Side::Red;
    //flag for whether there is a piece in the target position
    bool flagHasPiece = board->isPiece(targetColumn,targetRow);
    if(flagHasPiece){
        targetSide = Piece::sideOfPiece(board->labelAt(targetColumn,targetRow));
    }
    if(this->turn != labelSide)
        //its not your turn yet!
        return false;

    if(flagHasPiece){
        if(labelSide == targetSide)
            //can not eat pieces from the same side,nah
            return false;
    }




    //it is your turn and the potential target piece is from your opposite side

    //flag for feasibility of the move
    bool flag = true;
    //by default red is the one beneath,black is the one on top on the board
    //converting work is needed when the board is upside down,but not here

    //first check if start point is correct
    if(startRow>=0 && startRow<=9 && startColumn>=0 && startColumn <=8
          && targetRow>=0 && targetRow<=9 && targetColumn>=0 && targetColumn <=8){
        switch (labelType) {
        case Piece::PieceType::Pawn:
            //you can literally eat anyone,so no additional piece type checking is needed here

            if(startRow >=3 && startRow<=4){
                //idk why i have to write <= >= here lol,but yeah let's just leave it
                if(startColumn == 1 || startColumn==3 ||startColumn==5||startColumn==7){
                    flag = false;
                }
                else{
                    //only moving forward is allowed
                    flag =(targetColumn == startColumn && startRow+1 == targetRow);
                }
            }else{
                //已上岸
                if(startColumn == targetColumn){
                    flag = (startRow+1==targetRow);
                }else if(startRow == targetRow){
                    flag = (startColumn+1==targetColumn || startColumn-1==targetColumn);
                }else{
                    flag = false;
                }
            }
            break;
        case Piece::PieceType::Soilder:
            //you can literally eat anyone,so no additional piece type checking is needed here
            //bruh this is tedious work

            if(startRow<=6 && startRow>=5){
                if(startColumn == 1 || startColumn==3 ||startColumn==5||startColumn==7){
                    flag = false;
                }else{
                    //only moving forward is allowed
                    flag = (targetColumn == startColumn && startRow-1 == targetRow);
                }
            }else{
                //已上岸
                if(startColumn == targetColumn){
                    flag = (startRow-1==targetRow);
                }else if(startRow == targetRow){
                    flag = (startColumn+1==targetColumn || startColumn-1==targetColumn);
                }else{
                    flag = false;
                }
            }

            break;
        case Piece::PieceType::Cannon:
            if(startColumn == targetColumn || startRow == targetRow){
                //at lease in the same line
                QVector<QLabel*> labels =
                    blockingPieces(startColumn,startRow,targetColumn,targetRow);
                //nothing blocking the way
                //at least one piece blocking the way
                if(labels.isEmpty() && !flagHasPiece){
                    flag=true;
                }else{
                    flag = (labels.size() == 1 && flagHasPiece);
                    //very important part of cannon moving criteria:there is only one piece
                    //separating start piece(cannon) and target piece
                }
            }else{
                flag=false;
            }
            break;
        case Piece::PieceType::Chariot:
            if(startColumn == targetColumn || startRow == targetRow){
                //at lease in the same line
                QVector<QLabel*> labels =
                    blockingPieces(startColumn,startRow,targetColumn,targetRow);
                //nothing blocking the way
                //at least one piece blocking the way
                flag = labels.isEmpty();
            }else{
                flag=false;
            }
            break;
        case Piece::PieceType::Horse:
            if(startColumn>=0 && startColumn<=8 && startRow>=0 && startRow<=9 &&
                targetColumn>=0 && targetColumn<=8 && targetRow>=0 && targetRow<=9){


                // calculate horizontal and vertical distance
                int colDiff = abs(targetColumn - startColumn);
                int rowDiff = abs(targetRow - startRow);

                // 马走“日”
                if ((colDiff == 2 && rowDiff == 1) || (colDiff == 1 && rowDiff == 2)) {
                    // judge if there is pieces blocking
                    if (colDiff == 2) {
                        //check horizontally
                        int midCol = (startColumn + targetColumn) / 2;
                        if (board->isPiece(midCol, startRow)) {
                            flag = false;
                        }
                    }
                    else if (rowDiff == 2) {
                        // check vertically
                        int midRow = (startRow + targetRow) / 2;
                        if (board->isPiece(startColumn, midRow)) {
                            flag = false;
                        }
                    }else flag = true;
                }else flag = false;// 不是legal的“日”字形
            }else flag = false;
            break;
        case Piece::PieceType::Elephant:
            if(startColumn>=0 && startColumn<=8 && startRow>=0 && startRow<=4 &&
                targetColumn>=0 && targetColumn<=8 && targetRow>=0 && targetRow<=4){
                if((abs(targetColumn - startColumn) == 2 && abs(targetRow - startRow) == 2)){
                    //now judge if there is some label in the middle(卡象眼）
                    int midColumn = (targetColumn + startColumn)/2;
                    int midRow = (targetRow + startRow)/2;
                    flag = !(board->isPiece(midColumn,midRow));
                }else flag = false;
            }else flag = false;
            //black
            break;
        case Piece::PieceType::Bishop:
            //red
            if(startColumn>=0 && startColumn<=8 && startRow>=5 && startRow<=9 &&
                targetColumn>=0 && targetColumn<=8 && targetRow>=5 && targetRow<=9){
                if((abs(targetColumn - startColumn) == 2 && abs(targetRow - startRow) == 2)){
                    //now judge if there is some label in the middle(卡象眼）
                    //note that the round down of integer here,it is different from elephant,which always has
                    //even result
                    int midColumn = (targetColumn + startColumn)/2;
                    int midRow = (targetRow + startRow)/2;
                    flag = !(board->isPiece(midColumn,midRow));
                }else flag = false;
            }else flag = false;
            break;
        case Piece::PieceType::Advisor:
            switch(labelSide){
                case Piece::Side::Red:
                    if(startColumn>=3 && startColumn<=5 && startRow>=7 && startRow<=9 &&
                        targetColumn>=3 && targetColumn<=5 && targetRow>=7 && targetRow<=9){
                        flag = (abs(targetColumn - startColumn) == 1 && abs(targetRow - startRow) == 1);
                    }else flag= false;
                    break;
                case Piece::Side::Black:
                    if(startColumn>=3 && startColumn<=5 && startRow>=0 && startRow<=2 &&
                        targetColumn>=3 && targetColumn<=5 && targetRow>=0 && targetRow<=2){
                        flag = (abs(targetColumn - startColumn) == 1 && abs(targetRow - startRow) == 1);
                    }else flag= false;
                    break;
            }
            break;
        case Piece::PieceType::General:
            //keep in mind the rule that general and chief can not face to face with each other
            if(startColumn>=3 && startColumn<=5 && startRow>=0 && startRow<=2 &&
                targetColumn>=3 && targetColumn<=5 && targetRow>=0 && targetRow<=2){
                int distance =
                    abs(startColumn-targetColumn) + abs(startRow-targetRow);
                flag = (distance == 1);//adjacent move,which is legal
            }else flag= false;
            break;
        case Piece::PieceType::Chief:
            //keep in mind the rule that general and chief can not face to face with each other
            if(startColumn>=3 && startColumn<=5 && startRow>=7 && startRow<=9 &&
                targetColumn>=3 && targetColumn<=5 && targetRow>=7 && targetRow<=9){
                int distance =
                    abs(startColumn-targetColumn) + abs(startRow-targetRow);
                flag = (distance == 1);
            }else flag= false;
            break;
        case Piece::PieceType::Unknown:
            break;
        }
    }
    return flag;
}

bool CChessRule::isGeneralAndChiefEncounter(){
    if(this->general == NULL) general = getGeneral();
    if(this->chief == NULL) chief = getChief();
    QPair<int,int> generalPos = board->coordinateOfPiece(general);
    QPair<int,int> chiefPos = board->coordinateOfPiece(chief);
    QVector<QLabel*> labels =
        blockingPieces(chiefPos.first,chiefPos.second,generalPos.first,generalPos.second);
    return labels.isEmpty();
}
CChessRule::Check CChessRule::getCheckFlag(){
    return checkFlag;
}



//do the updating work,and signal if necessary
//(for example send signal to sound controller to trigger check sound or checkmate sound)
void CChessRule::updateCheckFlag(){
    if(this->general == NULL) general = getGeneral();
    if(this->chief == NULL) chief = getChief();
    //other pieces may have been removed from the board during the game
    //which means update vectors is necessary
    pawns = getPawns();
    soilders = getSoilders();
    blackChariots = getBlackChariots();
    redChariots = getRedChariots();
    blackHorses = getBlackHorses();
    redHorses = getRedHorses();
    blackCannons = getBlackCannons();
    redCannons = getRedCannons();

    //do not forget to clear the checking pieces up
    redCheckPieces.clear();
    redCheckPieces.squeeze();
    blackCheckPieces.clear();
    blackCheckPieces.squeeze();


    QPair<int,int> generalPos = board->coordinateOfPiece(general);
    QPair<int,int> chiefPos = board->coordinateOfPiece(chief);



    //check if it is RedCheckBlack
    //soilders
     for(int i = 0;i<soilders.size();i++){
        QPair<int,int> soilderPos = board->coordinateOfPiece(soilders.at(i));
        // soilder can only check the general by horizontal or vertical direction in adjacent block
        if ((soilderPos.first == generalPos.first && soilderPos.second - generalPos.second == 1)
            || //soilder and general in the same column and adjacent
            (soilderPos.second == generalPos.second && abs(soilderPos.first - generalPos.first) == 1)) {
            // soilder and general in the same row and adjacent
            checkFlag = CChessRule::Check::RedCheckBlack;
            redCheckPieces.push_back(soilders.at(i));
            // return;
        }
    }

    //chariots
    for(int i = 0;i<redChariots.size();i++){
        QPair<int,int> chariotPos = board->coordinateOfPiece(redChariots.at(i));
        if (chariotPos.first == generalPos.first ||  chariotPos.second==generalPos.second) {
            QVector<QLabel*> blocking = blockingPieces(
                chariotPos.first,chariotPos.second,generalPos.first,generalPos.second);
            if(blocking.isEmpty()){
                checkFlag = CChessRule::Check::RedCheckBlack;
                redCheckPieces.push_back(redChariots.at(i));
                // return;
            }
        }
    }

    //cannons
    for(int i = 0;i<redCannons.size();i++){
        QPair<int,int> cannonPos = board->coordinateOfPiece(redCannons.at(i));
        if (cannonPos.first == generalPos.first ||  cannonPos.second==generalPos.second) {
            QVector<QLabel*> blocking = blockingPieces(
                cannonPos.first,cannonPos.second,generalPos.first,generalPos.second);
             if(blocking.size() == 1){
                checkFlag = CChessRule::Check::RedCheckBlack;
                redCheckPieces.push_back(redCannons.at(i));
                // return;
            }
        }

    }

    //horses
    for(int i = 0;i<redHorses.size();i++){
        QPair<int,int> horsePos = board->coordinateOfPiece(redHorses.at(i));
        // calculate horizontal and vertical distance
        int colDiff = abs(generalPos.first - horsePos.first);
        int rowDiff = abs(generalPos.second - horsePos.second);

        // 马走“日”
        if ((colDiff == 2 && rowDiff == 1) || (colDiff == 1 && rowDiff == 2)) {
            // judge if there is pieces blocking
            if (colDiff == 2) {
                //check horizontally
                int midCol = (horsePos.first + generalPos.first) / 2;
                if (!board->isPiece(midCol,horsePos.first)) {
                    checkFlag = CChessRule::Check::RedCheckBlack;
                    redCheckPieces.push_back(redHorses.at(i));
                    // return;
                }
            }
            else if (rowDiff == 2) {
                // check vertically
                int midRow = (horsePos.second + generalPos.second) / 2;
                if (!board->isPiece(horsePos.second, midRow)) {
                    checkFlag = CChessRule::Check::RedCheckBlack;
                    redCheckPieces.push_back(redHorses.at(i));
                    // return;
                }
            }
        }
    }

    //check if it is BlackCheckRed
    //pawns
    for(int i = 0;i<pawns.size();i++){
        QPair<int,int> pawnPos = board->coordinateOfPiece(pawns.at(i));
        // pawn can only check the chief by horizontal or vertical direction in adjacent block
        if ((pawnPos.first == chiefPos.first && pawnPos.second - chiefPos.second == -1)
            || //pawn and chief in the same column and adjacent
            (pawnPos.second == chiefPos.second && abs(pawnPos.first - chiefPos.first) == 1)) {
            //pawn and chief in the same row and adjacent
            checkFlag = CChessRule::Check::BlackCheckRed;
            blackCheckPieces.push_back(pawns.at(i));
            // return;
        }
    }

    //chariots
    for(int i = 0;i<blackChariots.size();i++){
        QPair<int,int> chariotPos = board->coordinateOfPiece(blackChariots.at(i));
        if (chariotPos.first == chiefPos.first ||  chariotPos.second==chiefPos.second) {
            QVector<QLabel*> blocking = blockingPieces(
                chariotPos.first,chariotPos.second,chiefPos.first,chiefPos.second);
            if(blocking.isEmpty()){
                checkFlag = CChessRule::Check::BlackCheckRed;
                blackCheckPieces.push_back(blackChariots.at(i));
                // return;
            }
        }
    }

    //cannons
    for(int i = 0;i<blackCannons.size();i++){
        QPair<int,int> cannonPos = board->coordinateOfPiece(blackCannons.at(i));
        if (cannonPos.first == chiefPos.first ||  cannonPos.second== chiefPos.second) {
            QVector<QLabel*> blocking = blockingPieces(
                cannonPos.first,cannonPos.second,chiefPos.first,chiefPos.second);
            if(blocking.size() == 1){
                checkFlag = CChessRule::Check::BlackCheckRed;
                blackCheckPieces.push_back(blackCannons.at(i));
                // return;
            }
        }

    }

    //horses
    for(int i = 0;i<blackHorses.size();i++){
        QPair<int,int> horsePos = board->coordinateOfPiece(blackHorses.at(i));
        // calculate horizontal and vertical distance
        int colDiff = abs(chiefPos.first - horsePos.first);
        int rowDiff = abs(chiefPos.second - horsePos.second);

        // 马走“日”
        if ((colDiff == 2 && rowDiff == 1) || (colDiff == 1 && rowDiff == 2)) {
            // judge if there is pieces blocking
            if (colDiff == 2) {
                //check horizontally
                int midCol = (horsePos.first + chiefPos.first) / 2;
                if (!board->isPiece(midCol,horsePos.first)) {
                    checkFlag = CChessRule::Check::BlackCheckRed;
                    blackCheckPieces.push_back(blackHorses.at(i));
                    // return;
                }
            }
            else if (rowDiff == 2) {
                // check vertically
                int midRow = (horsePos.second + chiefPos.second) / 2;
                if (!board->isPiece(horsePos.second, midRow)) {
                    checkFlag = CChessRule::Check::BlackCheckRed;
                    blackCheckPieces.push_back(blackHorses.at(i));
                    // return;
                }
            }
        }
    }


    for (QLabel* redPiece : redCheckPieces) {
        qDebug() << redPiece->objectName() <<" checking black";
    }
    for (QLabel* blackPiece : blackCheckPieces) {
        qDebug() << blackPiece->objectName() <<" checking red";
    }
    if(redCheckPieces.isEmpty() && blackCheckPieces.isEmpty())
        //after checking both sides,check flag now can be set as NotChecking safely from here
        checkFlag=CChessRule::Check::NotChecking;
}

//private function
QLabel* CChessRule::getGeneral(){
    QVector<QLabel*> vector = board->findByName("General");
    return vector.at(0);
}
QLabel* CChessRule::getChief(){
    QVector<QLabel*> vector = board->findByName("Chief");
    return vector.at(0);
}
QVector<QLabel*> CChessRule::getPawns(){
    QVector<QLabel*> vector = board->findByName("Pawn");
    return vector;
}
QVector<QLabel*> CChessRule::getSoilders(){
    QVector<QLabel*> vector = board->findByName("Soilder");
    return vector;
}
QVector<QLabel*> CChessRule::getBlackChariots(){
    QVector<QLabel*> vector = board->findByName("Black_Chariot");
    return vector;
}
QVector<QLabel*> CChessRule::getRedChariots(){
    QVector<QLabel*> vector = board->findByName("Red_Chariot");
    return vector;
}
QVector<QLabel*> CChessRule::getBlackHorses(){
    QVector<QLabel*> vector = board->findByName("Black_Horse");
    return vector;
}
QVector<QLabel*> CChessRule::getRedHorses(){
    QVector<QLabel*> vector = board->findByName("Red_Horse");
    return vector;
}
QVector<QLabel*> CChessRule::getBlackCannons(){
    QVector<QLabel*> vector = board->findByName("Black_Cannon");
    return vector;
}
QVector<QLabel*> CChessRule::getRedCannons(){
    QVector<QLabel*> vector = board->findByName("Red_Cannon");
    return vector;
}

QVector<QLabel*> CChessRule::getBlackAdvisors(){
    QVector<QLabel*> vector = board->findByName("Black_Advisor");
    return vector;
}

QVector<QLabel*> CChessRule::getRedAdvisors(){
    QVector<QLabel*> vector = board->findByName("Red_Advisor");
    return vector;
}

QVector<QLabel*> CChessRule::getElephants(){
    QVector<QLabel*> vector = board->findByName("Elephant");
    return vector;
}

QVector<QLabel*> CChessRule::getBishops(){
    QVector<QLabel*> vector = board->findByName("Bishop");
    return vector;
}


//note that the start point and target point are guaranteed to be in the same line
QVector<QLabel*> CChessRule::blockingPieces(
    int startColumn,int startRow,int targetColumn,int targetRow){
    QVector<QLabel*> labels;
    if(startColumn == targetColumn){
        //same column,which means vertically inline with each other
        int column = startColumn;
        //used in for loop to determine checking sequence
        int orientation = startRow<targetRow?1:-1;
        if(orientation > 0){
            //from up to below
            for(int i = startRow + orientation;i<targetRow;i+=orientation){
                if(board->isPiece(column,i)){
                    //add to the list
                    labels.push_back(board->labelAt(column,i));
                }
            }
        }else{
            //from below to up
            for(int i = startRow + orientation;i>targetRow;i+=orientation){
                if(board->isPiece(column,i)){
                    //add to the list
                    labels.push_back(board->labelAt(column,i));
                }
            }
        }
    }else if(startRow == targetRow){
        //same row,which means horizontally inline with each other
        int row = startRow;
        //used in for loop to determine checking sequence
        int orientation = startColumn<targetColumn?1:-1;
        if(orientation > 0){
            //from left to right
            for(int i = startColumn + orientation;i<targetColumn;i+=orientation){
                if(board->isPiece(i,row)){
                    //add to the list
                    labels.push_back(board->labelAt(i,row));
                }
            }
        }else{
            //from right to left
            for(int i = startColumn + orientation;i>targetColumn;i+=orientation){
                if(board->isPiece(i,row)){
                    //add to the list
                    labels.push_back(board->labelAt(i,row));
                }
            }
        }
    }else{
        //not same line
        labels.push_back(new QLabel());
    }
    return labels;
}

Piece::Side CChessRule::getTurn(){
    return this->turn;
}
void CChessRule::swapTurn(){
    this->turn = (this->turn == Piece::Side::Red)?Piece::Side::Black:Piece::Side::Red;
}
