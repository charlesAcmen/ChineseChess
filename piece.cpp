#include "piece.h"

Piece::Piece() {

}



//return type by checking objectname
Piece::PieceType Piece::typeOfPiece(const QLabel* label){
    Piece::PieceType type = Piece::PieceType::Unknown;
    QString labelName = label->objectName();

    if(labelName.contains("Pawn",Qt::CaseInsensitive)){
        type = Piece::PieceType::Pawn;
    } else if (labelName.contains("Soilder",Qt::CaseInsensitive)) {
        type = Piece::PieceType::Soilder;
    } else if (labelName.contains("Cannon",Qt::CaseInsensitive)) {
        type = Piece::PieceType::Cannon;
    } else if (labelName.contains("Chariot",Qt::CaseInsensitive)) {
        type = Piece::PieceType::Chariot;
    } else if (labelName.contains("Horse",Qt::CaseInsensitive)) {
        type = Piece::PieceType::Horse;
    } else if (labelName.contains("Elephant",Qt::CaseInsensitive)) {
        type = Piece::PieceType::Elephant;
    } else if (labelName.contains("Bishop",Qt::CaseInsensitive)) {
        type = Piece::PieceType::Bishop;
    } else if (labelName.contains("Advisor",Qt::CaseInsensitive)) {
        type = Piece::PieceType::Advisor;
    } else if (labelName.contains("General",Qt::CaseInsensitive)) {
        type = Piece::PieceType::General;
    } else if (labelName.contains("Chief",Qt::CaseInsensitive)) {
        type = Piece::PieceType::Chief;
    } else {
        type = Piece::PieceType::Unknown;
    }
    return type;
}
//return side by checking object name of the piece label
Piece::Side Piece::sideOfPiece(const QLabel* label){
    Piece::Side type = Piece::Side::Red;
    QString labelName = label->objectName();
    if(labelName.contains("Red",Qt::CaseInsensitive)){
        type = Piece::Side::Red;
    }else{
        type = Piece::Side::Black;
    }
    return type;
}
Piece::Side Piece::sideOfPiece(const QString& name){
    Piece::Side type = Piece::Side::Red;
    if(name.contains("Red",Qt::CaseInsensitive)){
        type = Piece::Side::Red;
    }else{
        type = Piece::Side::Black;
    }
    return type;
}
