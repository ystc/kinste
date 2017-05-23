//------------------------------------------------------------------------------------------------
// Filename: CostumTabWidget.h
// Description: QTabWidget with public QTabBar for editing
// Author: ystc
// Created: 12.08.2016
// Last change: 12.08.2016
//------------------------------------------------------------------------------------------------

#ifndef COSTUMTABWIDGET_H
#define COSTUMTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>

class CostumTabWidget : public QTabWidget
{
    Q_OBJECT

  public:
  CostumTabWidget(QWidget *parent = 0) : QTabWidget(parent){}

  QTabBar *tabBar() const {return QTabWidget::tabBar();}
};


#endif // COSTUMTABWIDGET_H
