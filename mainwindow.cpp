#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(){
    // get type Widget with name of "Board"
    QWidget * boardWidgets = ui->centralwidget->findChild<QWidget*>("Board");
    // get type QLabel with name of each pieces:
    //
    QVector<QLabel*> labels;
    //add in order of row
    labels.append(boardWidgets->findChild<QLabel*>("Black_Chariot1"));
    labels.append(boardWidgets->findChild<QLabel*>("Black_Horse2"));
    labels.append(boardWidgets->findChild<QLabel*>("Black_Elephant3"));
    labels.append(boardWidgets->findChild<QLabel*>("Black_Advisor4"));
    labels.append(boardWidgets->findChild<QLabel*>("Black_General"));
    labels.append(boardWidgets->findChild<QLabel*>("Black_Advisor6"));
    labels.append(boardWidgets->findChild<QLabel*>("Black_Elephant7"));
    labels.append(boardWidgets->findChild<QLabel*>("Black_Horse8"));
    labels.append(boardWidgets->findChild<QLabel*>("Black_Chariot9"));


    labels.append(boardWidgets->findChild<QLabel*>("label_1"));
    labels.append(boardWidgets->findChild<QLabel*>("label_2"));
    labels.append(boardWidgets->findChild<QLabel*>("label_3"));
    labels.append(boardWidgets->findChild<QLabel*>("label_4"));
    labels.append(boardWidgets->findChild<QLabel*>("label_5"));
    labels.append(boardWidgets->findChild<QLabel*>("label_6"));
    labels.append(boardWidgets->findChild<QLabel*>("label_7"));
    labels.append(boardWidgets->findChild<QLabel*>("label_8"));
    labels.append(boardWidgets->findChild<QLabel*>("label_9"));

    labels.append(boardWidgets->findChild<QLabel*>("label_10"));





    labels.append(boardWidgets->findChild<QLabel*>("Black_Cannon2"));

    labels.append(boardWidgets->findChild<QLabel*>("label_11"));
    labels.append(boardWidgets->findChild<QLabel*>("label_12"));
    labels.append(boardWidgets->findChild<QLabel*>("label_13"));
    labels.append(boardWidgets->findChild<QLabel*>("label_14"));
    labels.append(boardWidgets->findChild<QLabel*>("label_15"));

    labels.append(boardWidgets->findChild<QLabel*>("Black_Cannon8"));
    labels.append(boardWidgets->findChild<QLabel*>("label_16"));


    labels.append(boardWidgets->findChild<QLabel*>("Black_Pawn1"));
    labels.append(boardWidgets->findChild<QLabel*>("label_17"));

    labels.append(boardWidgets->findChild<QLabel*>("Black_Pawn3"));
    labels.append(boardWidgets->findChild<QLabel*>("label_18"));

    labels.append(boardWidgets->findChild<QLabel*>("Black_Pawn5"));
    labels.append(boardWidgets->findChild<QLabel*>("label_19"));

    labels.append(boardWidgets->findChild<QLabel*>("Black_Pawn7"));
    labels.append(boardWidgets->findChild<QLabel*>("label_20"));

    labels.append(boardWidgets->findChild<QLabel*>("Black_Pawn9"));


    labels.append(boardWidgets->findChild<QLabel*>("label_21"));
    labels.append(boardWidgets->findChild<QLabel*>("label_22"));
    labels.append(boardWidgets->findChild<QLabel*>("label_23"));
    labels.append(boardWidgets->findChild<QLabel*>("label_24"));
    labels.append(boardWidgets->findChild<QLabel*>("label_25"));
    labels.append(boardWidgets->findChild<QLabel*>("label_26"));
    labels.append(boardWidgets->findChild<QLabel*>("label_27"));
    labels.append(boardWidgets->findChild<QLabel*>("label_28"));
    labels.append(boardWidgets->findChild<QLabel*>("label_29"));





    labels.append(boardWidgets->findChild<QLabel*>("label_30"));
    labels.append(boardWidgets->findChild<QLabel*>("label_31"));
    labels.append(boardWidgets->findChild<QLabel*>("label_32"));
    labels.append(boardWidgets->findChild<QLabel*>("label_33"));
    labels.append(boardWidgets->findChild<QLabel*>("label_34"));
    labels.append(boardWidgets->findChild<QLabel*>("label_35"));
    labels.append(boardWidgets->findChild<QLabel*>("label_36"));
    labels.append(boardWidgets->findChild<QLabel*>("label_37"));
    labels.append(boardWidgets->findChild<QLabel*>("label_38"));


    labels.append(boardWidgets->findChild<QLabel*>("Red_Soilder9"));
    labels.append(boardWidgets->findChild<QLabel*>("label_39"));

    labels.append(boardWidgets->findChild<QLabel*>("Red_Soilder7"));
    labels.append(boardWidgets->findChild<QLabel*>("label_40"));

    labels.append(boardWidgets->findChild<QLabel*>("Red_Soilder5"));
    labels.append(boardWidgets->findChild<QLabel*>("label_41"));

    labels.append(boardWidgets->findChild<QLabel*>("Red_Soilder3"));
    labels.append(boardWidgets->findChild<QLabel*>("label_42"));

    labels.append(boardWidgets->findChild<QLabel*>("Red_Soilder1"));


    labels.append(boardWidgets->findChild<QLabel*>("label_43"));



    labels.append(boardWidgets->findChild<QLabel*>("Red_Cannon8"));
    labels.append(boardWidgets->findChild<QLabel*>("label_44"));
    labels.append(boardWidgets->findChild<QLabel*>("label_45"));
    labels.append(boardWidgets->findChild<QLabel*>("label_46"));
    labels.append(boardWidgets->findChild<QLabel*>("label_47"));
    labels.append(boardWidgets->findChild<QLabel*>("label_48"));

    labels.append(boardWidgets->findChild<QLabel*>("Red_Cannon2"));
    labels.append(boardWidgets->findChild<QLabel*>("label_49"));

    labels.append(boardWidgets->findChild<QLabel*>("label_50"));
    labels.append(boardWidgets->findChild<QLabel*>("label_51"));
    labels.append(boardWidgets->findChild<QLabel*>("label_52"));
    labels.append(boardWidgets->findChild<QLabel*>("label_53"));
    labels.append(boardWidgets->findChild<QLabel*>("label_54"));
    labels.append(boardWidgets->findChild<QLabel*>("label_55"));
    labels.append(boardWidgets->findChild<QLabel*>("label_56"));
    labels.append(boardWidgets->findChild<QLabel*>("label_57"));
    labels.append(boardWidgets->findChild<QLabel*>("label_58"));

    labels.append(boardWidgets->findChild<QLabel*>("Red_Chariot9"));
    labels.append(boardWidgets->findChild<QLabel*>("Red_Horse8"));
    labels.append(boardWidgets->findChild<QLabel*>("Red_Bishop7"));
    labels.append(boardWidgets->findChild<QLabel*>("Red_Advisor6"));
    labels.append(boardWidgets->findChild<QLabel*>("Red_Chief"));
    labels.append(boardWidgets->findChild<QLabel*>("Red_Advisor4"));
    labels.append(boardWidgets->findChild<QLabel*>("Red_Bishop3"));
    labels.append(boardWidgets->findChild<QLabel*>("Red_Horse2"));
    labels.append(boardWidgets->findChild<QLabel*>("Red_Chariot1"));








    this->board = new Board(boardWidgets,labels);
}
