#ifndef SHUTTLE_H
#define SHUTTLE_H

#include <QObject>
#include <QListWidgetItem>

class ShuttlePageControl;
class UserInterface;
class MainPage;

class Shuttle : public QObject
{
    Q_OBJECT
  public:
    Shuttle(int id, QObject *parent = 0);

    QString getShuttleIp() const;
    void setShuttleIp(const QString &value);

    ShuttlePageControl *getController() const;
    void setController(ShuttlePageControl *new_controller);

    void setWindowOpen(bool value);
    bool getWindowOpen() const;

    QString getShuttleName() const;
    void setShuttleName(const QString &value);

    int getShuttleId() const;
    void setShuttleId(int value);

    int getPort() const;
    void setPort(int value);

    void close();

    UserInterface *getShuttleWindow() const;
    void setShuttleWindow(UserInterface *ui);

private:
    int shuttle_id;
    int port;
    QString shuttle_ip;
    QString shuttle_name;
    bool window_open;
    ShuttlePageControl *controller;
    UserInterface *shuttleWindow;

  signals:

  public slots:
};

#endif // SHUTTLE_H
