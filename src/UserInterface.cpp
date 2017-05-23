//------------------------------------------------------------------------------
// Filename: UserInterface.cpp
// Author: ystc
// Created: 18.07.2016
// Last change: 24.08.2016
//------------------------------------------------------------------------------

#include "inc/UserInterface.h"
#include "inc/SequenceTab.h"
#include "inc/SingleCommandsTab.h"
#include "inc/ShuttleControl.h"
#include "inc/MaintenanceTab.h"
#include "inc/CostumTabWidget.h"
#include "inc/MainPage.h"
#include "inc/Shuttle.h"

//------------------------------------------------------------------------------
UserInterface::UserInterface(Shuttle *shuttleData, QWidget *parent)
  : QWidget(parent)
{
  cout << ">>>Opening new Shuttle Page" <<  endl;

  tasi_order_id = "Nada";
  sub_order_id = 0;
  sequence_running = 0;
  shuttle = shuttleData;

  mainLayout = new QGridLayout(this);
  singleCommandsTab = new SingleCommandsTab(this);
  sequenceTab = new SequenceTab();
  maintenanceTab = new MaintenanceTab();
  setWindowTitle(shuttle->getShuttleName() + "\t" + shuttle->getShuttleIp());
  createLayout();

  commandTimer = new QTimer();
  commandTimer -> setSingleShot(1);
  connect(commandTimer, SIGNAL(timeout()), this, SLOT(commandTimeOut()));

  //Connecting Control to UI to get shuttle messages
  shuttleControl = new ShuttleControl(this);

  //Get Data to fill the UI
  shuttleControl
      -> setShuttleIpAddress(shuttle->getShuttleIp(), shuttle->getPort());
  shuttleControl -> sendCmdGetSwVersions();
  shuttleControl -> sendCmdMntGetSensorData();
  buttonGetPosClicked();

  connect(singleCommandsTab, SIGNAL(executeSetPositionSignal(QString)),
          this, SLOT(executeSetPosition(QString)));
  connect(singleCommandsTab, SIGNAL(executeMoveSignal(QStringList)),
          this, SLOT(executeMove(QStringList)));
  connect(singleCommandsTab, SIGNAL(executeLoadSignal(QString)),
          this, SLOT(executeLoad(QString)));
  connect(singleCommandsTab, SIGNAL(executeUnloadSignal(QString)),
          this, SLOT(executeUnload(QString)));
  connect(singleCommandsTab, SIGNAL(saveUsedPosition(QString)),
          this, SLOT(savePosition(QString)));


  connect(shuttleControl, SIGNAL(printShuttleStatus(int, QString, bool)),
          this, SLOT(printShuttleStatusMessage(int, QString, bool)));
  connect(shuttleControl, SIGNAL(changeConnectionStatus(bool)),
          this, SLOT(setConnectionStatus(bool)));
  connect(shuttleControl, SIGNAL(setData(QString, float, float)),
          this, SLOT(setShuttleData(QString, float, float)));
//  connect(shuttleControl, SIGNAL(printSensorData(QStringList)), this,
//    SLOT(printShuttleData(QStringList)));

  connect(sequenceTab, SIGNAL(buildStatusMessage(QString)),
          this, SLOT(sendSequenceStatus(QString)));
  connect(sequenceTab, SIGNAL(saveUsedPosition(QString)),
          this, SLOT(savePosition(QString)));
  connect(sequenceTab, SIGNAL(moveTo(QStringList)),
          this, SLOT(executeMove(QStringList)));
  connect(sequenceTab, SIGNAL(loadAt(QString)),
          this, SLOT(executeLoad(QString)));
  connect(sequenceTab, SIGNAL(unloadAt(QString)),
          this, SLOT(executeUnload(QString)));
  connect(sequenceTab, SIGNAL(sequenceStarted(bool)),
          this, SLOT(setSequenceStarted(bool)));
  connect(sequenceTab, SIGNAL(getCurrentLevel()),
          this, SLOT(getLevel()));

  connect(maintenanceTab, SIGNAL(mntDriveSignal(int, int, unsigned char)),
          this, SLOT(mntDriveSlot(int, int, unsigned char)));
  connect(maintenanceTab, SIGNAL(mntFingerSignal(int, int)),
          this, SLOT(mntFingerSlot(int, int)));
  connect(maintenanceTab, SIGNAL(mntLAMSignal(int, int, int, int)),
          this, SLOT(mntLAMSlot(int, int, int, int)));
  connect(maintenanceTab, SIGNAL(mntSteerSignal(int, int, int)),
          this, SLOT(mntSteerSlot(int, int, int)));
  connect(maintenanceTab, SIGNAL(mntSpindleSignal(int, int)),
          this, SLOT(mntSpindleSlot(int, int)));
  connect(maintenanceTab, SIGNAL(calibrateSignal(int,int,int,int,int)),
          this, SLOT(mntCalibrate(int,int,int,int,int)));

  if(shuttle->getShuttleIp() == "dummy")
  {
    shuttleControl -> stopHeartBeat();
  }
}

//------------------------------------------------------------------------------
UserInterface::~UserInterface()
{
  delete maintenanceTab;
  delete sequenceTab;
  delete singleCommandsTab;
}

void UserInterface::closeEvent(QCloseEvent *)
{
  shuttle->setWindowOpen(false);
  setVisible(false);
  emit(userInterfaceClosed());
}


//------------------------------------------------------------------------------
void UserInterface::createLayout()
{
  setContentsMargins(0, 0, 0, 0);
  headerGroup = new QGroupBox;
  headerGrid = new QGridLayout;
  statusGrid = new QGridLayout;
  statusGroup = new QGroupBox;
  bodyGrid = new QGridLayout;
  bodyGroup = new QGroupBox;
  tabWidget = new CostumTabWidget;
  footerGroup = new QGroupBox;
  footerGrid = new QGridLayout;
  controlsGrid = new QGridLayout;
  controlsGroup = new QGroupBox;

  //header
  buttonCloseWindow = new QPushButton("X");
  buttonMaxWindow = new QPushButton("B");
  buttonMinWindow = new QPushButton("S");

  labelPosition = new QLabel("Position: -");
  labelIPAddress = new QLabel("IP: " + shuttle->getShuttleIp());
  labelShuttleName = new QLabel(shuttle->getShuttleName());
  labelEnergy = new QLabel("Energy Status: ");

  batteryEnergyBar = new QProgressBar;
  batteryEnergyBar -> setOrientation(Qt::Horizontal);
  batteryEnergyBar -> setMaximum(8500);
  batteryEnergyBar -> setMinimum(0);
  batteryEnergyBar -> setTextVisible(1);
  batteryEnergyBar -> setFormat("Battery        %p%      %v mAh");
  capsEnergyBar = new QProgressBar;
  capsEnergyBar -> setOrientation(Qt::Horizontal);
  capsEnergyBar -> setMaximum(70000);
  capsEnergyBar -> setMinimum(0);
  capsEnergyBar -> setTextVisible(1);
  capsEnergyBar -> setFormat("Capacitor     %p%         %v J");

  statusGrid->addWidget(labelShuttleName, 0, 1);
  statusGrid->addWidget(labelIPAddress, 0, 3);
  statusGrid->addWidget(labelPosition, 0, 5);
  statusGrid->addWidget(capsEnergyBar, 0, 9);
  statusGrid->addWidget(batteryEnergyBar, 0, 11);
  statusGrid->setContentsMargins(0, 0, 0, 0);
  statusGrid->setColumnStretch(1, 5);
  statusGrid->setColumnStretch(3, 5);
  statusGrid->setColumnStretch(5, 7);
  statusGrid->setColumnStretch(7, 5);
  statusGrid->setColumnStretch(9, 20);
  statusGrid->setColumnStretch(11, 20);
  statusGroup->setLayout(statusGrid);

  headerGrid->addWidget(statusGroup, 0, 0);
  headerGrid->setContentsMargins(0, 0, 3, 0);
  headerGroup->setLayout(headerGrid);

  //body
  tabWidget -> tabBar() -> setFocusPolicy(Qt::NoFocus);
  tabWidget->addTab(singleCommandsTab, "Commands");
  tabWidget->addTab(sequenceTab, "Sequence");
  tabWidget->addTab(maintenanceTab, "Maintenance");
  buttonPrepare = new QPushButton("Prepare for Work");
  connect(buttonPrepare, SIGNAL(pressed()),
          this, SLOT(buttonPrepareClicked()));

  buttonReference = new QPushButton("Reference Shuttle");
  connect(buttonReference, SIGNAL(pressed()),
          this, SLOT(buttonReferenceClicked()));

  buttonReset = new QPushButton("Deregulate Motors");
  connect(buttonReset, SIGNAL(pressed()), this, SLOT(buttonResetClicked()));

  controlsGrid->addWidget(buttonPrepare, 0, 0);
  controlsGrid->addWidget(buttonReference, 0, 1);
  controlsGrid->addWidget(buttonReset, 0, 2);
  controlsGroup->setLayout(controlsGrid);

  bodyGrid->addWidget(tabWidget, 0, 0);
  bodyGrid->addWidget(controlsGroup, 1, 0);
  bodyGrid->setContentsMargins(5, 0, 0, 0);
  bodyGroup->setLayout(bodyGrid);

  //footer
  log = new QTextEdit(this);
  log -> setPlainText("Shuttle Test Environment Version 1.3.0");
  log -> setReadOnly(1);
  footerGrid->addWidget(log, 1, 0);
  footerGrid->setContentsMargins(0, 0, 3, 0);
  footerGroup->setLayout(footerGrid);

  //add in main gridlayout
  mainLayout->addWidget(headerGroup, 0, 0);
  mainLayout->addWidget(bodyGroup, 1, 0);
  mainLayout->addWidget(footerGroup, 2, 0);
  //mainLayout->setRowStretch(0, 10 );
  //mainLayout->setRowStretch(1, 50);
  //mainLayout->setRowStretch(2, 20);
  mainLayout->setContentsMargins(0, 0, 5, 5);
  setLayout(mainLayout);

  //set object names for css/qss
  log -> setObjectName("log");
  tabWidget -> setObjectName("mainTabWidget");
  tabWidget -> tabBar() -> setObjectName("mainTabBar");
  batteryEnergyBar -> setObjectName("batteryBar");
  capsEnergyBar -> setObjectName("capsBar");
  headerGroup->setObjectName("header");
  labelPosition->setObjectName("positionLabel");
  labelIPAddress->setObjectName("ipLabel");
  labelShuttleName->setObjectName("nameLabel");
  buttonPrepare->setProperty("executeButton", true);
  buttonReference->setProperty("executeButton", true);
  buttonReset->setProperty("executeButton", true);
  labelPosition -> setProperty("statusLabel", true);
  labelIPAddress -> setProperty("statusLabel", true);
  labelShuttleName -> setProperty("statusLabel", true);
  labelEnergy -> setProperty("statusLabel", true);
  capsEnergyBar->setProperty("energyBar", true);
  batteryEnergyBar->setProperty("energyBar", true);
  statusGroup->setObjectName("statusGroup");
  bodyGroup->setObjectName("bodyGroup");
  controlsGroup->setObjectName("controlsGroup");

  cout << "UI created" << endl;
}

//------------------------------------------------------------------------------
void UserInterface::printShuttleData(QStringList shuttle_data)
{
  cout << "Adding Shuttle Data..." << endl;
  for(int index = 0; index < shuttle_data.size(); index++)
  {
    listSensorData -> addItem(shuttle_data[index]);
  }

  listSensorData -> addItem("-----------------------------");
}

//------------------------------------------------------------------------------
void UserInterface::setShuttleData(QString current_position, float benergy,
                                   float cenergy)
{
  cout << "Set Position and Energy Data..." << endl;

  if(benergy < 1500)
  {
    batteryEnergyBar
        -> setStyleSheet("#batteryBar::chunk{background-color: #CA0000}");
  }
  else
  {
    batteryEnergyBar
        -> setStyleSheet("#batteryBar::chunk{background-color: green}");
  }


  if(cenergy < 30000)
  {
    capsEnergyBar
        -> setStyleSheet("#capsBar::chunk{background-color: #CA0000}");
  }
  else
  {
    capsEnergyBar -> setStyleSheet("#capsBar::chunk{background-color: green}");
  }

  batteryEnergyBar -> setValue(benergy);
  capsEnergyBar -> setValue(cenergy);

  if(current_position.size() > 10)
  {
    labelPosition -> setText(tr("Pos: %1").arg(current_position));
  }
}

//------------------------------------------------------------------------------
void UserInterface::commandTimeOut()
{
  enable();
  commandTimer -> stop();
}

//------------------------------------------------------------------------------
QString UserInterface::simplify(QString positions)
{
  positions.simplified();
  positions.replace(" ", "");

  return positions;
}

//------------------------------------------------------------------------------
void UserInterface::buttonEmergencyStopClicked()
{
  cout << "!!!!!!!!!!!!!!!!!!!!!!EMERGENCY STOP!!!!!!!!!!!!!!!!!!!!!!" << endl;
  shuttleControl -> sendCmdMntEmergencyStop();
}

//------------------------------------------------------------------------------
void UserInterface::buttonRefreshDataClicked()
{
  listSensorData -> clear();
  shuttleControl -> sendCmdGetSwVersions();
  shuttleControl -> sendCmdMntGetSensorData();
}

//------------------------------------------------------------------------------
void UserInterface::buttonSaveLogClicked()
{
  QString filename;
  QString list;
  QByteArray text;

  list = log -> toPlainText();

  text = list.toUtf8();

  filename = QFileDialog::getSaveFileName(this, "Save Logfile", ".txt",
    "Textdateien (*.txt)");
  QFile file(filename);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    QMessageBox::warning(this, "Error", "File could not be saved!",
                         QMessageBox::Ok);
  }
  else
  {
    file.write(text);
    QMessageBox::warning(this, "Success!", "File saved", QMessageBox::Ok);
  }
}

//------------------------------------------------------------------------------
void UserInterface::savePosition(QString last_used_position)
{
  QStringList split_positions;

  last_used_position.simplified();
  last_used_position.replace(" ", "");
  split_positions = last_used_position.split(",");


  for(int index = 0; index < split_positions.size(); index++)
  {
    if(combobox_position_list.contains(split_positions[index]) == 0)
    {
      combobox_position_list.append(split_positions[index]);
    }
  }
  updateAllComboBoxes();
}

//------------------------------------------------------------------------------
void UserInterface::updatePositionList(QComboBox* current_combobox)
{
  int index;

  for(index = 0; index < combobox_position_list.size(); index++)
  {
    if(current_combobox -> findText(combobox_position_list[index]) == -1)
    {
      current_combobox -> addItem(combobox_position_list[index]);
    }
  }

  current_combobox -> setCurrentIndex(current_combobox->count() - 1);
}

//------------------------------------------------------------------------------
void UserInterface::updateAllComboBoxes()
{
  updatePositionList(singleCommandsTab -> setPositionCombo);
  updatePositionList(singleCommandsTab -> moveCombo);
  updatePositionList(singleCommandsTab -> loadCombo);
  updatePositionList(singleCommandsTab -> unloadCombo);
  updatePositionList(sequenceTab -> scomboMoveTo);
  updatePositionList(sequenceTab -> scomboLoad);
  updatePositionList(sequenceTab -> scomboUnload);
}

//------------------------------------------------------------------------------
void UserInterface::printShuttleStatusMessage(int status, QString status_text,
                                              bool move_finished)
{
  if(status != 3 && move_finished == 1)
  {
    enable();
  }

  if(move_finished == 0)
  {
    if(commandTimer -> isActive())
    {
      commandTimer -> start(TEN_SEC);
    }
  }

  if(status == 0)
  {
    log -> setTextColor("#000000");
  }
  else if(status == 3)
  {
    log -> setTextColor("#000000");
  }
  else
  {
    log -> setTextColor("#c20000");

    if(sequence_running == 1)
    {
      sequenceTab -> lastCommandSuccessful(0, status_text);
    }
  }

  if(status_text.isEmpty() == 0)
  {
    log -> append(status_text);
  }

  if(sequence_running == 1 && status == 0)
  {
    sequenceTab -> lastCommandSuccessful(1, "");
  }
}

//------------------------------------------------------------------------------
void UserInterface::setConnectionStatus(bool shuttle_is_online)
{
  sequenceTab -> setConnectionStatus(shuttle_is_online);
  if(shuttle_is_online)
  {
    emit(connection(1));
  }
  else
  {
    emit(connection(0));
  }
}

//------------------------------------------------------------------------------
void UserInterface::setSequenceStarted(bool started)
{
  sequence_running = started;

  if(started)
  {
    disable();
  }
  else
  {
    enable();
  }
}

//------------------------------------------------------------------------------
void UserInterface::enable()
{
  cout << "Enable UI..." << endl;
  tabWidget -> setTabEnabled(0, 1);
  tabWidget -> setTabEnabled(2, 1);

  buttonReference -> setEnabled(1);
  buttonPrepare -> setEnabled(1);
  buttonReset -> setEnabled(1);
}

//------------------------------------------------------------------------------
void UserInterface::disable()
{
  cout << "Disable UI..." << endl;
  tabWidget -> setTabEnabled(0, 0);
  tabWidget -> setTabEnabled(2, 0);

  buttonReference -> setEnabled(0);
  buttonPrepare -> setEnabled(0);
  buttonReset -> setEnabled(0);
}

//------------------------------------------------------------------------------
void UserInterface::executeMove(QStringList positions)
{
  shuttleControl -> sendCmdMove(tasi_order_id, sub_order_id, positions);
}

//------------------------------------------------------------------------------
void UserInterface::executeSetPosition(QString position)
{
  shuttleControl -> sendCmdSetPosition(position);
}

//------------------------------------------------------------------------------
void UserInterface::executeLoad(QString rackchannel)
{
  shuttleControl -> sendCmdLoad(tasi_order_id, rackchannel);
}

//------------------------------------------------------------------------------
void UserInterface::executeUnload(QString rackchannel)
{
  shuttleControl -> sendCmdUnload(tasi_order_id, rackchannel);
}

//------------------------------------------------------------------------------
void UserInterface::buttonGetPosClicked()
{
  shuttleControl -> sendCmdGetStatus();
}

//------------------------------------------------------------------------------
void UserInterface::buttonPrepareClicked()
{
  shuttleControl -> sendCmdPrepareForWork();
}

//------------------------------------------------------------------------------
void UserInterface::buttonReferenceClicked()
{
  shuttleControl -> sendCmdMntReference(99);
}

//------------------------------------------------------------------------------
void UserInterface::buttonResetClicked()
{
  shuttleControl -> sendCmdMntSwReset();
}

//------------------------------------------------------------------------------
void UserInterface::buttonClearBarClicked()
{
  log -> clear();
  log -> setPlainText("Shuttle Test Environment Version 1.2.0");
}

//------------------------------------------------------------------------------
void UserInterface::sendSequenceStatus(QString status_text)
{
  shuttleControl -> printSequenceStatus(status_text);
}

//------------------------------------------------------------------------------
void UserInterface::mntDriveSlot(int distance, int speed, unsigned char ctrl)
{
  shuttleControl -> sendCmdMntDrive(distance, speed, ctrl);
}

//------------------------------------------------------------------------------
void UserInterface::mntFingerSlot(int finger, int angle)
{
  shuttleControl -> sendCmdMntFinger(finger, angle);
}

//------------------------------------------------------------------------------
void UserInterface::mntLAMSlot(int position, int vmax, int acceleration,
                               int brake)
{
  shuttleControl -> sendCmdMntMoveLAM(position, vmax, acceleration, brake);
}

//------------------------------------------------------------------------------
void UserInterface::mntSteerSlot(int degree, int wheels, int delay)
{
  shuttleControl -> sendCmdMntQuickSteer(degree, wheels, delay);
}

//------------------------------------------------------------------------------
void UserInterface::mntSpindleSlot(int spindle, int position)
{
  shuttleControl -> sendCmdMntSpindle(spindle, position);
}

//------------------------------------------------------------------------------
void UserInterface::mntCalibrate(int sub_cmd, int motor_nr, int steer_context,
                                 int diff, int opt_diff)
{
  shuttleControl -> sendCmdMntCalibrate(sub_cmd, motor_nr, steer_context, diff,
                                        opt_diff);
}

//------------------------------------------------------------------------------
bool UserInterface::eventFilter(QObject *obj, QEvent *event)
{
  //TODO: find a easier way to get the event Mouserelease
  if (obj == lineMoveTo -> lineEdit() || obj == lineLoad -> lineEdit()
      || obj == lineUnload -> lineEdit() || obj == lineSetPos -> lineEdit())
  {
      if (event->type() == QEvent::MouseButtonRelease)
      {
          if(((QMouseEvent*)event) -> button() == Qt::LeftButton)
          {
            //Sends the comboBox obj pointer to the function in userinterface
            //and adds the list
            if(obj == lineLoad -> lineEdit())
            {
              updatePositionList(lineLoad);
            }
            else if(obj == lineMoveTo -> lineEdit())
            {
              updatePositionList(lineMoveTo);
            }
            else if(obj == lineSetPos -> lineEdit())
            {
              updatePositionList(lineSetPos);
            }
            else if(obj == lineUnload -> lineEdit())
            {
              updatePositionList(lineUnload);
            }
          }
          return true;
      }
      else
      {
          return false;
      }
  }
  else
  {
    // pass the event on to the parent class
    return UserInterface::eventFilter(obj, event);
  }
}

//------------------------------------------------------------------------------
void UserInterface::getLevel()
{
  QStringList get_level;

  if(labelPosition -> text().split(" ")[1].size() > 3)
  {
    get_level = labelPosition -> text().split(".");
  }
  else
  {
    get_level.append("S01");
    get_level.append("001");
  }

  QString current_level = get_level.at(1) + ".";

  sequenceTab -> setLevel(current_level);
}
