#include <QApplication>

#include "cookie_optimizer.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QGuiApplication::setApplicationDisplayName("Cookie Optimizer v1.4.2");
    QGuiApplication::setWindowIcon(QIcon("goldCookie.png"));

    CookieOptimizer dialog;
    dialog.show();

    return QCoreApplication::exec();
}
