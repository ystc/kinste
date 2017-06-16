/**
  * Filename: MaintenanceTab.h
  * Description: Additional Shuttle functions mainly for testing and solving
  *  errors.
  *  Also includes functions to calibrate the Shuttle for first operation.
  * Author: ystc
  * Created: 04.05.2016
  */

#ifndef MAINTENANCETAB_H_
#define MAINTENANCETAB_H_

#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QMessageBox>

using std::cout;
using std::endl;

class MaintenanceTab: public QWidget
{
	Q_OBJECT

	public:
    MaintenanceTab(QWidget *parent = 0);
		~MaintenanceTab();


	private:
    void buildLayout();

		MaintenanceTab(const MaintenanceTab& original);

    QGridLayout *mainGrid, *driveGrid, *fingerGrid, *lamGrid, *spindleGrid, *steerGrid,
      *calibrateGrid, *calSteerLeftGrid, *calSteerRightGrid;
    QPushButton *driveButton, *fingerButton, *lamButton, *spindleButton, *steerButton,
      *calLamButton, *calSpindleButton, *calFingerButton, *calSteerLeftFront, *calSteerRightFront,
      *calSteerLeftRear, *calSteerRightRear, *calSteerIs0, *calSteerIs90, *calDriveFront,
      *calDriveBack, *saveConfig, *resetConfig, *loadConfig;
    QCheckBox *fingerFront, *fingerRear, *spindleFront, *spindleRear, *steerLeftFront,
      *steerLeftRear, *steerRightFront, *steerRightRear, *calSteer0, *calSteer90;
    QLineEdit *driveLine1, *driveLine2, *driveLine3, *fingerLine, *lamLine1, *lamLine2,
      *lamLine3, *lamLine4, *spindleLine, *steerLine1, *steerLine2, *calSteerLine;
    QGroupBox *driveGroup, *fingerGroup, *lamGroup, *spindleGroup, *steerGroup, *calibrateGroup,
      *calSteerGroup;
    QLabel *driveLabel1, *driveLabel2, *driveLabel3, *fingerLabel, *lamLabel1, *lamLabel2,
      *lamLabel3, *lamLabel4, *spindleLabel, *steerLabel1, *steerLabel2, *calSteerLabel,
      *calDriveLabel;
    QHBoxLayout *calSteerLayout;

		int convertToInt(QString param);
		unsigned char convertToChar(QString param);

	private slots:
		void emitDriveSignal();
		void emitFingerSignal();
		void emitLAMSignal();
		void emitSteerSignal();
		void emitSpindleSignal();
    void emitCalibrateLam();
    void emitCalibrateSpindle();
    void emitCalibrateFingers();
    void emitCalibrateMotorFront();
    void emitCalibrateMotorRear();
    void emitSetSteeringStraight();
    void emitSetSteeringCross();
    void emitSetLeftFront();
    void emitSetRightFront();
    void emitSetLeftRear();
    void emitSetRightRear();
    void emitCalibrateSaveConfig();
    void emitCalibrateResetConfig();
    void emitCalibrateLoadConfig();
    void setCalSteer0Checked(bool checked);
    void setCalSteer90Checked(bool checked);

	signals:
		void mntDriveSignal(int distance, int speed, unsigned char ctrl);
		void mntFingerSignal(int finger, int angle);
		void mntLAMSignal(int position, int vmax, int acceleration, int brake);
		void mntSteerSignal(int degree, int wheels, int delay);
		void mntSpindleSignal(int spindle, int position);
    void calibrateSignal(int sub_cmd, int motor_nr, int steer_context, int diff, int opt_diff);
};

#endif //MAINTENANCETAB_H_
