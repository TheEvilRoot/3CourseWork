//
// Created by Прогрессивный Долгострой on 19.05.20.
//

#include "ui/login.h"
#include "ui_login_dialog.h"

#include <QObject>

LoginForm::LoginForm(ClientView *clientView, QWidget *parent):
  QDialog(parent), ui{new Ui_Dialog}, view_{clientView}
{
  ui->setupUi(this);
  QObject::connect(ui->bConnect, &QPushButton::clicked, this, &LoginForm::onConnectClicked);
}

LoginForm::~LoginForm() {
  delete ui;
}

void LoginForm::onConnectClicked() {
  auto serverValue = ui->leServer->text();
  auto portValue = ui->lePort->text();
  auto username = ui->leUsername->text();
  auto nickname = ui->leNickname->text();
  auto realname = ui->leRealname->text();

  if (serverValue.isEmpty() || portValue.isEmpty() || username.isEmpty()) {
    ui->lError->setText("Please, enter server, port and username first");
    return;
  }

  if (nickname.isEmpty()) nickname = username;
  if (realname.isEmpty()) realname = username;

  uint16_t port = 0;
  bool portSucceed = false;
  port = portValue.toUInt(&portSucceed);
  if (!portSucceed || port > 65535) {
    ui->lError->setText("Port value is invalid");
    return;
  }

  view_->createClient(serverValue.toStdString(), port, username.toStdString(), realname.toStdString(), nickname.toStdString());
}