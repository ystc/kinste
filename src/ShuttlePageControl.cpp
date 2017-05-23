//------------------------------------------------------------------------------
// Filename: SingleCommandsTab.cpp
// Author: ystc
// Created: 03.03.2017
// Last change: 18.04.2017
//------------------------------------------------------------------------------

#include "inc/ShuttlePageControl.h"
#include "inc/UserInterface.h"
#include "inc/MainPage.h"
#include "inc/Shuttle.h"

//------------------------------------------------------------------------------
ShuttlePageControl::ShuttlePageControl(QString directory, Shuttle *newShuttle,
                                       QObject *parent) : QObject(parent)
{
  shuttle_window = NULL;
  app_dir = directory;
  shuttle = newShuttle;
}

//------------------------------------------------------------------------------
ShuttlePageControl::~ShuttlePageControl()
{
  //delete shuttle_window;
}

//------------------------------------------------------------------------------
void ShuttlePageControl::create(int width, int height)
{
  shuttle_window = new UserInterface(shuttle);
  shuttle_window->setObjectName("UIMainWindow");
  connect(shuttle_window, SIGNAL(connection(int)),
          this, SLOT(setConnection(int)));
  connect(shuttle_window, SIGNAL(userInterfaceClosed()),
          this, SLOT(interfaceClosed()));

  shuttle_window->move(400, 0);
  shuttle_window->resize(width - 400, height);
  shuttle_window->show();
  shuttle->setShuttleWindow(shuttle_window);

  QString final_path;
  final_path = app_dir + "/qss/userinterface_stylesheet.qss";
  QFile file(final_path);
  if(file.open(QFile::ReadOnly)) {
    QString style_sheet = QLatin1String(file.readAll());
    file.close();
    style_sheet.replace("%PATH%",app_dir);
    shuttle_window->setStyleSheet(style_sheet);
  }
}

//------------------------------------------------------------------------------
void ShuttlePageControl::setConnection(int connection)
{
  emit(forwardConnectionSignal(connection, shuttle->getShuttleName()));
}

//------------------------------------------------------------------------------
void ShuttlePageControl::interfaceClosed()
{
  emit(shuttleWindowClosed());
}


