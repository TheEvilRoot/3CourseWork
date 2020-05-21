#pragma once

#include <QDialog>

#include "clientview.h"

QT_BEGIN_NAMESPACE
class Ui_Dialog;
QT_END_NAMESPACE
class LoginForm: public QDialog {

  Q_OBJECT

 public:
  explicit LoginForm(ClientView *, QWidget *parent = nullptr);
  ~LoginForm() override;

  void onConnectClicked();

 private:
  Ui_Dialog *ui;
  ClientView *view_;

};