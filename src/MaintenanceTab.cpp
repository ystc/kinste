//------------------------------------------------------------------------------
// Filename: MaintenanceTab.cpp
// Author: Christoph Steinkellner
// Created: 04.05.2016
// Last change: 19.08.2016
//------------------------------------------------------------------------------

#include "inc/MaintenanceTab.h"

//------------------------------------------------------------------------------
MaintenanceTab::MaintenanceTab(QWidget* parent) : QWidget(parent)
{
  buildLayout();
}

//------------------------------------------------------------------------------
MaintenanceTab::~MaintenanceTab()
{
  cout << "Destruct Maintenance" << endl;
}

//------------------------------------------------------------------------------
void MaintenanceTab::buildLayout()
{
  mainGrid = new QGridLayout;

  //Drive group
  driveGroup = new QGroupBox("Drive");
  driveGrid = new QGridLayout;
  driveButton = new QPushButton("Go");
  driveLine1 = new QLineEdit("300");
  driveLine2 = new QLineEdit("0");
  driveLine3 = new QLineEdit("8");
  driveLabel1 = new QLabel("Speed (mm/s)");
  driveLabel2 = new QLabel("Distance (mm)");
  driveLabel3 = new QLabel("Breaking");
  connect(driveLine2, SIGNAL(returnPressed()), this, SLOT(emitDriveSignal()));
  connect(driveLine1, SIGNAL(returnPressed()), this, SLOT(emitDriveSignal()));
  connect(driveLine3, SIGNAL(returnPressed()), this, SLOT(emitDriveSignal()));
  connect(driveButton, SIGNAL(pressed()), this, SLOT(emitDriveSignal()));
  driveButton -> setProperty("goButton", true);
  driveLine1 ->  setProperty("mntLine", true);
  driveLine2 -> setProperty("mntLine", true);
  driveLine3 -> setProperty("mntLine", true);
  driveLabel1 -> setProperty("mntLabel", true);
  driveLabel2 -> setProperty("mntLabel", true);
  driveLabel3 -> setProperty("mntLabel", true);
  driveGroup -> setProperty("groupBox", true);
  driveGrid -> addWidget(driveLine1, 0, 0, Qt::AlignBottom);
  driveGrid -> addWidget(driveLabel1, 1, 0, Qt::AlignTop);
  driveGrid -> addWidget(driveLine2, 0, 1, Qt::AlignBottom);
  driveGrid -> addWidget(driveLabel2, 1, 1, Qt::AlignTop);
  driveGrid -> addWidget(driveLine3, 2, 0, Qt::AlignBottom);
  driveGrid -> addWidget(driveLabel3, 3, 0, Qt::AlignTop);
  driveGrid -> addWidget(driveButton, 2, 1, Qt::AlignBottom);
  driveGrid -> setContentsMargins(0, 20, 0, 20);
  driveGrid ->setSpacing(5);
  driveGroup -> setLayout(driveGrid);

  //Finger group
  fingerGroup = new QGroupBox("Fingers");
  fingerGrid = new QGridLayout;
  fingerFront = new QCheckBox("Front");
  fingerRear = new QCheckBox("Rear");
  fingerLine = new QLineEdit("90");
  fingerButton = new QPushButton("Go");
  fingerLabel = new QLabel("Angle (degree)");
  fingerFront -> setChecked(1);
  fingerRear -> setChecked(1);
  connect(fingerButton, SIGNAL(pressed()), this, SLOT(emitFingerSignal()));
  connect(fingerLine, SIGNAL(returnPressed()), this, SLOT(emitFingerSignal()));
  fingerRear -> setProperty("mntCheckBox", true);
  fingerFront -> setProperty("mntCheckBox", true);
  fingerButton -> setProperty("goButton", true);
  fingerLabel -> setProperty("mntLabel", true);
  fingerLine ->  setProperty("mntLine", true);
  fingerGroup -> setProperty("groupBox", true);
  fingerGrid -> addWidget(fingerFront, 0, 0);
  fingerGrid -> addWidget(fingerRear, 1, 0, 2, 1);
  fingerGrid -> addWidget(fingerLine, 0, 1, Qt::AlignBottom);
  fingerGrid -> addWidget(fingerLabel, 1, 1, Qt::AlignTop);
  fingerGrid -> addWidget(fingerButton, 2, 1, Qt::AlignBottom);
  fingerGrid -> setContentsMargins(20, 20, 20, 20);
  fingerGrid ->setSpacing(5);
  fingerGroup -> setLayout(fingerGrid);

  //Spindle group
  spindleGroup = new QGroupBox("Spindle");
  spindleGrid = new QGridLayout;
  spindleButton = new QPushButton("Go");
  spindleFront = new QCheckBox("Front");
  spindleRear = new QCheckBox("Rear");
  spindleLine = new QLineEdit("60");
  spindleLabel = new QLabel("Distance (mm)");
  spindleFront -> setChecked(1);
  spindleRear -> setChecked(1);
  connect(spindleButton, SIGNAL(pressed()), this, SLOT(emitSpindleSignal()));
  connect(spindleLine, SIGNAL(returnPressed()),
          this, SLOT(emitSpindleSignal()));
  spindleGroup -> setProperty("groupBox", true);
  spindleFront -> setProperty("mntCheckBox", true);
  spindleRear -> setProperty("mntCheckBox", true);
  spindleLine ->  setProperty("mntLine", true);
  spindleLabel -> setProperty("mntLabel", true);
  spindleButton -> setProperty("goButton", true);
  spindleGrid -> addWidget(spindleFront, 0, 0);
  spindleGrid -> addWidget(spindleRear, 1, 0, 2, 1);
  spindleGrid -> addWidget(spindleLine, 0, 1, Qt::AlignBottom);
  spindleGrid -> addWidget(spindleLabel, 1, 1, Qt::AlignTop);
  spindleGrid -> addWidget(spindleButton, 2, 1, Qt::AlignBottom);
  spindleGrid ->setContentsMargins(20, 20, 20, 20);
  spindleGrid ->setSpacing(5);
  spindleGroup -> setLayout(spindleGrid);

  //LAM group
  lamGroup = new QGroupBox("Sword");
  lamGrid = new QGridLayout;
  lamButton = new QPushButton("Go");
  lamLine1 = new QLineEdit("300");
  lamLine2 = new QLineEdit("0");
  lamLine3 = new QLineEdit("600");
  lamLine4 = new QLineEdit("600");
  lamLabel1 = new QLabel("Speed (mm/s)");
  lamLabel2 = new QLabel("Distance (mm)");
  lamLabel3 = new QLabel("Acceleration");
  lamLabel4 = new QLabel("Breaking power");
  lamLine3 -> setToolTip("Acceleration between 300 and 600");
  lamLine4 -> setToolTip("Breaking power between 300 and 600");
  connect(lamButton, SIGNAL(pressed()), this, SLOT(emitLAMSignal()));
  connect(lamLine4, SIGNAL(returnPressed()), this, SLOT(emitLAMSignal()));;
  connect(lamLine1, SIGNAL(returnPressed()), this, SLOT(emitLAMSignal()));
  connect(lamLine2, SIGNAL(returnPressed()), this, SLOT(emitLAMSignal()));
  connect(lamLine3, SIGNAL(returnPressed()), this, SLOT(emitLAMSignal()));
  lamLine1 ->  setProperty("mntLine", true);
  lamLine2 ->  setProperty("mntLine", true);
  lamLine3 ->  setProperty("mntLine", true);
  lamLine4 ->  setProperty("mntLine", true);
  lamButton -> setProperty("goButton", true);
  lamGroup ->  setProperty("groupBox", true);
  lamLabel1 -> setProperty("mntLabel", true);
  lamLabel2 -> setProperty("mntLabel", true);
  lamLabel3 -> setProperty("mntLabel", true);
  lamLabel4 -> setProperty("mntLabel", true);
  lamGrid -> addWidget(lamLine1, 0, 0, Qt::AlignBottom);
  lamGrid -> addWidget(lamLabel1, 1, 0, Qt::AlignTop);
  lamGrid -> addWidget(lamLine2, 0, 1, Qt::AlignBottom);
  lamGrid -> addWidget(lamLabel2, 1, 1, Qt::AlignTop);
  lamGrid -> addWidget(lamLine3, 2, 0, Qt::AlignBottom);
  lamGrid -> addWidget(lamLabel3, 3, 0, Qt::AlignTop);
  lamGrid -> addWidget(lamLine4, 2, 1, Qt::AlignBottom);
  lamGrid -> addWidget(lamLabel4, 3, 1, Qt::AlignTop);
  lamGrid -> addWidget(lamButton, 2, 2, Qt::AlignBottom);
  lamGrid -> setContentsMargins(0, 20, 0 , 20);
  lamGrid ->setSpacing(5);
  lamGroup -> setLayout(lamGrid);

  //Steer group
  steerGroup = new QGroupBox("Steer");
  steerGrid = new QGridLayout;
  steerButton = new QPushButton("Go");
  steerRightFront = new QCheckBox("Front right");
  steerRightRear = new QCheckBox("Rear right");
  steerLeftFront = new QCheckBox("Front left");
  steerLeftRear = new QCheckBox("Rear left");
  steerLine1 = new QLineEdit("0");
  steerLine2 = new QLineEdit("10");
  steerLabel1 = new QLabel("Angle (degree)");
  steerLabel2 = new QLabel("Delay (ms)");
  steerRightFront -> setChecked(1);
  steerRightRear -> setChecked(1);
  steerLeftFront -> setChecked(1);
  steerLeftRear -> setChecked(1);
  connect(steerLine1, SIGNAL(returnPressed()), this, SLOT(emitSteerSignal()));
  connect(steerButton, SIGNAL(pressed()), this, SLOT(emitSteerSignal()));
  connect(steerLine2, SIGNAL(returnPressed()), this, SLOT(emitSteerSignal()));
  steerGroup -> setProperty("groupBox", true);
  steerLabel2 -> setProperty("mntLabel", true);
  steerLabel1 -> setProperty("mntLabel", true);
  steerLine2 ->  setProperty("mntLine", true);
  steerLine1 ->  setProperty("mntLine", true);
  steerRightFront -> setProperty("mntCheckBox", true);
  steerRightRear -> setProperty("mntCheckBox", true);
  steerLeftFront -> setProperty("mntCheckBox", true);
  steerLeftRear -> setProperty("mntCheckBox", true);
  steerButton -> setProperty("goButton", true);
  steerGrid -> addWidget(steerLeftFront, 1, 0, Qt::AlignTop);
  steerGrid -> addWidget(steerRightFront, 1, 1, Qt::AlignTop);
  steerGrid -> addWidget(steerLeftRear, 2, 0, Qt::AlignTop);
  steerGrid -> addWidget(steerRightRear, 2, 1, Qt::AlignTop);
  steerGrid -> addWidget(steerLine1, 3, 0, Qt::AlignBottom);
  steerGrid -> addWidget(steerLine2, 3, 1, Qt::AlignBottom);
  steerGrid -> addWidget(steerLabel1, 4, 0, Qt::AlignTop);
  steerGrid -> addWidget(steerLabel2, 4, 1, Qt::AlignTop);
  steerGrid -> addWidget(steerButton, 3, 2, Qt::AlignBottom);
  steerGrid -> setContentsMargins(20, 20, 20, 20);
  steerGrid ->setSpacing(5);
  steerGroup -> setLayout(steerGrid);

  //Calibrate group
  calibrateGroup = new QGroupBox("Calibrate");
  calibrateGrid = new QGridLayout;
  calSteerGroup = new QGroupBox("Calibrate Steering");
  calSteerLayout = new QHBoxLayout;
  calSteerLeftGrid = new QGridLayout;
  calSteerRightGrid = new QGridLayout;
  calLamButton = new QPushButton("Calibrate LAM");
  calSpindleButton = new QPushButton("Calibrate Spindle");
  calFingerButton = new QPushButton("Calibrate Finger");
  calDriveFront = new QPushButton("Front Motor");
  calDriveBack = new QPushButton("Rear Motor");
  calSteerIs0 = new QPushButton("Set straight");
  calSteerIs90 = new QPushButton("Set cross");
  saveConfig = new QPushButton("Save Config");
  resetConfig = new QPushButton("Reset Config");
  loadConfig = new QPushButton("Load Config");
  calSteerLeftFront = new QPushButton;
  calSteerRightFront = new QPushButton;
  calSteerLeftRear = new QPushButton;
  calSteerRightRear = new QPushButton;
  //CARE! Windows needs 248 for the degree symbol!!
  calSteer0 = new QCheckBox(tr("0%1").arg(static_cast<char>(176)));
  calSteer90 = new QCheckBox(tr("90%1").arg(static_cast<char>(176)));
  calSteerLine = new QLineEdit("0");;
  calSteerLabel = new QLabel("Calibrate Steering");
  calDriveLabel = new QLabel("Calibrate Motors");
  calSteerLayout -> setSpacing(0);
  calSteerLayout -> setContentsMargins(0, 0, 0, 0);
  calSteerRightGrid -> setSpacing(0);
  calSteerRightGrid -> setContentsMargins(0, 0, 0, 0);
  calSteer0 -> setChecked(1);
  connect(calLamButton, SIGNAL(pressed()), this, SLOT(emitCalibrateLam()));
  connect(calSpindleButton, SIGNAL(pressed()),
          this, SLOT(emitCalibrateSpindle()));
  connect(calFingerButton, SIGNAL(pressed()),
          this, SLOT(emitCalibrateFingers()));
  connect(calDriveFront, SIGNAL(pressed()),
          this, SLOT(emitCalibrateMotorFront()));
  connect(calDriveBack, SIGNAL(pressed()),
          this, SLOT(emitCalibrateMotorRear()));
  connect(calSteerIs0, SIGNAL(pressed()),
          this, SLOT(emitSetSteeringStraight()));
  connect(calSteerIs90, SIGNAL(pressed()), this, SLOT(emitSetSteeringCross()));
  connect(saveConfig, SIGNAL(pressed()), this, SLOT(emitCalibrateSaveConfig()));
  connect(resetConfig, SIGNAL(pressed()),
          this, SLOT(emitCalibrateResetConfig()));
  connect(loadConfig, SIGNAL(pressed()),
          this, SLOT(emitCalibrateLoadConfig()));
  connect(calSteerLeftFront, SIGNAL(pressed()), this, SLOT(emitSetLeftFront()));
  connect(calSteerRightFront, SIGNAL(pressed()),
          this, SLOT(emitSetRightFront()));
  connect(calSteerLeftRear, SIGNAL(pressed()), this, SLOT(emitSetLeftRear()));
  connect(calSteerRightRear, SIGNAL(pressed()), this, SLOT(emitSetRightRear()));
  connect(calSteer0, SIGNAL(clicked(bool)),
          this, SLOT(setCalSteer0Checked(bool)));
  connect(calSteer90, SIGNAL(clicked(bool)),
          this, SLOT(setCalSteer90Checked(bool)));
  calibrateGroup -> setProperty("groupBox", true);
  calSteerGroup ->setObjectName("calSteerGroup");
  calSteerLabel -> setProperty("mntLabel", true);
  calDriveLabel -> setProperty("mntLabel", true);
  calSteerLine -> setObjectName("calSteerLine");
  calSteer0 -> setProperty("mntCheckBox", true);
  calSteer90 -> setProperty("mntCheckBox", true);
  calSteerLeftFront -> setProperty("mntCalSteerButton", true);
  calSteerRightFront -> setProperty("mntCalSteerButton", true);
  calSteerLeftRear -> setProperty("mntCalSteerButton", true);
  calSteerRightRear -> setProperty("mntCalSteerButton", true);
  calSteerLeftFront -> setObjectName("calSteerLeftFront");
  calSteerRightFront -> setObjectName("calSteerRightFront");
  calSteerLeftRear -> setObjectName("calSteerLeftRear");
  calSteerRightRear -> setObjectName("calSteerRightRear");
  saveConfig -> setProperty("mntButton", true);
  resetConfig -> setProperty("mntButton", true);
  loadConfig -> setProperty("mntButton", true);
  calSteerIs90 -> setProperty("mntButton", true);
  calSteerIs0 -> setProperty("mntButton", true);
  calDriveBack -> setProperty("mntButton", true);
  calDriveFront -> setProperty("mntButton", true);
  calFingerButton -> setProperty("mntButton", true);
  calSpindleButton -> setProperty("mntButton", true);
  calLamButton -> setProperty("mntButton", true);
  calibrateGrid -> addWidget(calLamButton, 0, 0);
  calibrateGrid -> addWidget(calSpindleButton, 0, 1);
  calibrateGrid -> addWidget(calFingerButton, 1, 0);
  calibrateGrid -> addWidget(calDriveFront, 2, 0);
  calibrateGrid -> addWidget(calDriveBack, 2, 1);
  calSteerLeftGrid -> addWidget(calSteer0, 0, 0, 1, 1);
  calSteerLeftGrid -> addWidget(calSteer90, 0, 1, 1, 1);
  calSteerLeftGrid -> addWidget(calSteerIs0, 1, 0, 1, 2);
  calSteerLeftGrid -> addWidget(calSteerIs90, 2, 0, 1, 2);
  calSteerRightGrid -> addWidget(calSteerLeftFront, 0, 0, 1, 1);
  calSteerRightGrid -> addWidget(calSteerRightFront, 0, 2, 1, 1);
  calSteerRightGrid -> addWidget(calSteerLine, 1, 1, 1, 1);
  calSteerRightGrid -> addWidget(calSteerLeftRear, 2, 0, 1, 1);
  calSteerRightGrid -> addWidget(calSteerRightRear, 2, 2, 1, 1);
  calSteerLayout -> addLayout(calSteerLeftGrid);
  calSteerLayout -> addLayout(calSteerRightGrid);
  calSteerGroup -> setLayout(calSteerLayout);
  calibrateGrid -> addWidget(calSteerGroup, 4 ,0, 1, 2);
  calibrateGrid -> addWidget(resetConfig, 5, 0);
  calibrateGrid -> addWidget(saveConfig, 5, 1);
  calibrateGrid -> addWidget(loadConfig, 6, 0);
  calibrateGrid -> setContentsMargins(0, 0, 0 , 0);
  calibrateGroup -> setLayout(calibrateGrid);

  //Grouping
  mainGrid -> addWidget(driveGroup, 0, 3);
  mainGrid -> addWidget(fingerGroup, 4, 3);
  mainGrid -> addWidget(lamGroup, 0, 1);
  mainGrid -> addWidget(spindleGroup, 2, 3);
  mainGrid -> addWidget(steerGroup, 2, 1, 3, 1);
  mainGrid -> addWidget(calibrateGroup, 0, 5, 5, 1);
  mainGrid -> setColumnStretch(0, 2);
  mainGrid -> setColumnStretch(1, 20);
  mainGrid -> setColumnStretch(2, 2);
  mainGrid -> setColumnStretch(3, 20);
  mainGrid -> setColumnStretch(4, 2);
  mainGrid -> setColumnStretch(5, 20);
  mainGrid -> setColumnStretch(6, 2);
  mainGrid -> setRowStretch(0, 20);
  mainGrid -> setRowStretch(1, 2);
  mainGrid -> setRowStretch(2, 20);
  mainGrid -> setRowStretch(3, 2);
  setLayout(mainGrid);

  cout << "MntTab built" << endl;
}

//------------------------------------------------------------------------------
void MaintenanceTab::setCalSteer0Checked(bool checked)
{
  if(checked)
  {
    calSteer90 ->setChecked(0);
  }
  else
  {
    calSteer90 ->setChecked(1);
  }
}

//------------------------------------------------------------------------------
void MaintenanceTab::setCalSteer90Checked(bool checked)
{
  if(checked)
  {
    calSteer0 ->setChecked(0);
  }
  else
  {
    calSteer0 ->setChecked(1);
  }
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitDriveSignal()
{
	int distance, speed, ctrl;

  distance = convertToInt(driveLine2 -> displayText());
  speed = convertToInt(driveLine1 -> displayText());
  ctrl = convertToInt(driveLine3 -> displayText());

	emit(mntDriveSignal(distance, speed, ctrl));
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitFingerSignal()
{
	int finger, angle;
  angle = convertToInt(fingerLine -> displayText());

  if (fingerFront -> isChecked() == 1 && fingerRear -> isChecked() == 1)
	{
		finger = 0;
	}
  else if (fingerFront -> isChecked() == 1 && fingerRear -> isChecked() == 0)
	{
		finger = 1;
	}
  else if(fingerFront -> isChecked() == 0 && fingerRear -> isChecked() == 1)
	{
		finger = -1;
	}

	emit(mntFingerSignal(finger, angle));
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitLAMSignal()
{

	int position, vmax, acceleration, brake;

  vmax = convertToInt(lamLine1 -> displayText());
  position = convertToInt(lamLine2 -> displayText());
  acceleration = convertToInt(lamLine3 -> displayText());
  brake = convertToInt(lamLine4 -> displayText());

	emit(mntLAMSignal(position, vmax, acceleration, brake));
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitSpindleSignal()
{
	int spindle, position;

    position = convertToInt(spindleLine -> displayText());

    if (spindleFront -> isChecked() == 1 && spindleRear -> isChecked() == 1)
		{
			spindle = 0;
		}
    else if (spindleFront -> isChecked() == 1
             && spindleRear -> isChecked() == 0)
		{
			spindle = 1;
		}
    else if(spindleFront -> isChecked() == 0
            && spindleRear -> isChecked() == 1)
		{
			spindle = -1;
		}

		emit(mntSpindleSignal(spindle, position));
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitSteerSignal()
{
	int degree, wheels, delay;
  degree = convertToInt(steerLine1 -> displayText());
  delay = convertToInt(steerLine2 -> displayText());
	wheels = 0;

  if (steerLeftRear -> isChecked() == 1)
	{
		wheels += 1;
	}
  if (steerRightRear -> isChecked() == 1)
	{
		wheels += 2;
	}
  if (steerRightFront -> isChecked() == 1)
	{
		wheels += 4;
	}
  if (steerLeftFront -> isChecked() == 1)
	{
		wheels += 8;
	}

	emit(mntSteerSignal(degree, wheels, delay));
}

//------------------------------------------------------------------------------
int MaintenanceTab::convertToInt(QString param)
{
	int newparam;
	newparam = param.toInt();
	return newparam;
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitCalibrateLam()
{
  emit(calibrateSignal(0, 1, 0, 0, 0));
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitCalibrateSpindle()
{
  emit(calibrateSignal(0, 4, 0, 0, 0));
  emit(calibrateSignal(0, 11, 0, 0, 0));
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitCalibrateFingers()
{
  emit(calibrateSignal(0, 3, 0, 0, 0));
  emit(calibrateSignal(0, 10, 0, 0, 0));
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitCalibrateMotorFront()
{
  emit(calibrateSignal(0, 0, 0, 0, 0));
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitCalibrateMotorRear()
{
  emit(calibrateSignal(0, 7, 0, 0, 0));
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitSetSteeringStraight()
{
  emit(calibrateSignal(1, 0, 0, 0, 0));
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitSetSteeringCross()
{
  emit(calibrateSignal(2, 0, 0, 0, 0));
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitSetLeftFront()
{
  if(calSteer0 -> isChecked() && calSteer90 -> isChecked() == 0)
  {
    emit(calibrateSignal(0, 2, 0, calSteerLine -> displayText().toInt(), 0));
  }
  else if (calSteer90 -> isChecked() && calSteer0 -> isChecked() == 0)
  {
    emit(calibrateSignal(0, 2, 1, calSteerLine -> displayText().toInt(), 0));
  }
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitSetRightFront()
{
  if(calSteer0 -> isChecked() && calSteer90 -> isChecked() == 0)
  {
    emit(calibrateSignal(0, 5, 0, calSteerLine -> displayText().toInt(), 0));
  }
  else if (calSteer90 -> isChecked() && calSteer0 -> isChecked() == 0)
  {
    emit(calibrateSignal(0, 5, 1, calSteerLine -> displayText().toInt(), 0));
  }
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitSetLeftRear()
{
  if(calSteer0 -> isChecked() && calSteer90 -> isChecked() == 0)
  {
    emit(calibrateSignal(0, 12, 0, calSteerLine -> displayText().toInt(), 0));
  }
  else if (calSteer90 -> isChecked() && calSteer0 -> isChecked() == 0)
  {
    emit(calibrateSignal(0, 12, 1, calSteerLine -> displayText().toInt(), 0));
  }
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitSetRightRear()
{
  if(calSteer0 -> isChecked() && calSteer90 -> isChecked() == 0)
  {
    emit(calibrateSignal(0, 9, 0, calSteerLine -> displayText().toInt(), 0));
  }
  else if (calSteer90 -> isChecked() && calSteer0 -> isChecked() == 0)
  {
    emit(calibrateSignal(0, 9, 1, calSteerLine -> displayText().toInt(), 0));
  }
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitCalibrateSaveConfig()
{
  emit(calibrateSignal(3, 0, 0, 0, 0));
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitCalibrateResetConfig()
{
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Reset",
                                "<font size = 5>Reset Configuration?</font>",
                                QMessageBox::Yes|QMessageBox::No);

  if (reply == QMessageBox::Yes) {
   //emit(calibrateSignal(4, 0, 0, 0, 0));
  }
}

//------------------------------------------------------------------------------
void MaintenanceTab::emitCalibrateLoadConfig()
{
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Load",
                                "<font size = 5>"
                                "Load Standart Configuration?</font>",
                                QMessageBox::Yes|QMessageBox::No);

  if (reply == QMessageBox::Yes) {
   emit(calibrateSignal(4, 0, 0, 0, 0));
  }
}
