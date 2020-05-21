#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent),
  ClientView(),
  ui{new Ui::MainWindow},
  logModel{new QStringListModel},
  usersModel{new QStringListModel},
  channelsModel{new QStringListModel},
  client_{nullptr},
  clientAlive_{false} {
  ui->setupUi(this);
  init();
  updateStatus("Idle");
  setConnected(false);
}

void MainWindow::createClient(std::string address, uint16_t port, std::string username, std::string realname, std::string nickname) {
  form_.reset();
  if (clientAlive_) {
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

    self->updateStatus("Connecting...");
    self->disableControls();

    auto *parser = new IrcParser;
    auto *handler = new IrcHandler(parser);
    client->setHandler(handler);
    client->connect();

    self->updateStatus("Connected");
    self->enableControls();

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
    if (clientAlive_) {
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
    }
  });

  connect(ui->bLeave, &QPushButton::clicked, this, [&]() {
    if (clientAlive_) {
      if (!client_->getChannel().empty()) {
        client_->leaveChannel();
      }
    }
  });
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::appendMessage(QString msg) {
  QMetaObject::invokeMethod(this, [this, msg]() {
    if (!msg.isEmpty()) {
      logList.append(msg.trimmed());
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
  auto msg = QString::fromStdString("Connection failure: " + reason);
  updateStatus(msg);
  appendMessage(msg);
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

void MainWindow::onUnknownMessage(std::string message) {
  appendMessage(QString::fromStdString(message));
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
  if (clientAlive_) {
    client_->shutdown();
    disableControls();
    usersList.clear();
    channelsList.clear();
    updateUsers();
    updateChannels();
  }
}

void MainWindow::onConnectClicked() {
  if (!form_) {
    form_ = std::make_unique<LoginForm>(this, this);
    form_->show();
  } else {
    form_->setFocus();
  }
}