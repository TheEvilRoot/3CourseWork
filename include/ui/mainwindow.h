#ifndef ANSIRC_INCLUDE_UI_MAINWINDOW_H_
#define ANSIRC_INCLUDE_UI_MAINWINDOW_H_

#include <QMainWindow>
#include <QMetaObject>
#include <QStringListModel>
#include <QString>
#include <QStringList>

#include "ui/clientview.h"

#include "client.h"
#include "pthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public ClientView {

  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override { delete ui; };

  void motdUpdate(std::string motd) override;
  void newMessage(std::string msg) override;

  void updateLogModel();

private:
  Ui::MainWindow *ui;
  std::unique_ptr<QStringListModel> logModel;
  QStringList logList;
  Client *client_;
  pthread_t clientThread_;
};

#endif //ANSIRC_INCLUDE_UI_MAINWINDOW_H_
