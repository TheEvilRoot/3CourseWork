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
  ~MainWindow() override;

  void onMOTDUpdate(std::string motd) override;
  void onPrivateMessage(std::string message) override;
  void onUserJoin(std::string message) override;

  void onPing() override;
  void onPong() override;

  void onConnected() override;
  void onDisconnected() override;
  void onConnectionFailure(std::string reason) override;

  void onCurrentChannelUsersUpdated(const std::vector<std::string>& users) override;

  void updateLogModel();

 private:
  void appendMessage(QString qstr);
  void updateStatus(QString status);
  void updateUsers();

  void createClient();
  void init();

  Ui::MainWindow *ui;

  std::unique_ptr<QStringListModel> logModel;
  std::unique_ptr<QStringListModel> usersModel;

  QStringList logList;
  QStringList usersList;

  Client *client_;
  pthread_t clientThread_;

  QString motd_;
};

#endif //ANSIRC_INCLUDE_UI_MAINWINDOW_H_
