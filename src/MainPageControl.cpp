//------------------------------------------------------------------------------
// Filename: MainPageControl.cpp
// Author: ystc
// Created: 03.03.2017
//------------------------------------------------------------------------------

#include "inc/MainPageControl.h"
#include "inc/MainPage.h"
#include "inc/ShuttlePageControl.h"
#include "inc/UserInterface.h"
#include "inc/Shuttle.h"

//------------------------------------------------------------------------------
MainPageControl::MainPageControl(QObject *parent) : QObject(parent)
{
    cout << "--------------------STE Version 3.0--------------------" << endl;
    shuttle_port = 1350;
    shuttle_counter = 0;
    currentShuttle = NULL;
    window_is_open_flag = false;
    shuttle_id_count = 0;
}

//------------------------------------------------------------------------------
MainPageControl::~MainPageControl()
{
  saveShuttleList();

  for(int delete_count = 0; delete_count < shuttle_counter; delete_count++){
    delete shuttleList.at(delete_count)->getController();
  }
}

//------------------------------------------------------------------------------
void MainPageControl::show(int argc, char* argv[])
{
  QApplication main_application(argc, argv);
  application_directory  = main_application.applicationDirPath();
  main_window = new MainPage(argc, argv, this,  application_directory);
  QRect display_size = QApplication::desktop() -> screenGeometry();

  display_width = display_size.width();
  display_height = display_size.height();

  main_window->setObjectName("MainPage");
  main_window->move(0, 0);
  main_window->resize(500, display_height);
  main_window->show();

  connect(main_window, SIGNAL(closeShuttlePageSignal(Shuttle*, int)),
          this, SLOT(removeShuttleFromList(Shuttle*, int)));
  connect(main_window, SIGNAL(disconnectShuttlePageSignal(Shuttle*, int)),
          this, SLOT(disconnectShuttlePage(Shuttle*, int)));
  QString file_name = "data/shuttles.dat";
  QFile file(file_name);

  if(file.open(QFile::ReadOnly)) {
    QString temp_shuttle_list = QLatin1String(file.readAll());
    file.close();
    temp_shuttle_list = temp_shuttle_list.simplified();
    QStringList shuttle_list = temp_shuttle_list.split(" ");

    for(int count = 1; count < shuttle_list.count(); count += 2){
      main_window->addShuttleLine->setText(shuttle_list.at(count));
      main_window->addShuttlePageSlot();
    }
  }

  if(argc > 1 ){
    if(strcmp(argv[1], "-i") == 0){
      main_window->addShuttleLine->setText(argv[2]);
      main_window->addShuttlePageSlot();
    }
    else if(strcmp(argv[1], "-f") == 0){
      QString file_name = "";
      file_name.append(argv[2]);
      QFile file(file_name);

      if(file.open(QFile::ReadOnly)) {
        QString temp_shuttle_list = QLatin1String(file.readAll());
        file.close();
        temp_shuttle_list = temp_shuttle_list.simplified();

        QRegExp rx("shuttles=\" "
                   "(SHUT\\d\\d\\d\\d \\d+\\.\\d+\\.\\d+\\.\\d+ )+");
        rx.indexIn(temp_shuttle_list);

        QStringList list = rx.capturedTexts();
        cout << list[0].toStdString() << endl;
        QStringList shuttle_list = list[0].split(" ");

        for(int count = 2 ; count < shuttle_list.count(); count += 2){
          cout << shuttle_list.at(count).toStdString() << endl;
          main_window->addShuttleLine->setText(shuttle_list.at(count));
          main_window->addShuttlePageSlot();
        }
      }
      else{
        cout << "Could not open file!" << endl;

        QMessageBox msgBox;
        msgBox.setText("<font size = 5>Could not open file!.</font>");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();

      }
    }
  }

  main_application.exec();
}

//------------------------------------------------------------------------------
void MainPageControl::saveShuttleList()
{
  QFile file("data/shuttles.dat");

  if(file.open(QFile::WriteOnly)) {
    for(int count = 0; count < shuttle_counter; count++){
      QString tem_data = shuttleList.at(count)->getShuttleName()
          + " " + shuttleList.at(count)->getShuttleIp() + "\n";
      file.write(tem_data.toLocal8Bit());
    }
    file.close();
  }
}

//------------------------------------------------------------------------------
void MainPageControl::addShuttleToList(Shuttle *newShuttle)
{
  cout << "adding shuttle to list" << endl;
  newShuttle->setPort(shuttle_port++);
  pageController = new ShuttlePageControl(application_directory, newShuttle);
  newShuttle->setController(pageController);
  newShuttle->setShuttleWindow(NULL);

  cout << "shuttle id: " << newShuttle->getShuttleId() << endl;
  cout << "shuttle name: "
       << newShuttle->getShuttleName().toStdString() << endl;
  cout << "shuttle ip: " << newShuttle->getShuttleIp().toStdString() << endl;
  cout << "shuttle port: " << newShuttle->getPort() << endl;

  shuttle_counter++;
  shuttleList.append(newShuttle);
  connect(pageController, SIGNAL(forwardConnectionSignal(int, QString, bool)),
          this, SLOT(setConnection(int, QString, bool)));
  connect(pageController, SIGNAL(shuttleWindowClosed()),
          this, SLOT(setWindowFlag()));

  if(window_is_open_flag == 0){
    window_is_open_flag = 1;
    pageController->create(display_width, display_height);
    newShuttle->setWindowOpen(1);
    currentShuttle = newShuttle;
  }
  else{
    newShuttle->setWindowOpen(0);
  }
}

//------------------------------------------------------------------------------
void MainPageControl::removeShuttleFromList(Shuttle *shuttle, int list_position)
{
  if(!shuttle){
    return;
  }

  if(shuttle->getShuttleWindow() != NULL){
    cout << "Delete Shuttle with name: "
         << shuttle->getShuttleName().toStdString() << endl;
    cout << "Delete at: "  << list_position << endl;

    if(shuttle->getWindowOpen() == 1){
      shuttle->getShuttleWindow()->close();
    }

    delete(shuttleList.at(list_position)->getShuttleWindow());
    shuttleList.at(list_position)->setShuttleWindow(NULL);
    shuttleList.takeAt(list_position);
    shuttle_counter--;
    currentShuttle = NULL;
  }
  else{
    shuttleList.takeAt(list_position);
    shuttle_counter--;
  }

}

//------------------------------------------------------------------------------
void MainPageControl::disconnectShuttlePage(Shuttle *shuttle, int list_position)
{
  if(shuttle->getShuttleWindow() != NULL){
    shuttle->getShuttleWindow()->close();
    delete(shuttleList.at(list_position)->getShuttleWindow());
    shuttleList.at(list_position)->setShuttleWindow(NULL);
    currentShuttle = NULL;
  }
}

//------------------------------------------------------------------------------
void MainPageControl::showShuttlePage(QListWidgetItem *current_item)
{
  tempShuttle = searchForIP(current_item->text().split("\t")[1]);

  if(tempShuttle->getWindowOpen() == 0){
    if(currentShuttle){
      currentShuttle->getShuttleWindow()->setVisible(0);
      currentShuttle->setWindowOpen(0);
    }

    if(!tempShuttle->getShuttleWindow()){
      tempShuttle->getController()->create(display_width, display_height);
      tempShuttle->setWindowOpen(1);
      currentShuttle = tempShuttle;
    }
    else{
      tempShuttle->getShuttleWindow()->setVisible(1);
      tempShuttle->setWindowOpen(1);
      currentShuttle = tempShuttle;
    }
  }
}

//------------------------------------------------------------------------------
Shuttle* MainPageControl::searchForIP(QString ip)
{
  int counter = 0;

  while(counter < shuttle_counter){
    tempShuttle = shuttleList.at(counter);
    if(tempShuttle->getShuttleIp() == ip){
      return tempShuttle;
    }
    counter++;
  }
  return NULL;
}

//------------------------------------------------------------------------------
void MainPageControl::setConnection(int connected, QString name,
                                    bool sequence_status)
{
  main_window->setConnection(connected, name, sequence_status);
}

//------------------------------------------------------------------------------
void MainPageControl::deleteAll()
{
  for(int count = 0; count < shuttle_counter; count++){
    tempShuttle = shuttleList.at(count);
    tempShuttle->getController()->shuttle_window->close();
  }
}

//------------------------------------------------------------------------------
void MainPageControl::setWindowFlag()
{
  window_is_open_flag = !window_is_open_flag;
}
