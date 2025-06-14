#include "gamerecorder.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "piece.h"
#include <QStringList>
#include <QRegularExpression>



//inner class part
const QString GameRecorder::Move::nums[9] = { "九", "八", "七", "六", "五", "四", "三", "二", "一" };


QString GameRecorder::Move::toString() const{
    bool isRed = Piece::sideOfPiece(this->piece) == Piece::Side::Red;
    QString move;
    move += piece;
    move += Move::columnToString(fromColumn, isRed);
    QString direction = getDirection(fromRow, toRow, isRed);
    move += direction;
    if (direction == "平") {
        // move horizontally
        //車一平六
        move += columnToString(toColumn, isRed);
    } else {
        // move vertically，calculate steps taken
        //马七进五 etc
        move += QString::number(abs(fromRow - toRow));
    }
    return move;
}

inline QString GameRecorder::Move::columnToString(int column, bool isRed) {
    return isRed ? nums[column] : nums[8-column];
}
inline QString GameRecorder::Move::getDirection(int fromRow, int toRow, bool isRed) {
    if (fromRow == toRow) return "平"; //horizontally
    //note the relative direction when it comes to the side
    return (isRed ? (toRow < fromRow) : (toRow > fromRow)) ? "进" : "退";
}

GameRecorder::GameRecorder() {


}

void GameRecorder::recordMove(Move* move){
    moveHistory.push_back(move);
}

QString GameRecorder::getRecord() const{
    QString record;
    for (const auto move : moveHistory) {
        record.append(move->toString() + " ");
    }
    return record;
}
//load record
void GameRecorder::loadRecord(const QString& record){
    moveHistory.clear();

    // separate by space character
    QStringList lines = record.split(' ');

    for (const QString& line : lines) {
        moveHistory.append(new Move(line));
    }
}
//undo the last move
void GameRecorder::undoLastMove(){
    if (!moveHistory.empty()) {
        moveHistory.pop_back();
    }
}
//save the record to file
void GameRecorder::saveToFile(const std::string& filename) const{
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return;
    }
    outFile << getRecord().toStdString();
    outFile.close();
}
//load record from specified file
bool GameRecorder::loadFromFile(const std::string& filename){
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return false;
    }

    std::ostringstream oss;
    //read content from the file and transfer to string
    oss << inFile.rdbuf();
    inFile.close();

    loadRecord(QString::fromStdString(oss.str()));
    return true;
}
