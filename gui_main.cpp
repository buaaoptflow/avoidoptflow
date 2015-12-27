#include <QtWidgets>
#include "ui_dialog.h"
int gui_main(int argc,char **argv){
    QApplication app(argc,argv);
    QDialog* dd = new QDialog();
    Ui_Dialog *ui = new Ui_Dialog();
    ui->setupUi(dd);
    dd->show();
    return app.exec();
}
