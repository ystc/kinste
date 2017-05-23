//------------------------------------------------------------------------------
// Filename: MainPage.cpp
// Author: ystc
// Created: 03.03.2017
// Last change: 18.04.2017
//------------------------------------------------------------------------------

#include "inc/MainPage.h"
#include "inc/TASIHandler.h"
#include "inc/MainPageControl.h"
#include "inc/ShuttlePageControl.h"
#include "inc/UserInterface.h"
#include "inc/Shuttle.h"

//-----------------------------------------------------------------------------
MainPage::MainPage(int argc, char* argv[], MainPageControl* controller,
                   QString directory_path, QWidget *parent) : QWidget(parent)
{
  arg_count = argc;
  arguments = argv;
  row_counter = 0;
  shuttle_counter = 0;
  mainControl = controller;

  mainGrid = new QGridLayout(this);
  bodyGrid = new QGridLayout;
  body = new QGroupBox;
  foot = new QGroupBox;
  logo = new QLabel("Shuttle Tester");
  list = new QListWidget;
  addShuttle = new QPushButton("Add");
  deleteShuttle = new QPushButton("Delete");
  disconnectShuttle = new QPushButton("Disconnect");
  save = new QPushButton("Save");
  addShuttleLabel = new QLabel("Enter Shuttle-IP");
  addShuttleLine = new QLineEdit;

  list->setIconSize(QSize(60, 40));
  list->setFocus();
  list -> setContextMenuPolicy(Qt::CustomContextMenu);

  connect(list, SIGNAL(customContextMenuRequested(QPoint)), this,
    SLOT(showContextMenu(QPoint)));
  connect(addShuttle, SIGNAL(pressed()), this, SLOT(addShuttlePageSlot()));
  connect(addShuttleLine, SIGNAL(returnPressed()),
          this, SLOT(addShuttlePageSlot()));
  connect(list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          mainControl, SLOT(showShuttlePage(QListWidgetItem*)));
  connect(deleteShuttle, SIGNAL(pressed()), this, SLOT(closeShuttlePageSlot()));
  connect(disconnectShuttle, SIGNAL(pressed()),
          this, SLOT(disconnectShuttleSlot()));

  logo->setObjectName("logo");
  body->setObjectName("body");
  //foot->setObjectName("footer");
  list->setObjectName("listWidget");
  addShuttle->setProperty("button", true);
  deleteShuttle->setProperty("button", true);
  disconnectShuttle->setProperty("button", true);
  save->setProperty("button", true);
  addShuttleLabel->setProperty("executeLabel", true);
  addShuttleLine->setProperty("executeLine", true);

  bodyGrid->setAlignment(Qt::AlignHCenter);
  bodyGrid->addWidget(addShuttleLabel, 0, 0);
  bodyGrid->addWidget(addShuttleLine, 1, 0, 1, 2);
  bodyGrid->addWidget(addShuttle, 1, 2);
  bodyGrid->addWidget(list, 2, 0, 1, 3);
  bodyGrid->addWidget(deleteShuttle, 3, 0);
  bodyGrid->addWidget(disconnectShuttle, 3, 1);
  bodyGrid->addWidget(save, 3, 2);
  //bodyGrid->setColumnStretch(0, 5);
  //bodyGrid->setColumnStretch(1, 5);
  //bodyGrid->setColumnStretch(2, 5);
  body->setLayout(bodyGrid);

  mainGrid->addWidget(logo, 0, 0);
  mainGrid->addWidget(body, 1, 0);
  mainGrid->addWidget(foot, 2, 0);

  QString final_path;
  final_path = directory_path + "/qss/mainpage_stylesheet.qss";
  QFile file(final_path);

  if(file.open(QFile::ReadOnly)) {
    QString style_sheet = QLatin1String(file.readAll());
    file.close();
    style_sheet.replace("%PATH%",directory_path);
    setStyleSheet(style_sheet);
  }
}

//------------------------------------------------------------------------------
void MainPage::closeEvent(QCloseEvent *)
{
  if(mainControl->currentShuttle){
    mainControl->currentShuttle->close();
  }
}

//------------------------------------------------------------------------------
void MainPage::addShuttlePageSlot()
{
  QString shuttle_data;

  if(!mainControl->searchForIP(addShuttleLine->text())){
    newShuttle = new Shuttle(shuttle_counter++);

    if(shuttle_counter < 10){
      shuttle_data = tr("SHUT000") + QString::number(shuttle_counter);
    }
    else if(shuttle_counter < 100){
      shuttle_data = tr("SHUT00") + QString::number(shuttle_counter);
    }
    else if(shuttle_counter < 1000){
      shuttle_data = tr("SHUT0") + QString::number(shuttle_counter);
    }
    else if(shuttle_counter < 10000){
      shuttle_data = tr("SHUT") + QString::number(shuttle_counter);
    }
    else{
      QMessageBox::information(this, "Warning",
                               "<font size = 5>"
                               "Too many shuttles for me!</font>");
    }
    newShuttle->setShuttleName(shuttle_data);
    shuttle_data.append("\t");
    newShuttle->setShuttleIp(addShuttleLine->text());
    shuttle_data.append(addShuttleLine->text());
    item = new QListWidgetItem(QIcon("icons/wifi_nc.png"), shuttle_data);
    list->insertItem(row_counter, item);
    list->setCurrentRow(row_counter);
    mainControl->addShuttleToList(newShuttle);
    row_counter++;
  }
  else{
    QMessageBox::information(this, "Information",
                             "<font size = 5>Shuttle already in list!</font>");
  }
}

//------------------------------------------------------------------------------
void MainPage::closeShuttlePageSlot()
{
  if(row_counter == 0){
    return;
  }

  QString ip = list->item(list->currentRow())->text().split("\t")[1];
  tempShuttle = mainControl->searchForIP(ip);
  emit(closeShuttlePageSignal(tempShuttle, list->currentRow()));
  delete(list->takeItem(list->currentRow()));
  row_counter--;
}

//------------------------------------------------------------------------------
void MainPage::disconnectShuttleSlot()
{
  if(row_counter == 0){
    return;
  }

  QString ip = list->item(list->currentRow())->text().split("\t")[1];
  tempShuttle = mainControl->searchForIP(ip);
  emit(disconnectShuttlePageSignal(tempShuttle, list->currentRow()));
  list->item(list->currentRow())->setIcon(QIcon("icons/wifi_nc.png"));
}

//------------------------------------------------------------------------------
void MainPage::setConnection(int connected, QString name)
{
  QListWidgetItem *itemTemp;
  for(int count = 0; count < row_counter; count++){
    if(list->item(count)->text().split("\t")[0] == name){
      itemTemp = list->item(count);

      if(connected)
      {
        itemTemp->setIcon(QIcon("icons/wifi_c.png"));
      }
      else
      {
        itemTemp->setIcon(QIcon("icons/wifi_nc.png"));
      }
    }
  }
}

//------------------------------------------------------------------------------
void MainPage::showContextMenu(QPoint pos)
{
  QPoint globalPos = list -> mapToGlobal(pos);

  QMenu myMenu;
  myMenu.addAction("Delete", this, SLOT(closeShuttlePageSlot()));
  myMenu.addAction("Disconnect", this, SLOT(disconnectShuttleSlot()));
  myMenu.exec(globalPos);
}
