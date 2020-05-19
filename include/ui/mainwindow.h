#ifndef ANSIRC_INCLUDE_UI_MAINWINDOW_H_
#define ANSIRC_INCLUDE_UI_MAINWINDOW_H_

#include <QMainWindow>
#include <QMetaObject>
#include <QStringListModel>
#include <QString>
#include <QStringList>
#include <QMessageBox>

#include "ui/clientview.h"
#include "ui/login.h"

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
  void onServerChannelsUpdated(const std::vector<std::string>& channels) override;

  void onErrorMessage(std::string message) override;

  void enableControls() override;

  void disableControls() override;

  void createClient(std::string address, uint16_t port, std::string username, std::string realname, std::string nickname) override;
 private:
  void appendMessage(QString qstr);
  void updateStatus(QString status);
  void updateLog();
  void updateUsers();
  void updateChannels();

  void setControlsState(bool);
  void setConnected(bool);

  void onConnectClicked();
  void onDisconnectClicked();
  void init();

  Ui::MainWindow *ui;

  std::unique_ptr<QStringListModel> logModel;
  std::unique_ptr<QStringListModel> usersModel;
  std::unique_ptr<QStringListModel> channelsModel;

  QStringList logList;
  QStringList usersList;
  QStringList channelsList;

  Client *client_;
  pthread_t clientThread_;
  bool clientAlive_;

  QString motd_;

  std::unique_ptr<LoginForm> form_;
};

#endif //ANSIRC_INCLUDE_UI_MAINWINDOW_H_
