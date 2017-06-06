//----------------------------------------------------------------------------
// Filename: SingleCommandsTab4.h
// Description: First tab - contains most used commands for the Shuttle
// Author: ystc
// Created: 04.05.2016
// Last change: 03.08.2016
//----------------------------------------------------------------------------

#ifndef SINGLECOMMANDSTAB_H_
#define SINGLECOMMANDSTAB_H_

#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QTimer>
#include <QString>
#include <QStringList>

using std::cout;
using std::endl;

class SingleCommandsTab : public QWidget
{
  Q_OBJECT

  public:
    SingleCommandsTab(QWidget* parent = 0);
    QComboBox *setPositionCombo, *moveCombo, *loadCombo, *unloadCombo;

  private:
    QGridLayout *mainLayout;
    QPushButton *setPositionButton, *moveButton, *loadButton, *unloadButton;
    QLabel *setPositionLabel, *moveLabel, *loadLabel, *unloadLabel;
    QTimer *commandTimer;

    void enable();
    void disable();
    void execute(int command_tag, QComboBox *command_box);

  public slots:
    void buttonSetPosClicked();
    void buttonMoveClicked();
    void buttonLoadClicked();
    void buttonUnloadClicked();
    void commandTimeOut();
    QString simplify(QString positions);

  signals:
    void executeSetPositionSignal(QString position);
    void executeMoveSignal(QStringList positions);
    void executeLoadSignal(QString rackchannel);
    void executeUnloadSignal(QString rackchannel);
    void saveUsedPosition(QString last_position_used);
};

#endif //SINGLECOMMANDSTAB_H_
