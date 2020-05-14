//
// Created by Прогрессивный Долгострой on 14.05.20.
//

#ifndef ANSIRC_INCLUDE_UI_MAINWINDOW_H_
#define ANSIRC_INCLUDE_UI_MAINWINDOW_H_

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
};

#endif //ANSIRC_INCLUDE_UI_MAINWINDOW_H_
