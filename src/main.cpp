#include <QApplication>

#include <iostream>

#include <unistd.h>
#include <csignal>
#include <sys/wait.h>

#include "client.h"
#include "socket.h"

#include "ui/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);
  auto *window = new MainWindow(nullptr);
  window->show();
  return QApplication::exec();
}
