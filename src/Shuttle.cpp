//------------------------------------------------------------------------------
// Filename: Shuttle.cpp
// Author: ystc
// Created: 06.03.2017
// Last change: 18.04.2017
//------------------------------------------------------------------------------

#include "inc/Shuttle.h"
#include "inc/ShuttlePageControl.h"
#include "inc/UserInterface.h"

//------------------------------------------------------------------------------
Shuttle::Shuttle(int id, QObject *parent) : QObject(parent)
{
  shuttle_id = id;
}

//------------------------------------------------------------------------------
ShuttlePageControl *Shuttle::getController() const
{
  return controller;
}

//------------------------------------------------------------------------------
void Shuttle::setController(ShuttlePageControl *new_controller)
{
  controller = new_controller;
}

//------------------------------------------------------------------------------
void Shuttle::setWindowOpen(bool value)
{
  window_open = value;
}

//------------------------------------------------------------------------------
bool Shuttle::getWindowOpen() const
{
  return window_open;
}

//------------------------------------------------------------------------------
int Shuttle::getShuttleId() const
{
  return shuttle_id;
}

//------------------------------------------------------------------------------
void Shuttle::setShuttleId(int value)
{
  shuttle_id = value;
}

//------------------------------------------------------------------------------
int Shuttle::getPort() const
{
  return port;
}

//------------------------------------------------------------------------------
void Shuttle::setPort(int value)
{
  port = value;
}

//------------------------------------------------------------------------------
QString Shuttle::getShuttleName() const
{
  return shuttle_name;
}

//------------------------------------------------------------------------------
void Shuttle::setShuttleName(const QString &value)
{
  shuttle_name = value;
}

//------------------------------------------------------------------------------
QString Shuttle::getShuttleIp() const
{
  return shuttle_ip;
}

//------------------------------------------------------------------------------
void Shuttle::setShuttleIp(const QString &value)
{
  shuttle_ip = value;
}

//------------------------------------------------------------------------------
void Shuttle::close()
{
  shuttleWindow->close();
}

//------------------------------------------------------------------------------
UserInterface *Shuttle::getShuttleWindow() const
{
  return shuttleWindow;
}

//------------------------------------------------------------------------------
void Shuttle::setShuttleWindow(UserInterface *ui)
{
  shuttleWindow = ui;
}
