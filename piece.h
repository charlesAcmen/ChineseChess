#ifndef PIECE_H
#define PIECE_H

#include "qlabel.h"
class Piece
{

public:
    //note that it is different from the piece in the enumeration Label
    enum class PieceType{
        Pawn,
        Soilder,
        Cannon,
        Chariot,
        Horse,
        Elephant,
        Bishop,
        Advisor,
        General,
        Chief,
        Unknown
    };
    enum class Side{
        Red,
        Black
    };
    Piece();

    //use enum tag to refer to type "Piece" in this scope
    static Piece::PieceType typeOfPiece(const QLabel*);
    //return the side the piece belongs to
    static Piece::Side sideOfPiece(const QLabel*);
    static Piece::Side sideOfPiece(const QString&);
};

#endif // PIECE_H
