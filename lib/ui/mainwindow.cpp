#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent),
  ClientView(),
  ui{new Ui::MainWindow},
  logModel{new QStringListModel},
  usersModel{new QStringListModel},
  channelsModel{new QStringListModel} {
  ui->setupUi(this);
  init();
  updateStatus("Idle");
  setConnected(false);
}

void MainWindow::createClient(const char *address, uint16_t port, const std::string &username, const std::string &realname, const std::string &nickname) {
  if (client_ != nullptr && client_->isConnected() || clientAlive_) {
    QMessageBox::critical(this, "Create client", "Unable to create new client while previous is working.\nWait for 'Idle' status and try again");
    return;
  }
  delete client_;
  client_ = new Client(address,
                       port,
                       username,
                       realname,
                       nickname,
                       "",
                       this);
  pthread_create(&clientThread_, nullptr, [](void *args) -> void* {
    auto *self = static_cast<MainWindow *>(args);
    auto client = self->client_;

    self->clientAlive_ = true;
    self->setConnected(true);

    auto *parser = new IrcParser;
    auto *handler = new IrcHandler(parser);
    client->setHandler(handler);
    client->connect();
    client->joinRead();

    self->updateStatus("Idle");
    self->clientAlive_ = false;
    self->enableControls();
    self->setConnected(false);

    return nullptr;
  }, this);
}

void MainWindow::init() {
  connect(ui->bSend, &QPushButton::clicked, this, [&]() {
    auto value = ui->leMessage->text().toStdString();
    if (!value.empty()) {
      if (value[0] == '/') {
        value.erase(value.begin());
        client_->sendRaw(value);
      } else {
        client_->sendCurrentChannelMessage(value);
      }
      ui->leMessage->clear();
      ui->leMessage->repaint();
    }
  });

  connect(ui->bLeave, &QPushButton::clicked, this, [&]() {
    if (!client_->getChannel().empty()) {
      client_->leaveChannel();
    }
  });
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::appendMessage(QString msg) {
  QMetaObject::invokeMethod(this, [this, msg]() {
    if (!msg.isEmpty()) {
      logList.append(msg);
      updateLog();
    }
  });
}

void MainWindow::updateStatus(QString status) {
  QMetaObject::invokeMethod(this, [this, status]() {
    ui->lStatus->setText("Status: " + status);
    ui->lStatus->repaint();
  });
}

void MainWindow::updateUsers() {
  QMetaObject::invokeMethod(this, [this]() {
    ui->lwUsers->setModel(usersModel.get());
    ui->lwUsers->repaint();
  });
}

void MainWindow::updateLog() {
  logModel->setStringList(logList);
  ui->lwLog->setModel(logModel.get());
  ui->lwLog->scrollToBottom();
  ui->lwLog->repaint();
}

void MainWindow::updateChannels() {
  channelsModel->setStringList(channelsList);
  ui->lwChannels->setModel(channelsModel.get());
}

void MainWindow::onMOTDUpdate(std::string motd) {
  if (motd_.isEmpty()) {
    motd_ = QString::fromStdString(motd);
    appendMessage(motd_);
  }
}

void MainWindow::onPrivateMessage(std::string message) {
  appendMessage(QString::fromStdString(message));
}

void MainWindow::onUserJoin(std::string message) {
  appendMessage(QString::fromStdString(message));
}

void MainWindow::onPing() {
  updateStatus("Pinging...");
}

void MainWindow::onPong() {
  updateStatus("Connected");
}

void MainWindow::onConnected() {
  updateStatus("Connected");
}

void MainWindow::onDisconnected() {
  updateStatus("Disconnected");
}

void MainWindow::onConnectionFailure(std::string reason) {
  updateStatus(QString::fromStdString("Connection failure: " + reason));
}

void MainWindow::onCurrentChannelUsersUpdated(const std::vector<std::string>& users) {
  usersList.clear();
  for (const auto & u : users)
    usersList.append(QString::fromStdString(u));
  usersModel->setStringList(usersList);
  updateUsers();
}

void MainWindow::onServerChannelsUpdated(const std::vector<std::string>& channels) {
  channelsList.clear();
  for (const auto & c : channels)
    channelsList.append(QString::fromStdString(c));
  channelsModel->setStringList(channelsList);
  updateChannels();
}

void MainWindow::onErrorMessage(std::string message) {
  appendMessage(QString::fromStdString("Error: " + message));
}

void MainWindow::enableControls() {
  setControlsState(true);
}

void MainWindow::disableControls() {
  setControlsState(false);
}

void MainWindow::setControlsState(bool state) {
  QMetaObject::invokeMethod(this, [this, state] () {
    ui->bDisconnect->setDisabled(!state);
    ui->bLeave->setDisabled(!state);
    ui->bSend->setDisabled(!state);
    ui->lwUsers->setDisabled(!state);
    ui->lwChannels->setDisabled(!state);
  });
}

void MainWindow::setConnected(bool cn) {
  QMetaObject::invokeMethod(this, [this, cn]() {
    if (!cn) {
      ui->bDisconnect->setText("New connection");
      disconnect(ui->bDisconnect, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);
      connect(ui->bDisconnect, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    } else {
      ui->bDisconnect->setText("Disconnect");
      disconnect(ui->bDisconnect, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
      connect(ui->bDisconnect, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);
    }
  });
}

void MainWindow::onDisconnectClicked() {
  client_->shutdown();
  disableControls();
}

void MainWindow::onConnectClicked() {
  createClient("127.0.0.1", 6667, "S1ave", "S1ave", "S1ave");
}