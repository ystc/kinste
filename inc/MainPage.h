#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QIcon>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QByteArray>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QSize>
#include <iostream>
#include <QPoint>
#include <QMenu>

using namespace std;

class MainPageControl;
class Shuttle;

class MainPage : public QWidget
{
    Q_OBJECT

  public:
    MainPage(int argc, char* argv[], MainPageControl* controller,
             QString directory_path, QWidget* parent = 0);

     void setConnection(int connected, QString name);
     QListWidget *list;
     QPushButton *addShuttle, *deleteShuttle, *disconnectShuttle, *save;
     QLineEdit *addShuttleLine;

  private:
    int row_counter, shuttle_counter, arg_count;
    char** arguments;
    QGridLayout *mainGrid, *bodyGrid;
    QGroupBox *head, *body, *foot;
    QLabel *logo, *addShuttleLabel;
    MainPageControl *mainControl;
    QListWidgetItem *item;
    Shuttle *newShuttle, *tempShuttle;


  protected:
    void closeEvent(QCloseEvent *);

  public slots:
    void closeShuttlePageSlot();
    void addShuttlePageSlot();
    void disconnectShuttleSlot();
    void showContextMenu(QPoint pos);

  signals:
    void closeShuttlePageSignal(Shuttle *shuttle, int list_position);
    void disconnectShuttlePageSignal(Shuttle *shuttle, int list_position);
};

#endif // MAINPAGE_H
