//------------------------------------------------------------------------------
// Filename: SingleCommandsTab.cpp
// Author: ystc
// Created: 03.03.2017
// Last change: 18.04.2017
//------------------------------------------------------------------------------

#include "inc/SingleCommandsTab.h"

enum commands{
  SET_POS,
  MOVE_TO,
  LOAD,
  UNLOAD
};

//------------------------------------------------------------------------------
SingleCommandsTab::SingleCommandsTab(QWidget *parent) : QWidget(parent)
{
  commandTimer = new QTimer();
  commandTimer -> setSingleShot(1);
  connect(commandTimer, SIGNAL(timeout()), this, SLOT(commandTimeOut()));

  mainLayout = new QGridLayout;

  setPositionLabel = new QLabel("Set Position: ");
  setPositionCombo = new QComboBox;
  setPositionCombo -> setEditable(1);
  connect(setPositionCombo -> lineEdit(), SIGNAL(returnPressed()),
          this, SLOT(buttonSetPosClicked()));
  setPositionCombo -> lineEdit() -> installEventFilter(this);
  setPositionButton = new QPushButton("Go");
  connect(setPositionButton, SIGNAL(pressed()),
          this, SLOT(buttonSetPosClicked()));

  moveLabel = new QLabel("Move to: ");
  moveCombo = new QComboBox;
  moveCombo -> setEditable(1);
  moveCombo -> setToolTip("Separate positions with a comma to create a path.");
  connect(moveCombo -> lineEdit(), SIGNAL(returnPressed()),
          this, SLOT(buttonMoveClicked()));
  moveCombo -> lineEdit() -> installEventFilter(this);
  moveButton = new QPushButton("Go");
  connect(moveButton, SIGNAL(pressed()), this, SLOT(buttonMoveClicked()));

  loadLabel = new QLabel("Load carrier: ");
  loadCombo = new QComboBox;
  loadCombo -> setEditable(1);
  connect(loadCombo -> lineEdit(), SIGNAL(returnPressed()),
          this, SLOT(buttonLoadClicked()));
  loadCombo -> lineEdit() -> installEventFilter(this);
  loadButton = new QPushButton("Go");
  connect(loadButton, SIGNAL(pressed()), this, SLOT(buttonLoadClicked()));

  unloadLabel = new QLabel("Unload carrier: ");
  unloadCombo = new QComboBox();
  unloadCombo -> setEditable(1);
  connect(unloadCombo -> lineEdit(), SIGNAL(returnPressed()),
          this, SLOT(buttonUnloadClicked()));
  unloadCombo -> lineEdit() -> installEventFilter(this);
  unloadButton = new QPushButton("Go");
  connect(unloadButton, SIGNAL(pressed()), this, SLOT(buttonUnloadClicked()));


  mainLayout->addWidget(setPositionLabel, 1, 1);
  mainLayout->addWidget(setPositionCombo, 2, 1);
  mainLayout->addWidget(setPositionButton, 2 , 2);
  mainLayout->addWidget(moveLabel, 3, 1);
  mainLayout->addWidget(moveCombo, 4, 1);
  mainLayout->addWidget(moveButton, 4, 2);
  mainLayout->addWidget(loadLabel, 5, 1);
  mainLayout->addWidget(loadCombo, 6, 1);
  mainLayout->addWidget(loadButton, 6, 2);
  mainLayout->addWidget(unloadLabel, 7, 1);
  mainLayout->addWidget(unloadCombo, 8, 1);
  mainLayout->addWidget(unloadButton, 8, 2);
  mainLayout->setContentsMargins(10, 0, 0, 50);
  mainLayout->setColumnStretch(0, 5);
  mainLayout->setColumnStretch(1, 30);
  mainLayout->setColumnStretch(2, 10);
  mainLayout->setColumnStretch(3, 100);
  setLayout(mainLayout);

  setPositionLabel -> setProperty("executeLabel", true);
  setPositionCombo -> setProperty("executeLine", true);
  setPositionButton -> setProperty("executeButton", true);
  moveLabel -> setProperty("executeLabel", true);
  moveCombo -> setProperty("executeLine", true);
  moveButton -> setProperty("executeButton", true);
  loadLabel -> setProperty("executeLabel", true);
  loadCombo -> setProperty("executeLine", true);
  loadButton -> setProperty("executeButton", true);
  unloadLabel -> setProperty("executeLabel", true);
  unloadCombo -> setProperty("executeLine", true);
  unloadButton -> setProperty("executeButton", true);
}

//------------------------------------------------------------------------------
void SingleCommandsTab::execute(int command_tag, QComboBox *command_box)
{
  QStringList positions_list;
  disable();
  commandTimer->start(10);
  command_box->setCurrentText(command_box->currentText().toUpper());
  QString command_string = simplify(command_box -> currentText());
  emit(saveUsedPosition(command_string));

  switch(command_tag){
    case 0:
      emit(executeSetPositionSignal(command_string));
      break;
    case 1:
      positions_list = command_string.split(",");
      emit(executeMoveSignal(positions_list));
      break;
    case 2:
      emit(executeLoadSignal(command_string));
      break;
    case 3:
      emit(executeUnloadSignal(command_string));
      break;
  }
}

//------------------------------------------------------------------------------
void SingleCommandsTab::buttonSetPosClicked()
{
  execute(SET_POS, setPositionCombo);
}

//------------------------------------------------------------------------------
void SingleCommandsTab::buttonMoveClicked()
{
  execute(MOVE_TO, moveCombo);
}

//------------------------------------------------------------------------------
void SingleCommandsTab::buttonLoadClicked()
{
  execute(LOAD, loadCombo);
}

//------------------------------------------------------------------------------
void SingleCommandsTab::buttonUnloadClicked()
{
  execute(UNLOAD, unloadCombo);
}

//------------------------------------------------------------------------------
void SingleCommandsTab::enable()
{
  loadButton->setEnabled(1);
  unloadButton->setEnabled(1);
  setPositionButton->setEnabled(1);
  moveButton->setEnabled(1);
}

//------------------------------------------------------------------------------
void SingleCommandsTab::disable()
{
  loadButton->setEnabled(0);
  unloadButton->setEnabled(0);
  setPositionButton->setEnabled(0);
  moveButton->setEnabled(0);
}

//------------------------------------------------------------------------------
void SingleCommandsTab::commandTimeOut()
{
  enable();
  commandTimer -> stop();
}

//------------------------------------------------------------------------------
QString SingleCommandsTab::simplify(QString positions)
{
  positions.simplified();
  positions.replace(" ", "");

  return positions;
}


