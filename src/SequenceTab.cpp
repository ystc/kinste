//------------------------------------------------------------------------------
// Filename: SequenceTab.cpp
// Author: Christoph Steinkellner
// Created: 04.05.2016
// Last change: 11.08.2016
//------------------------------------------------------------------------------

#include "inc/SequenceTab.h"

//------------------------------------------------------------------------------
SequenceTab::SequenceTab(QWidget *parent) : QWidget(parent)
{
  hours = 0;
  minutes = 0;
  seconds = 0;
  main_list_index = 0;

  buildLayout();

  pauseTimer = new QTimer();
  connect(pauseTimer, SIGNAL(timeout()), this, SLOT(pauseEnded()));

  signalTimerSequence = new QTimer();
  signalTimerSequence -> setSingleShot(1);
  connect(signalTimerSequence, SIGNAL(timeout()),
    this, SLOT(sequenceSignalTimeOut()));

  errTimer = new QTimer();

  mainTimer = new QTimer();
  connect(mainTimer, SIGNAL(timeout()), this, SLOT(time()));
  timeLine -> show();
}

//------------------------------------------------------------------------------
SequenceTab::~SequenceTab()
{

}

//------------------------------------------------------------------------------
void SequenceTab::buildLayout()
{
  mainGrid = new QGridLayout();
  leftGrid = new QGridLayout();
  rightGrid = new QGridLayout();
  topGrid = new QGridLayout();
  topGroup = new QGroupBox();
  leftGroup = new QGroupBox();
  botGrid = new QGridLayout();
  botGroup = new QGroupBox();
  vbox = new QVBoxLayout();
  listWidget = new QListWidget();
  row_count = 0;

  timeLine = new QLabel();
  timeLine -> setObjectName("timer");

  timeLine -> hide();
  saveSeqButton = new QPushButton("Save");
  saveSeqButton -> setProperty("executeButton", true);
  connect(saveSeqButton, SIGNAL(pressed()), this, SLOT(saveSeqButtonSlot()));

  loadSeqButton = new QPushButton("Load");
  loadSeqButton -> setProperty("executeButton", true);
  connect(loadSeqButton, SIGNAL(pressed()), this, SLOT(loadSeqButtonSlot()));

  deleteCmdButton = new QPushButton("Delete");
  deleteCmdButton -> setProperty("executeButton", true);
  deleteCmdButton -> setToolTip("Deletes last command.");
  connect(deleteCmdButton, SIGNAL(pressed()),
          this, SLOT(deleteAllButtonSlot()));

  statisticButton = new QPushButton("Save log");
  statisticButton -> setProperty("executeButton", true);
  connect(statisticButton, SIGNAL(pressed()),
          this, SLOT(statisticButtonSlot()));
  statisticButton -> setEnabled(0);

  endSeqButton = new QPushButton("Stop");
  endSeqButton -> setProperty("executeButton", true);
  connect(endSeqButton, SIGNAL(pressed()), this, SLOT(endSeqButtonSlot()));

  startSeqButton = new QPushButton("Start");
  startSeqButton -> setProperty("executeButton", true);
  connect(startSeqButton, SIGNAL(pressed()),
    this, SLOT(startSeqButtonSlot()));

  restartSeqButton = new QPushButton("Restart");
  restartSeqButton -> setProperty("executeButton", true);
  connect(restartSeqButton, SIGNAL(pressed()),
      this, SLOT(restartSeqButtonSlot()));
  restartSeqButton -> setEnabled(0);

  slabelMoveTo = new QLabel("Move to: ");
  slabelMoveTo -> setProperty("executeLabel", true);

  scomboMoveTo = new QComboBox();
  scomboMoveTo -> setEditable(1);
  scomboMoveTo -> setProperty("executeLine", true);
  scomboMoveTo -> setMaxVisibleItems(10);
  connect(scomboMoveTo -> lineEdit(), SIGNAL(returnPressed()),
    this, SLOT(moveButtonSlot()));
  scomboMoveTo -> setToolTip ("Separate single positions with comma to "
    "add more than one to the sequence list!");
  scomboMoveTo -> lineEdit() -> installEventFilter(this);

  sbuttonMoveTo = new QPushButton("Add");
  sbuttonMoveTo -> setProperty("executeButton", true);
  connect(sbuttonMoveTo, SIGNAL(pressed()), this, SLOT(moveButtonSlot()));

  slabelLoad = new QLabel("Load Carrier: ");
  slabelLoad -> setProperty("executeLabel", true);

  scomboLoad = new QComboBox();
  scomboLoad -> setProperty("executeLine", true);
  scomboLoad -> setEditable(1);
  scomboLoad -> setMaxVisibleItems(10);
  connect(scomboLoad -> lineEdit(), SIGNAL(returnPressed()),
    this, SLOT(loadButtonSlot()));
  scomboLoad -> setToolTip("Only enter rack channels.\n"
    "For example: S01.001.01.01.001 .L or .R.");
  scomboLoad -> lineEdit() -> installEventFilter(this);

  sbuttonLoad = new QPushButton("Add");
  sbuttonLoad -> setProperty("executeButton", true);
  connect(sbuttonLoad, SIGNAL(pressed()), this, SLOT(loadButtonSlot()));

  slabelUnload = new QLabel("Unload Carrier: ");
  slabelUnload -> setProperty("executeLabel", true);

  scomboUnload = new QComboBox();
  scomboUnload -> setEditable(1);
  scomboUnload -> setProperty("executeLine", true);
  scomboUnload -> setMaxVisibleItems(10);
  connect(scomboUnload -> lineEdit(), SIGNAL(returnPressed()),
    this, SLOT(unloadButtonSlot()));
  scomboUnload -> setToolTip("Only enter rack channels.\n"
    "For example: S01.001.01.01.0001 .L or .R.");
  scomboUnload -> lineEdit() -> installEventFilter(this);

  sbuttonUnload = new QPushButton("Add");
  sbuttonUnload -> setProperty("executeButton", true);
  connect(sbuttonUnload, SIGNAL(pressed()), this, SLOT(unloadButtonSlot()));

  slabelLoop = new QLabel("Loops: ");
  slabelLoop -> setProperty("executeLabel", true);

  slineLoop = new QLineEdit();
  slineLoop -> setProperty("executeLine", true);
  slineLoop -> setToolTip
    ("If you enter -1 the sequence will loop to infinity and beyond.");
  connect(slineLoop, SIGNAL(returnPressed()), this, SLOT(loopButtonSlot()));

  sbuttonLoop = new QPushButton("Add");
  sbuttonLoop -> setProperty("executeButton", true);
  connect(sbuttonLoop, SIGNAL(pressed()), this, SLOT(loopButtonSlot()));

  slabelPause = new QLabel("Pause in seconds: ");
  slabelPause -> setProperty("executeLabel", true);

  slinePause = new QLineEdit();
  slinePause -> setProperty("executeLine", true);
  connect(slinePause, SIGNAL(returnPressed()), this, SLOT(pauseButtonSlot()));

  sbuttonPause = new QPushButton("Add");
  sbuttonPause -> setProperty("executeButton", true);
  connect(sbuttonPause, SIGNAL(pressed()), this, SLOT(pauseButtonSlot()));

  listWidget -> setContextMenuPolicy(Qt::CustomContextMenu);
  connect(listWidget, SIGNAL(customContextMenuRequested(QPoint)), this,
    SLOT(showContextMenu(QPoint)));
  listWidget -> setObjectName("listWidget");

  topGrid -> addWidget(loadSeqButton, 0, 0);
  topGrid -> addWidget(saveSeqButton, 1, 0);
  topGrid -> addWidget(deleteCmdButton, 2, 0);
  topGrid -> addWidget(statisticButton, 3, 0);

  topGroup -> setLayout(topGrid);
  topGroup ->  setProperty("groupBox", true);
  topGroup->setTitle("Options");

  botGrid -> addWidget(startSeqButton, 0, 0);
  botGrid -> addWidget(endSeqButton, 2, 0);
  botGrid -> addWidget(restartSeqButton, 1, 0);
  botGroup -> setLayout(botGrid);
  botGroup ->  setProperty("groupBox", true);
  botGroup->setTitle("Sequence Controls");

  rightGrid -> addWidget(topGroup, 1, 0);
  rightGrid -> addWidget(botGroup, 3, 0);
  rightGrid->setColumnStretch(0, 15);
  rightGrid->setColumnStretch(1, 10);
  rightGrid->setRowStretch(0, 0);
  rightGrid->setRowStretch(1, 45);
  rightGrid->setRowStretch(2, 5);
  rightGrid->setRowStretch(3, 25);
  rightGrid->setContentsMargins(0, 20, 0, 0);

  leftGrid -> addWidget(slabelMoveTo, 1, 0);
  leftGrid -> addWidget(scomboMoveTo, 2, 0);
  leftGrid -> addWidget(sbuttonMoveTo, 2, 1);

  leftGrid -> addWidget(slabelLoad, 3, 0);
  leftGrid -> addWidget(scomboLoad, 4, 0, 1, 1);
  leftGrid -> addWidget(sbuttonLoad, 4, 1);

  leftGrid -> addWidget(slabelUnload, 5, 0);
  leftGrid -> addWidget(scomboUnload, 6, 0);
  leftGrid -> addWidget(sbuttonUnload, 6, 1);

  leftGrid -> addWidget(slabelLoop, 7, 0);
  leftGrid -> addWidget(slineLoop, 8, 0);
  leftGrid -> addWidget(sbuttonLoop, 8, 1);

  leftGrid -> addWidget(slabelPause, 9, 0);
  leftGrid -> addWidget(slinePause, 10, 0);
  leftGrid -> addWidget(sbuttonPause, 10, 1);

  leftGrid -> setColumnStretch(0, 30);
  leftGrid -> setColumnStretch(1, 10);
  leftGroup->setLayout(leftGrid);
  leftGroup->setTitle("List Controls");
  leftGroup->setObjectName("leftGroup");
  //leftGroup->setContentsMargins(0, 0, 0, 0);

  vbox -> addWidget(timeLine);
  vbox -> addWidget(listWidget);

  mainGrid -> addWidget(leftGroup, 0, 1);
  mainGrid -> addLayout(vbox, 0, 3);
  mainGrid -> addLayout(rightGrid, 0, 5);

  mainGrid -> setColumnStretch(0, 5);
  mainGrid -> setColumnStretch(1, 30);
  mainGrid -> setColumnStretch(2, 1);
  mainGrid -> setColumnStretch(3, 50);
  mainGrid -> setColumnStretch(4, 1);
  mainGrid -> setColumnStretch(5, 30);
  mainGrid->setRowStretch(0, 99);
  mainGrid->setRowStretch(1, 1);

  mainGrid->setContentsMargins(0, 0, 0, 20);

  setLayout(mainGrid);
}

//------------------------------------------------------------------------------
void SequenceTab::setConnectionStatus(bool connection_status)
{
  shuttle_connection_status = connection_status;
}

//------------------------------------------------------------------------------
void SequenceTab::enable()
{
  listWidget -> setEnabled(1);
  startSeqButton -> setEnabled(1);
  loadSeqButton -> setEnabled(1);
  saveSeqButton -> setEnabled(1);
  sbuttonMoveTo -> setEnabled(1);
  sbuttonLoad -> setEnabled(1);
  sbuttonUnload -> setEnabled(1);
  sbuttonPause -> setEnabled(1);
  slineLoop -> setEnabled(1);
  deleteCmdButton -> setEnabled(1);
  scomboMoveTo -> setEnabled(1);
  scomboLoad -> setEnabled(1);
  scomboUnload -> setEnabled(1);
  slinePause -> setEnabled(1);
  restartSeqButton -> setEnabled(1);
  sbuttonLoop -> setEnabled(1);
}

//------------------------------------------------------------------------------
void SequenceTab::disable()
{
  listWidget -> setEnabled(0);
  startSeqButton -> setEnabled(0);
  loadSeqButton -> setEnabled(0);
  saveSeqButton -> setEnabled(0);
  sbuttonMoveTo -> setEnabled(0);
  sbuttonLoad -> setEnabled(0);
  sbuttonUnload -> setEnabled(0);
  sbuttonPause -> setEnabled(0);
  slineLoop -> setEnabled(0);
  slinePause -> setEnabled(0);
  deleteCmdButton -> setEnabled(0);
  scomboMoveTo -> setEnabled(0);
  scomboLoad -> setEnabled(0);
  scomboUnload -> setEnabled(0);
  restartSeqButton -> setEnabled(0);
  sbuttonLoop -> setEnabled(0);
}

//------------------------------------------------------------------------------
void SequenceTab::moveButtonSlot()
{
  QString line_text;
  QStringList position_strings;

	line_text = scomboMoveTo -> currentText();
  line_text.simplified();
  line_text.replace(" ", "");
  line_text.replace("\n", "");

	position_strings = line_text.split(",");

	for(int index = 0; index < position_strings.size(); index++)
  {
    line_text = QString::number(row_count + 1) + ". Move to: ";
		line_text += position_strings.at(index);
		listWidget -> addItem(line_text);
		listWidget -> setCurrentRow(row_count++);

		original_command_list.append("drive");
		original_command_list.append(position_strings.at(index));
	}

  line_text = scomboMoveTo -> currentText();
  emit(saveUsedPosition(line_text));
}

//------------------------------------------------------------------------------
void SequenceTab::loadButtonSlot()
{
	QString line_text;

  line_text = QString::number(row_count + 1) + ". Load: ";
	line_text += scomboLoad -> currentText();
	listWidget -> addItem(line_text);
	listWidget -> setCurrentRow(row_count++);

	original_command_list.append("load");
	original_command_list.append(scomboLoad -> currentText());

  line_text = scomboLoad -> currentText();
  emit(saveUsedPosition(line_text));
}

//------------------------------------------------------------------------------
void SequenceTab::unloadButtonSlot()
{
	QString line_text;

  line_text = QString::number(row_count + 1) + ". Unload: ";
	line_text += scomboUnload -> currentText();
	listWidget -> addItem(line_text);
	listWidget -> setCurrentRow(row_count++);

	original_command_list.append("unload");
	original_command_list.append(scomboUnload -> currentText());

  line_text = scomboUnload -> currentText();
  emit(saveUsedPosition(line_text));
}

//------------------------------------------------------------------------------
void SequenceTab::pauseButtonSlot()
{
  QString line_text;

  line_text += QString::number(row_count + 1) + ". Pause: ";
	line_text += slinePause -> displayText();
  line_text += "s";
	listWidget -> addItem(line_text);
	listWidget -> setCurrentRow(row_count++);

  original_command_list.append("pause");
	original_command_list.append(slinePause -> displayText());
}

//------------------------------------------------------------------------------
void SequenceTab::loopButtonSlot()
{
  QString loop_line_text;

  loop_line_text = slineLoop -> displayText();

  if(loop_line_text.toInt() < 0)
  {
    listWidget -> addItem(QString::number(row_count + 1) + ". Loop");
  }
  else
  {
    loop_line_text = QString::number(row_count + 1) + ". Loops: ";
    loop_line_text += slineLoop -> displayText();
    listWidget -> addItem(loop_line_text);
    listWidget -> setCurrentRow(row_count++);
  }

  original_command_list.append("loop");
  original_command_list.append(slineLoop -> displayText());
}

//------------------------------------------------------------------------------
void SequenceTab::saveSeqButtonSlot()
{
	QString filename;
	QString list;
  //const char* text;

  filename = QFileDialog::getSaveFileName(this, "Save File", ".txt",
                                          "Textdateien (*.txt)");

	list = original_command_list.join("/");

	QFile file(filename);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    QMessageBox::warning(this, "Error",
                         "<font size = 5>File could not be saved!</font>",
                         QMessageBox::Ok);
	}
	else
  {
    //text = list.toLatin1().data();
    cout << "save list data: " << list.toStdString().data() << endl;
    file.write(list.toStdString().data());
    QMessageBox::information(this, "Success!",
                             "<font size = 5>File saved</font>",
                             QMessageBox::Ok);
	}
}

//------------------------------------------------------------------------------
int SequenceTab::validateLoadedData(QString data_string)
{
  if(even)
  {
    even = 0;
    QRegExp rx("(drive|load|pause|loop|unload)");

    rx.indexIn(data_string);
    QStringList list = rx.capturedTexts();

    if(list[0] == data_string)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    even = 1;
    QRegExp rx("(S\\d\\d\\.\\d\\d\\d\\.((\\d\\w)|(\\d\\d))"
               "\\.((\\d\\d)|(\\d\\w))"
               "\\.((\\d\\d\\d\\d)|(\\w\\d\\d)|(\\d\\d\\d\\d"
               "\\.[RL])))|(-?\\d+)");

    rx.indexIn(data_string);
    QStringList list = rx.capturedTexts();

    if(list[0] == data_string)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

//------------------------------------------------------------------------------
void SequenceTab::loadSeqButtonSlot()
{
  even = 1;
  QString data;
  QString line_text;
  QStringList command_strings;

  filename = QFileDialog::getOpenFileName(this, "Open File", "",
                                          "Textdateien (*.txt)");
	QFile file(filename);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QMessageBox::warning(this, "Error",
                         "<font size = 5>File could not be loaded!</font>",
                         QMessageBox::Ok);
  }
  else
  {
    data += file.readAll();
  }

  data.simplified();
  data.replace(" ", "");
  data.replace("\n", "");

  cout << "Loaded Data: " << data.toStdString() << endl;

  emit(getCurrentLevel());
  data.replace(QRegExp("\\.\\d\\d\\d\\."), tr(".%1").arg(actual_level));

  if(data.size() > 0)
  {
    command_strings = data.split("/");
  }

  for (int index = 0; index < command_strings.size();)
  {
    param1_is_valid = validateLoadedData(command_strings[index]);

    if(command_strings.size() > 1)
    {
      param2_is_valid = validateLoadedData(command_strings[index + 1]);
    }

    if (command_strings[index] == "drive" && param1_is_valid && param2_is_valid)
    {
      addToCommandList(QString::number(row_count + 1) + ". Move to: ",
                       command_strings[index], command_strings[index + 1]);
      index = index + 2;
    }
    else if (command_strings[index] == "load"
             && param1_is_valid && param2_is_valid)
    {
      addToCommandList(QString::number(row_count + 1) + ". Load: ",
                       command_strings[index], command_strings[index + 1]);
      index = index + 2;
    }
    else if (command_strings[index] == "unload" && param1_is_valid
             && param2_is_valid)
    {
      addToCommandList(QString::number(row_count + 1) + ". Unload: ",
                       command_strings[index], command_strings[index + 1]);
      index = index + 2;
    }
    else if (command_strings[index] == "pause"
             && param1_is_valid && param2_is_valid)
    {
      QString time = command_strings[index + 1];
      addToCommandList(QString::number(row_count + 1) + ". Pause: ",
                       command_strings[index], time + " s");
      index = index + 2;
    }
    else if (command_strings[index] == "loop"
             && param1_is_valid && param2_is_valid)
    {
      addToCommandList(QString::number(row_count + 1) + ". Loops: ",
                       command_strings[index], command_strings[index + 1]);
      index = index + 2;
    }
    else if(command_strings[index] == "")
    {
      index++;
    }
    else
    {
     cout << "Invalid command: "<< command_strings[index].toStdString()
          << command_strings[index + 1].toStdString() << endl;
      emit(buildStatusMessage("Invalid File, data could not be loaded!"));
      cout << "Invalid File, abort loading data!" << endl;
      break;
    }
    row_count++;
  }

  cout << "File loaded successfully" << endl;
}

//------------------------------------------------------------------------------
void SequenceTab::setLevel(QString current_level)
{
  actual_level = current_level;
}

//------------------------------------------------------------------------------
void SequenceTab::addToCommandList(QString list_command, QString command,
                                   QString position)
{
  QString build_text;

  build_text += list_command;
  build_text += position;
  listWidget -> addItem(build_text);
  build_text.clear();

  original_command_list += command;
  original_command_list += position.split(" ")[0];
}

//------------------------------------------------------------------------------
void SequenceTab::deleteAllButtonSlot()
{
	int list_size = original_command_list.size();

  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Delete Sequence",
                                "<font size = 5>Delete "
                                " the entire sequence?</font>",
                                QMessageBox::Yes|QMessageBox::No);

  row_count = 0;
  if (reply == QMessageBox::Yes) {
    for(int list_count = 0; list_count < list_size; list_count++){
      original_command_list.removeAt(0);

      if(list_count < list_size/2){
        listWidget->takeItem(0);
      }
    }
  }
}

//------------------------------------------------------------------------------
void SequenceTab::deleteCommandButtonSlot()
{
  int list_size = original_command_list.size();
  int current = listWidget -> currentRow();

  if(listWidget->selectedItems().isEmpty()){
    emit(buildStatusMessage("No entry selected"));
    return;
  }

  if (list_size == 0)
  {
    emit(buildStatusMessage("No entries in sequence list!"));
  }
  else
  {
    listWidget -> takeItem(current);
    row_count--;

    if (current == 0)
    {
      original_command_list.removeAt(0);
      original_command_list.removeAt(0);
    }
    else
    {
      original_command_list.removeAt(current * 2);
      original_command_list.removeAt(current * 2);
    }
  }

  if(current < row_count){
    listWidget -> setCurrentRow(current);
  }
  else{
    listWidget -> setCurrentRow(current - 1);
  }
  decreaseListNumbers(listWidget->currentRow());
}

//------------------------------------------------------------------------------
void SequenceTab::showContextMenu(QPoint pos)
{
	QPoint globalPos = listWidget -> mapToGlobal(pos);

	QMenu myMenu;
  myMenu.addAction("Add", this, SLOT(contextAddItem()));
  myMenu.addAction("Delete", this, SLOT(contextEraseItem()));
  myMenu.addAction("Edit", this, SLOT(contextEditItem()));

	myMenu.exec(globalPos);
}

//------------------------------------------------------------------------------
void SequenceTab::contextEditItem()
{
	listWidget -> setEnabled(0);
	editWindow = new QWidget();
	editGrid = new QGridLayout();
  editOk = new QPushButton("Ok");
  editCancel = new QPushButton("Cancel");
  editPosition = new QLineEdit();
  editCommands = new QComboBox();

  int current = listWidget -> currentRow();

  editPosition -> setFixedSize(150,30);
  editOk -> setFixedSize(50, 30);
  editCommands -> setFixedSize(100, 30);
  editCommands -> addItem("Move");
  editCommands -> addItem("Load");
  editCommands -> addItem("Unload");
  editCommands -> addItem("Pause");
  editCommands -> addItem("Loop");
  editGrid -> addWidget(editOk, 3, 1);
  editGrid -> addWidget(editCancel, 3, 0);
  editGrid -> addWidget(editPosition, 0, 0);
  editGrid -> addWidget(editCommands, 0, 1);

  editPosition -> setText(original_command_list[current*2+1]);

  if(original_command_list[current*2] == "Move")
  {
    editCommands -> setCurrentIndex(0);
  }
  else if(original_command_list[current*2] == "Load")
  {
    editCommands -> setCurrentIndex(1);
  }
  else if(original_command_list[current*2] == "Unload")
  {
    editCommands -> setCurrentIndex(2);
  }
  else if(original_command_list[current*2] == "Pause")
  {
    editCommands -> setCurrentIndex(3);
  }
  else if(original_command_list[current*2] == "Loop")
  {
      editCommands -> setCurrentIndex(4);
  }

  connect(editCancel, SIGNAL(pressed()), this, SLOT(cancelClicked()));
  connect(editOk, SIGNAL(pressed()), this, SLOT(okClicked()));
  connect(editPosition, SIGNAL(returnPressed()), this, SLOT(okClicked()));

	editWindow -> setWindowFlags(Qt::Window | Qt::CustomizeWindowHint
	  | Qt::WindowTitleHint| Qt::WindowSystemMenuHint);
	editWindow -> setLayout(editGrid);
	editWindow -> setWindowTitle(tr("Edit"));
	editWindow -> setFixedSize(300, 100);
	editWindow -> move( mapToGlobal( QPoint( 150, 100 ) ) );
	editWindow -> show();
	editWindow -> setAttribute( Qt::WA_QuitOnClose, false );
}

//------------------------------------------------------------------------------
void SequenceTab::contextAddItem()
{
  listWidget -> setEnabled(0);
  editWindow = new QWidget();
  editGrid = new QGridLayout();
  editOk = new QPushButton("Ok");
  editCancel = new QPushButton("Cancel");
  editPosition = new QLineEdit();
  editCommands = new QComboBox();

  editPosition -> setFixedSize(150,30);
  editOk -> setFixedSize(50, 30);
  editCommands -> setFixedSize(100, 30);
  editCommands -> addItem("Move");
  editCommands -> addItem("Load");
  editCommands -> addItem("Unload");
  editCommands -> addItem("Pause");
  editCommands -> addItem("Loop");
  editGrid -> addWidget(editOk, 3, 1);
  editGrid -> addWidget(editCancel, 3, 0);
  editGrid -> addWidget(editPosition, 0, 0);
  editGrid -> addWidget(editCommands, 0, 1);

  connect(editCancel, SIGNAL(pressed()), this, SLOT(cancelClicked()));
  connect(editOk, SIGNAL(pressed()), this, SLOT(addOkClicked()));
  connect(editPosition, SIGNAL(returnPressed()), this, SLOT(addOkClicked()));

  editWindow -> setWindowFlags(Qt::Window | Qt::CustomizeWindowHint
    | Qt::WindowTitleHint| Qt::WindowSystemMenuHint);
  editWindow -> setLayout(editGrid);
  editWindow -> setWindowTitle("Add");
  editWindow -> setFixedSize(300, 100);
  editWindow -> move( mapToGlobal( QPoint( 150, 100 ) ) );
  editWindow -> show();
  editWindow -> setAttribute( Qt::WA_QuitOnClose, false );
}

//------------------------------------------------------------------------------
void SequenceTab::cancelClicked()
{
	listWidget -> setEnabled(1);
	editWindow -> close();
}

//------------------------------------------------------------------------------
void SequenceTab::okClicked()
{
	listWidget -> setEnabled(1);
	int current_cmd = listWidget -> currentRow();
	int current_pos = listWidget -> currentRow();

  /*QListWidgetItem *item =*/ listWidget -> takeItem(current_pos);
  //delete item;

	current_cmd *= 2;

  if(editCommands -> currentText() == "Move")
	{
    listWidget -> insertItem(current_pos, QString::number(current_pos + 1)
                             + ". Move to: " + editPosition -> displayText());
    editCommandList("drive", editPosition -> displayText(), current_cmd);
	}
  else if(editCommands -> currentText() == "Load")
	{
    listWidget -> insertItem(current_pos, QString::number(current_pos + 1)
                             + ". Load: " + editPosition -> displayText());
    editCommandList("load", editPosition -> displayText(), current_cmd);
	}
  else if(editCommands -> currentText() == "Unload")
	{
    listWidget -> insertItem(current_pos, QString::number(current_pos + 1)
                             + ". Unload: " + editPosition -> displayText());
    editCommandList("unload", editPosition -> displayText(), current_cmd);
	}
  else if(editCommands -> currentText() == "Pause")
	{
    listWidget -> insertItem(current_pos, QString::number(current_pos + 1)
                             + ". Pause: "
                             + editPosition -> displayText() + "s");
    editCommandList("pause", editPosition -> displayText(), current_cmd);
	}
  else if(editCommands -> currentText() == "Loop")
	{
    listWidget -> insertItem(current_pos, QString::number(current_pos + 1)
                             + ". Loops: " + editPosition -> displayText());
    editCommandList("loop", editPosition -> displayText(), current_cmd);
	}

	editWindow -> close();
}

//------------------------------------------------------------------------------
void SequenceTab::addOkClicked()
{
	listWidget -> setEnabled(1);

	int current_cmd = listWidget -> currentRow();
	int current_pos = listWidget -> currentRow();

  current_cmd *= 2;

  if(editCommands -> currentText() == "Move")
	{
    listWidget -> insertItem(current_pos, QString::number(current_pos + 1)
                             + ". Move to: " + editPosition -> displayText());
    addToCommandList("drive", editPosition -> displayText(), current_cmd);
	}
  else if(editCommands -> currentText() == "Load")
	{
    listWidget -> insertItem(current_pos, QString::number(current_pos + 1)
                             + ". Load: " + editPosition -> displayText());
    addToCommandList("load", editPosition -> displayText(), current_cmd);
	}
  else if(editCommands -> currentText() == "Unload")
	{
    listWidget -> insertItem(current_pos, QString::number(current_pos + 1)
                             + ". Unload: " + editPosition -> displayText());
    addToCommandList("unload", editPosition -> displayText(), current_cmd);

	}
  else if(editCommands -> currentText() == "Pause")
	{
    listWidget -> insertItem(current_pos, QString::number(current_pos + 1)
                             + ". Pause: "
                             + editPosition -> displayText() + "s");
    addToCommandList("pause", editPosition -> displayText(), current_cmd);

	}
  else if(editCommands -> currentText() == "Loop")
	{
    listWidget -> insertItem(current_pos, QString::number(current_pos + 1)
                             + ". Loops: " + editPosition -> displayText());
    addToCommandList("loop", editPosition -> displayText(), current_cmd);

	}
  else{
    cout << "No item from combobox" << endl;
  }

  row_count++;
  increaseListNumbers(listWidget->currentRow());

	editWindow -> close();
}

//------------------------------------------------------------------------------
void SequenceTab::contextEraseItem()
{
  //remove from list widget
  if(listWidget -> currentRow() != -1){
    listWidget -> takeItem(listWidget -> currentRow());
    row_count--;
  }
  else{
    return;
  }

  //remove from command list
  if (listWidget -> currentRow() == 0) {
    original_command_list.removeAt(0);
    original_command_list.removeAt(0);
  }
  else
  {
    original_command_list.removeAt(listWidget -> currentRow() * 2);
    original_command_list.removeAt(listWidget -> currentRow() * 2);
  }

  decreaseListNumbers(listWidget -> currentRow());
}

//------------------------------------------------------------------------------
void SequenceTab::decreaseListNumbers(int current_position)
{
  int temp_position;
  QStringList row_text;
  QString final_text;

  if(current_position == -1){
    temp_position = 0;
  }
  else if(current_position == 0){
    temp_position = current_position;
  }
  else{
    temp_position = current_position - 1;
  }

  for( ; temp_position < row_count; temp_position++){
    row_text = listWidget->item(temp_position)->text().split(" ");
    row_text[0] = QString::number(temp_position + 1) + ".";
    final_text = row_text.join(" ");
    listWidget -> takeItem(temp_position);
    listWidget->insertItem(temp_position, final_text);
  }

  listWidget->setCurrentRow(current_position);
}

//------------------------------------------------------------------------------
void SequenceTab::increaseListNumbers(int current_position)
{
  int temp_position;
  QString final_text;
  QStringList row_text;

  if(current_position == -1){
    temp_position = 0;
  }
  else if(current_position == 0){
    temp_position = current_position;
  }
  else{
    temp_position = current_position;
  }


  for( ; temp_position < row_count; temp_position++){
    row_text = listWidget->item(temp_position)->text().split(" ");
    row_text[0] = QString::number(temp_position + 1) + ".";
    final_text = row_text.join(" ");
    listWidget -> takeItem(temp_position);
    listWidget->insertItem(temp_position, final_text);
  }

  listWidget->setCurrentRow(current_position);
}

//------------------------------------------------------------------------------
void SequenceTab::editCommandList(QString command, QString position,
                                  int command_index)
{
  original_command_list[command_index] = command;
  original_command_list[command_index + 1] = position;
}

//------------------------------------------------------------------------------
void SequenceTab::addToCommandList(QString command, QString position,
                                   int command_index)
{
  original_command_list.insert(command_index, position);
  original_command_list.insert(command_index, command);
}

//------------------------------------------------------------------------------
void SequenceTab::keyPressEvent(QKeyEvent *event)
{
  if(listWidget -> isEnabled() == 1)
  {
    if(event->key() == Qt::Key_Delete)
    {
      deleteCommandButtonSlot();
    }

    if(event->key() == Qt::Key_S && event->modifiers()==Qt::ControlModifier)
    {
      saveSeqButtonSlot();
    }

    if(event->key() == Qt::Key_L && event->modifiers()==Qt::ControlModifier)
    {
      loadSeqButtonSlot();
    }

    if(event->key() == Qt::Key_A && event->modifiers()==Qt::ControlModifier)
    {
      startSeqButtonSlot();
    }

    if(event->key() == Qt::Key_E && event->modifiers()==Qt::ControlModifier)
    {
      endSeqButtonSlot();
    }
  }
}

//------------------------------------------------------------------------------
/*bool SequenceTab::eventFilter(QObject *obj, QEvent *event)
{
  if (obj == scomboMoveTo -> lineEdit() || obj == scomboLoad -> lineEdit()
      || obj == scomboUnload -> lineEdit())
  {
      if (event->type() == QEvent::MouseButtonRelease)
      {
          if(((QMouseEvent*)event) -> button() == Qt::LeftButton)
          {
            if(obj == scomboMoveTo -> lineEdit())
            {
              emit(updateComboBox(scomboMoveTo));
            }
            else if(obj == scomboLoad -> lineEdit())
            {
              emit(updateComboBox(scomboLoad));
            }
            else if(obj == scomboUnload -> lineEdit())
            {
              emit(updateComboBox(scomboUnload));
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
      return SequenceTab::eventFilter(obj, event);
  }
}*/

//------------------------------------------------------------------------------
void SequenceTab::startSeqButtonSlot()
{
  endseq = 0;
  errors = 0;
  row_counter = 0;
  unloads = 0;
  loads = 0;
  moves = 0;
  errtime = 0;
  loop_counter = 0;
  loop_check = 0;
  loop_index = 0;

  if (shuttle_connection_status == 1)
  {
    disable();

    if(main_list_index > 0)
    {
      restartSeqButton -> setEnabled(0);
      statisticButton -> setEnabled(0);
    }

    mainTimer -> start(ONE_SEC);

    listWidget -> setCurrentRow(row_counter);

    for(main_list_index = 0; main_list_index < original_command_list.size();
        main_list_index++)
    {
      if (original_command_list[main_list_index] == "loop")
      {
        loop_index = 0;
        loop_save.append(original_command_list[main_list_index + 1]);
        loop_check++;
      }

    }
    main_list_index = 0;

    if (filename.size() > 0)
    {
      emit(buildStatusMessage(tr("Sequence %1 started!").arg(filename)));
    }
    else
    {
      emit(buildStatusMessage("New Sequence started!"));
      filename = "";
    }

    if(listWidget -> count() > 0)
    {
      sequence();
    }
    else
    {
      emit(buildStatusMessage("Sequence list is empty!"));
      emit(sequenceStarted(0));
      enable();
      mainTimer -> stop();
    }
  }
  else
  {
   emit(buildStatusMessage("No Shuttle connected!"));
  }
}

//------------------------------------------------------------------------------
void SequenceTab::restartSeqButtonSlot()
{
  if (shuttle_connection_status == 1)
  {
    int current_cmd = listWidget -> currentRow();

    restartSeqButton -> setEnabled(0);
    statisticButton -> setEnabled(0);

    if(errTimer -> isActive())
    {
      errTimer -> stop();
    }

    for(int index = 0; index < original_command_list.size(); index++)
    {
      if (original_command_list[index] == "loop")
      {
        loop_index = 0;
        loop_save.append(original_command_list[index+ 1]);
      }

    }

    if(listWidget -> count() > 0)
    {
      if(current_cmd >= 0)
      {
        row_counter = current_cmd;
        main_list_index = current_cmd * 2;
      }
      else
      {
        row_counter = 0;
        listWidget -> setCurrentRow(0);
      }

      endseq = 0;
      emit(buildStatusMessage("Sequence restarted!"));
      mainTimer -> start(ONE_SEC);
      disable();
      sequence();
    }
    else
    {
      restartSeqButton -> setEnabled(1);
      emit(buildStatusMessage("Sequence list is empty! Create a sequence "
                              "first!"));
    }
  }
  else
  {
    emit(buildStatusMessage("No Shuttle connected!"));
  }
}

//------------------------------------------------------------------------------
void SequenceTab::sequence()
{
  QStringList driveparam;
  QString param;
  resend_counter = 0;

  if(main_list_index == 0)
  {
    first_loop_flag = 1;
  }

  if (main_list_index < original_command_list.size() && endseq != 1)
  {
    signalTimerSequence -> stop();

    cout << "Sequence started" << endl;

    if (original_command_list[main_list_index] == "drive")
    {
      cout << "drive eaten" << endl;
      driveparam += original_command_list[++main_list_index];
      emit(moveTo(driveparam));
      signalTimerSequence -> start(TEN_SEC);
      moves++;
    }
    else if (original_command_list[main_list_index] == "load")
    {
      cout << "load eaten" << endl;
      emit(loadAt(original_command_list[++main_list_index]));
      signalTimerSequence -> start(THIRTY_SEC);
      loads++;
    }
    else if (original_command_list[main_list_index] == "unload")
    {
      cout << "unload eaten" << endl;
      emit(unloadAt(original_command_list[++main_list_index]));
      signalTimerSequence -> start(THIRTY_SEC);
      unloads++;
    }
    else if (original_command_list[main_list_index] == "pause")
    {
      //Cancel pause command if it is the last one
      if(main_list_index + 2 != original_command_list.size() || loop > 0)
      {
        cout << "pause eaten" << endl;
        listWidget -> setCurrentRow(row_counter);
        emit(buildStatusMessage(tr("Pause sequence for %1 seconds")
          .arg(original_command_list[main_list_index+1])));
        param = original_command_list[++main_list_index];
        pause_in_seconds = param.toInt();
        cout << "pause: " << pause_in_seconds << endl;
        pause_restore = param.toInt();
        pauseTimer -> start(ONE_SEC);
      }
      else
      {
          listWidget -> setCurrentRow(row_counter++);
          emit(buildStatusMessage("Skipped Pause as last command in list"));
          endseq = 1;
          sequence();
      }
    }
    else if (original_command_list[main_list_index] == "loop")
    {
      cout << "loop eaten" << endl;
      QString loop_in_command_list = original_command_list
          .at(main_list_index + 1);
      loop = loop_in_command_list.toInt();

      int current_cmd = listWidget -> currentRow();
      QString list_number = listWidget->item(current_cmd)->text().split(" ")[0];
      listWidget -> takeItem(current_cmd);

      if(first_loop_flag == 1)
      {
        loop_index = 0;
        first_loop_flag = 0;
      }

      if(errTimer -> isActive())
      {
        errTimer -> stop();
      }

      if (loop > 0)
      {
        loop--;
        original_command_list[main_list_index + 1] = QString::number(loop);

        /*if(errTimer -> isActive())
        {
          errTimer -> stop();
        }*/

        listWidget -> insertItem (current_cmd, list_number
            + " Loops: " + QString::number(loop));
        emit(buildStatusMessage(tr("Loops: %1").arg(QString::number(loop))));
        row_counter = 0;
        listWidget -> setCurrentRow(row_counter);
        main_list_index = 0;
        row_counter = 0;
        sequence();
      }
      else if (loop == -1)
      {
        main_list_index = 0;
        row_counter = 0;
        loop_counter ++;
        emit(buildStatusMessage(tr("Loops: %1")
                                .arg(QString::number(loop_counter))));
        listWidget -> insertItem (current_cmd, list_number
            + " Loops: " + QString::number(loop_counter));
        listWidget -> setCurrentRow(row_counter);
        sequence();
      }
      else if (loop < 1)
      {
        if(loop_check > 1)loop--;
        original_command_list[main_list_index + 1] = QString::number(loop);
        {;
          listWidget -> insertItem (current_cmd, list_number
              + " Loops: " + QString::number(loop_index));
          original_command_list[main_list_index + 1] = loop_save.at(loop_index);
          loop_index++;
        }
        main_list_index += 2;
        listWidget -> setCurrentRow(++row_counter);
        sequence();
      }
    }

    emit(sequenceStarted(1));
  }
  else
  {
    emit(buildStatusMessage("Sequence stopped"));
    enable();
    mainTimer -> stop();
    statisticButton -> setEnabled(1);
    emit(sequenceStarted(0));
  }
}

//------------------------------------------------------------------------------
void SequenceTab::endSeqButtonSlot()
{
  endseq = 1;

  if(listWidget -> count() > 0)
  {
    if(mainTimer -> isActive())
    {
      mainTimer -> stop();
    }

    if (signalTimerSequence -> isActive())
    {
      signalTimerSequence ->  stop();
    }

    if (pauseTimer -> isActive())
    {
      pauseTimer ->  stop();
    }
    statisticButton -> setEnabled(1);

    restartSeqButton -> setEnabled(1);
  }
  else
  {
    emit(buildStatusMessage("Start a sequence first"));
  }

  if(errTimer -> isActive())
  {
    errTimer -> stop();
  }

  emit(sequenceStarted(0));
  enable();
}

//------------------------------------------------------------------------------
void SequenceTab::pauseEnded()
{
  pause_in_seconds--;
  QString list_number =
      listWidget->item(listWidget -> currentRow())->text().split(" ")[0];

  if(pause_in_seconds < 0){
    endSeqButtonSlot();
  }

  if(pause_in_seconds > 0)
  {
    listWidget -> takeItem(row_counter);
    cout << "Pause is bigger 0" << endl;
    listWidget -> insertItem(row_counter, list_number
                             + " Pause: "
                             + QString::number(pause_in_seconds) + "s");

    listWidget -> setCurrentRow(row_counter);
  }

  if(pause_in_seconds == 0)
  {
    listWidget -> takeItem(row_counter);
    cout << "pause is zero" << endl;
    listWidget -> insertItem(row_counter, list_number
                             + " Pause: " + QString::number(pause_restore) + "s");
    pauseTimer -> stop();
    emit(buildStatusMessage("Pause ended"));

    listWidget -> setCurrentRow(++row_counter);
    main_list_index++;
    sequence();
  }
}

//------------------------------------------------------------------------------
void SequenceTab::sequenceSignalTimeOut()
{
  QStringList driveparam;
  QString param;
  resend_counter++;
  main_list_index--;

  if (endseq != 1  && resend_counter < 3)
  {
    if (original_command_list[main_list_index] == "drive")
    {
      emit(buildStatusMessage("Resending Move-Command"));

      driveparam += original_command_list[++main_list_index];
      emit(moveTo(driveparam));
      signalTimerSequence -> start(TEN_SEC);
    }
    else if (original_command_list[main_list_index] == "load")
    {
      //TASI interference - shuttle doesn't send a status message
      //The command was already successful so skip it
      emit(buildStatusMessage("Skipped load command. Could be TASI "
                              "interference!"));
      listWidget -> setCurrentRow(++row_counter);
      loads++;
      main_list_index = main_list_index + 2;
      sequence();
    }
    else if (original_command_list[main_list_index] == "unload") {
      emit(buildStatusMessage("Skipped unload command. TASI interference!"));
      listWidget -> setCurrentRow(++row_counter);
      unloads++;
      main_list_index = main_list_index + 2;
      sequence();
    }
  }
  else
  {
    signalTimerSequence -> stop();
    mainTimer -> stop();
    enable();
    endseq = 1;
    endSeqButtonSlot();
  }
}

//------------------------------------------------------------------------------
void SequenceTab::time()
{
  if(seconds == 59)
  {
    seconds = 0;
    ++minutes;
  }
  else if(minutes == 59)
  {
    minutes = 0;
    ++hours;
  }
  else
  {
    seconds++;
  }

  time_format = QString::number(hours) + "h : " + QString::number(minutes)
      + "m : "+ QString::number(seconds) + "s";
  timeLine -> setText(time_format);
}

//------------------------------------------------------------------------------
void SequenceTab::lastCommandSuccessful(bool successful, QString status_text)
{
  if(successful == 1)
  {
    if(mainTimer -> isActive() == 1)
    {
      main_list_index++;
      signalTimerSequence -> stop();
      listWidget -> setCurrentRow(++row_counter);
      sequence();
    }
  }
  else
  {
    endSeqButtonSlot();

    if(signalTimerSequence -> isActive())
    {
      signalTimerSequence -> stop();
    }

    errors++;
    connect(errTimer, SIGNAL(timeout()), this, SLOT(errTime()));
    errTimer -> start(1000);
    error_list += "\n\n" + status_text;
    emit(blinkingErrorFrame());
  }
}

//------------------------------------------------------------------------------
void SequenceTab::errTime()
{
  errtime ++;
}

//------------------------------------------------------------------------------
void SequenceTab::statisticButtonSlot()
{
  QString error, time, errortime, move, unload, load, first, filename;

  first = "Time;Errors;Error time;Moves;Unloads;Loads";
  time = QString::number(hours) + ":" + QString::number(minutes) + ":"
    + QString::number(seconds);
  error = QString::number(errors);
  errortime = QString::number(errtime) + "s";
  unload = QString::number(unloads);
  load = QString::number(loads);
  move = QString::number(moves);

  filename = QFileDialog::getSaveFileName(this, "Create Statistic?", ".csv",
                                          "CSV-Datei (*.csv)");

  QFile file(filename);

  if (file.open(QFile::WriteOnly|QFile::Truncate))
  {
    QTextStream stream(&file);
    stream << first << "\n" << time << ";" << error << ";" << errortime
           << ";" << move << ";"
      << unload << ";" << load << "\n" << error_list;
    file.close();
  }
}
