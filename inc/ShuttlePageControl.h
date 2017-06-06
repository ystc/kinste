#ifndef SHUTTLEPAGECONTROL_H
#define SHUTTLEPAGECONTROL_H

#include <QObject>
#include <iostream>
#include <QString>

using namespace std;

class UserInterface;
class MainPage;
class Shuttle;

class ShuttlePageControl : public QObject
{
    Q_OBJECT

  public:
    ShuttlePageControl(QString directory, Shuttle *newShuttle,
                       QObject* parent = 0);
    ~ShuttlePageControl();
    void create(int width, int height);
    UserInterface *shuttle_window;

  private:
    QString current_shuttle_ip, app_dir, current_shuttle_name;
    QWidget* main_window_temp;
    int current_shuttle_port, controller_id;
    Shuttle *shuttle;

  public slots:
    void setConnection(int connection, bool sequence_status);
    void interfaceClosed();

  signals:
    void forwardConnectionSignal(int connection, QString shuttle_name,
                                 bool sequence_status);
    void shuttleWindowClosed();
};

#endif // SHUTTLEPAGECONTROL_H
