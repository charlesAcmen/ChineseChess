#ifndef GAMERECORDER_H
#define GAMERECORDER_H

#include <qlabel.h>


class GameRecorder
{
    //inner class
    class Move {
        public:
            //by default red side is the one down the chess board
            QString toString() const;

            Move(const QString& piece, int fColumn, int fRow, int tColumn, int tRow)
                : piece(piece), fromColumn(fColumn), fromRow(fRow), toColumn(tColumn), toRow(tRow) {}
            Move(const QString& record){
                this->record = &record;
            }
        private:
            static const QString nums[9];
            const QString piece;  // name of the piece,by default is the object name for the qlabel
            //note these starts from 0
            int fromColumn, fromRow;   // start point
            int toColumn, toRow;       // end point
            const QString* record;



            // convert column to qstring
            inline static QString columnToString(int column, bool isRed);

            // return the directoin of the move
            inline static QString getDirection(int fromRow, int toRow, bool isRed);
    };
public:

    GameRecorder();
    //record move
    void recordMove(Move* move);

    QString getRecord() const;
    //load record
    void loadRecord(const QString& record);
    //undo the last move
    void undoLastMove();
    //save the record to file
    void saveToFile(const std::string& filename) const;
    //load record from specified file
    bool loadFromFile(const std::string& filename);
private:

    QVector<Move*> moveHistory;
};

#endif // GAMERECORDER_H
