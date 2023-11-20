#include <QApplication>
#include <QtTextInput>
#include <QLabel>
#include <QTimer>

FNRICE_QT_WIDGETS_USE_NAMESPACE

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto *w = new QWidget;
    w->resize(640, 480);
    w->setStyleSheet(R"(
QtTextInput {
    background-color: #FF0000;
}

QtTextInput:disabled {
    background-color: #F2F2F2;
}
)");

    auto *text_input = new QtTextInput(w);
    text_input->move(20, 20);

    auto *text_input2 = new QtTextInput(w);
    text_input2->move(20, 60);

    auto *text_input3 = new QtTextInput(w);
    text_input3->move(20, 100);
    text_input3->setEnabled(false);

    w->show();

    return QApplication::exec();
}
