#include <QApplication>
#include <QtImageWidget>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    static auto constexpr kSpacing = 10;
    static auto constexpr kBlockSize = 120;

    auto *p = new QWidget;
    p->setMinimumSize(kBlockSize * 2 + kSpacing * 3, kBlockSize + kSpacing * 2);
    p->resize(kBlockSize * 2 + kSpacing * 3, kBlockSize + kSpacing * 2);

    auto *l = new QVBoxLayout(p);

    auto *i = new QtImageWidget(p);
    i->move(kSpacing, kSpacing);
    i->resize(kBlockSize, kBlockSize);
    i->setPixmap(QPixmap("E:/temp/1.png"));
    i->setBorderWidth(1);
    i->setBorderStyle(Qt::SolidLine);
    i->setBorderColor(Qt::red);
    i->setBorderRadius(10);
    i->setStyleSheet(R"(background: blue;)");

    i->setImageMaximumPixelSize({240, 240});
    i->setImageMaximumPercent(80);

    l->addWidget(i);
    p->show();

    return QApplication::exec();
}
