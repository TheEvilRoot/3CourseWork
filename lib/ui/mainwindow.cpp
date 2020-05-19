#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent), ClientView(), ui{new Ui::MainWindow}, logModel{new QStringListModel} {
  ui->setupUi(this);
  client_ = new Client("127.0.0.1", 6667, "S1lave", "S1lave", "Sl1ave", "hello", this);
  pthread_create(&clientThread_, nullptr, [](void *args) -> void* {
    auto *client = static_cast<Client *>(args);
    auto *parser = new IrcParser;
    auto *handler = new IrcHandler(parser);
    client->setHandler(handler);
    client->connect();
    client->sendJoin("#hello");
    client->joinRead();
    return nullptr;
  }, client_);
  connect(ui->bSend, &QPushButton::clicked, this, [&]() {
    auto value = ui->leMessage->text().toStdString();
    client_->sendRaw(value);
  });
}

void MainWindow::motdUpdate(std::string motd) {
  newMessage("MOTD: \n" + motd + "\nEnd of MOTD\n");
}

void MainWindow::newMessage(std::string msg) {
  QMetaObject::invokeMethod(this, [this, msg]() {
    if (!msg.empty()) {
      logList.append(QString::fromStdString(msg));
      updateLogModel();
    }
  });
}

void MainWindow::updateLogModel() {
  logModel->setStringList(logList);
  ui->listView->setModel(logModel.get());
}