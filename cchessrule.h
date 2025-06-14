#ifndef CCHESSRULE_H
#define CCHESSRULE_H

class Board;//forward declaration

#include "qlabel.h"
#include "piece.h"
class CChessRule
{


public:
    enum class Check{
        RedCheckBlack,
        BlackCheckRed,
        NotChecking
    };
    CChessRule(Board* board,bool = true,CChessRule::Check checkFlag = CChessRule::Check::NotChecking);
                 bool isRuleEnabled() const;
    void enableRule();
    void disableRule();
    //parameter starts from zero
    bool checkMove(QLabel*,int startColumn,int startRow,
        int targetColumn,int targetRow);
    //check if general and chief is in the state of face to face
    bool isGeneralAndChiefEncounter();
    //return current state of the board
    CChessRule::Check getCheckFlag();
    //update the checkFlag variable by checking the current board state
    void updateCheckFlag();
    Piece::Side getTurn();
    void swapTurn();
private:
    //flag for rule applied to the game
    bool isEnabeld;
    //corresponding board
    Board* board;
    //whose turn currently
    Piece::Side turn;
    //flag for whether it is checking
    CChessRule::Check checkFlag;
    //checking general pieces for red
    QVector<QLabel*> redCheckPieces;
    //chechking chief pieces for black
    QVector<QLabel*> blackCheckPieces;


    QLabel* general = NULL;
    QLabel* chief = NULL;
    QVector<QLabel*> pawns;
    QVector<QLabel*> soilders;
    QVector<QLabel*> blackChariots;
    QVector<QLabel*> redChariots;
    QVector<QLabel*> blackHorses;
    QVector<QLabel*> redHorses;
    QVector<QLabel*> blackCannons;
    QVector<QLabel*> redCannons;





    //return the label of general and chief
    QLabel* getGeneral();
    QLabel* getChief();
    //return the label of other pieces
    QVector<QLabel*> getPawns();
    QVector<QLabel*> getSoilders();
    QVector<QLabel*> getBlackChariots();
    QVector<QLabel*> getRedChariots();
    QVector<QLabel*> getBlackHorses();
    QVector<QLabel*> getRedHorses();
    QVector<QLabel*> getBlackCannons();
    QVector<QLabel*> getRedCannons();
    QVector<QLabel*> getBlackAdvisors();
    QVector<QLabel*> getRedAdvisors();
    QVector<QLabel*> getElephants();
    QVector<QLabel*> getBishops();

    //dedicated to return all Pieces along the way from start to target
    //this function is made specially for chariot and cannon
    QVector<QLabel*> blockingPieces(
        int startColumn,int startRow,int targetColumn,int targetRow);


};

#endif // CCHESSRULE_H
