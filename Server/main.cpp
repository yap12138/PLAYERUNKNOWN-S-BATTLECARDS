#include "bootstrap.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(time(NULL));

    ServerBootstrap w;
    w.show();
    return a.exec();
}
