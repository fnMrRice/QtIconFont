#include <QApplication>
#include <QtIconFont>
#include <QLabel>
#include <QTimer>

FNRICE_QT_WIDGETS_USE_NAMESPACE

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QtIconFont icon_font("iconfont.ttf", "iconfont.json");

    auto *label = new QLabel;
    label->setFont(icon_font.font());
    label->setAlignment(Qt::AlignCenter);
    label->resize(300, 200);

    label->setText("icon font will be shown after 3000ms");
    QTimer::singleShot(3000, [&] {
        label->setText(icon_font.iconByClass("churujingjiekoufuwu"));
    });

    label->show();

    return QApplication::exec();
}
