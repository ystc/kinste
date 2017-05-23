/**
  * Filename: SequenceTab.h
  *
  * Description: Builds everything thats needed to create/save a Shuttle sequence.
  *   This class is the main reason for this project, because it was the target to create
  *   a GUI, where everyone can easily create and start a test (sequence) for the Shuttles.
  *   The commands are added via Buttons+Lines and are saved in a QStringList. The list  is
  *   displayed on a QListWidget in the center, where you can always see what command is
  *   currently processed. After the test you can save a statistic with the most important
  *   collected data.
  *
  * Author: Christoph Steinkellner
  * Created: 04.05.2016
  * Last change: 11.08.2016
  */

#ifndef SEQUENCETAB_H_
#define SEQUENCETAB_H_

#include <iostream>
#include <string>
#include <QWidget>
#include <QTimer>
#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QTextStream>
#include <QChar>

using std::string;
using std::cout;
using std::endl;

class SequenceTab : public QWidget
{
	Q_OBJECT

	public:
    SequenceTab(QWidget* parent = 0);
		~SequenceTab();

    void setConnectionStatus(bool connection_status);
    void lastCommandSuccessful(bool successful, QString status_text);
    void setLevel(QString current_level);

    QComboBox *scomboMoveTo, *scomboLoad, *scomboUnload;

  protected:
    //bool eventFilter(QObject *obj, QEvent *ev);

	private:
		SequenceTab(const SequenceTab& original);

    void sequence();
    void buildLayout();
    void disable();
    void enable();
		void editCommandList(QString command, QString position, int command_index);
    void addToCommandList(QString list_command, QString command, QString position);
		void addToCommandList(QString command, QString position, int command_index);
		void keyPressEvent(QKeyEvent *event);
    void closeErrorWindow();
    int validateLoadedData(QString data_string);
    void errorHandler(QString error_text);
    void decreaseListNumbers(int current_position);
    void increaseListNumbers(int current_position);

		QGridLayout *mainGrid, *leftGrid, *rightGrid, *topGrid, *botGrid;
    QGroupBox *topGroup, *botGroup, *leftGroup;
    QVBoxLayout *vbox;
    QVBoxLayout *hbox;

    QWidget *editWindow;
		QGridLayout *editGrid;
    QPushButton *editCancel, *editOk;
    QLineEdit *editPosition;
    QComboBox *editCommands;
    QTimer *mainTimer, *pauseTimer, *signalTimerSequence, *errTimer;
    QPushButton 	*saveSeqButton, *loadSeqButton, *deleteCmdButton,
      *startSeqButton, *endSeqButton, *statisticButton,
      *sbuttonPause, *sbuttonMoveTo, *sbuttonLoad, *sbuttonUnload,
      *sbuttonLoop, *restartSeqButton;
    QListWidget *listWidget;
    QLineEdit *slinePause, *slineLoop;
    QLabel *slabelPause, *slabelMoveTo, *slabelLoad, *slabelUnload, *timeLine,
      *slabelLoop;
    QString filename, time_format, error_list, actual_level;
    QPoint globalPos;

    QStringList original_command_list;
    QStringList loop_save;

    int main_list_index, row_counter, loop, loop_counter, loop_check, loop_index, resend_counter,
      hours, minutes, seconds, errors, moves, unloads, loads, errtime, move_over_positions_size,
      pause_in_seconds, pause_restore, shuttle_port, last_command_time, row_count;
    bool first_loop_flag, endseq, first_command_flag, shuttle_connection_status, even,
      param1_is_valid, param2_is_valid;

    enum timer_values {ONE_SEC = 1000, FIVE_SEC = 5000, TEN_SEC = 10000, FIFTEEN_SEC = 15000,
      TWENTY_SEC = 20000, THIRTY_SEC = 30000};

  signals:
    void getCurrentLevel();
    void saveUsedPosition(QString last_position_used);
    void blinkingErrorFrame();
    void sequenceStarted(bool started);
    void moveTo(QStringList move_paramters);
    void loadAt(QString rackchannel);
    void unloadAt(QString rackchannel);
    void buildStatusMessage(QString status_text);
    void startSeqButtonClicked();
    void endSeqButtonClicked();
    void restartSeqButtonClicked();
    void statisticButtonClicked();
    void statusMessage(QString param1, QString param2, QString param3, QString param4);

	private slots:
		void saveSeqButtonSlot();
		void loadSeqButtonSlot();
		void deleteCommandButtonSlot();
    void deleteAllButtonSlot();
    void startSeqButtonSlot();
    void restartSeqButtonSlot();
		void loopButtonSlot();
		void moveButtonSlot();
		void loadButtonSlot();
		void unloadButtonSlot();
		void pauseButtonSlot();
		void endSeqButtonSlot();
		void showContextMenu(QPoint pos);
		void contextEraseItem();
    void contextEditItem();
    void contextAddItem();
		void cancelClicked();
		void okClicked();
		void addOkClicked();
		void statisticButtonSlot();
    void pauseEnded();
    void sequenceSignalTimeOut();
    void time();
    void errTime();
};

#endif //SEQUENCETAB_H_
