#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent),
  ClientView(),
  ui{new Ui::MainWindow},
  logModel{new QStringListModel},
  usersModel{new QStringListModel} {
  ui->setupUi(this);
  init();
  createClient();
}

void MainWindow::createClient() {
  client_ = new Client("127.0.0.1",
                       6667,
                       "S1lave",
                       "S1lave",
                       "Sl1ave",
                       "",
                       this);
  pthread_create(&clientThread_, nullptr, [](void *args) -> void* {
    auto *client = static_cast<Client *>(args);
    auto *parser = new IrcParser;
    auto *handler = new IrcHandler(parser);
    client->setHandler(handler);
    client->connect();
    client->joinRead();
    return nullptr;
  }, client_);
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
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::appendMessage(QString msg) {
  QMetaObject::invokeMethod(this, [this, msg]() {
    if (!msg.isEmpty()) {
      logList.append(msg);
      updateLogModel();
      ui->listView->scrollToBottom();
      ui->listView->repaint();
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
void MainWindow::updateLogModel() {
  logModel->setStringList(logList);
  ui->listView->setModel(logModel.get());
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
