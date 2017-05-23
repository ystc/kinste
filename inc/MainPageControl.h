#ifndef MAINPAGECONTROL_H
#define MAINPAGECONTROL_H

#include <QObject>
#include <QFile>
#include <QMessageBox>
#include <iostream>
#include <QApplication>
#include <QRect>
#include <QDesktopWidget>
#include <QListWidgetItem>
#include <QString>
#include <QList>

using namespace std;

class TASIHandler;
class ShuttlePageControl;
class MainPage;
class Shuttle;

/*struct _Shuttle_
{
  ShuttlePageControl *controller;
  QListWidgetItem *item;
  bool is_open;
  QString shuttle_ip;
} typedef Shuttle;*/

class MainPageControl : public QObject
{
    Q_OBJECT

  public:
    MainPageControl(QObject* parent = 0);
    ~MainPageControl();

    void show(int argc, char *argv[]);
    Shuttle *searchForIP(QString ip);
    void addShuttleToList(Shuttle *newShuttle);
    void deleteAll();

    int shuttle_counter;
    Shuttle  *currentShuttle;
    QList <Shuttle *> shuttleList;

  private:
    ShuttlePageControl *pageController;
    Shuttle *tempShuttle;
    TASIHandler *tasiHandler;
    MainPage *main_window;
    QString application_directory;
    int display_height, display_width, shuttle_port, shuttle_id_count;
    bool window_is_open_flag;

  public slots:
     void showShuttlePage(QListWidgetItem *current_item);
     void setConnection(int connection, QString name);
     void removeShuttleFromList(Shuttle *shuttle, int list_position);
     void disconnectShuttlePage(Shuttle *shuttle, int list_position);
     void setWindowFlag();
};

#endif // MAINPAGECONTROL_H
