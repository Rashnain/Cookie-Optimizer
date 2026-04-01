#include <QApplication>

#include "cookie_optimizer.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QGuiApplication::setApplicationDisplayName("Cookie Optimizer");
    QGuiApplication::setWindowIcon(QIcon("/media/stockage_2/SteamLibrary/steamapps/common/Cookie Clicker/resources/app/src/img/goldCookie.png"));

    CookieOptimizer dialog;
    dialog.show();

    return QCoreApplication::exec();
}
