#include <QApplication>
#include <QtTextInput>
#include <QLabel>
#include <QTimer>
#include <QToolButton>

FNRICE_QT_WIDGETS_USE_NAMESPACE

static auto constexpr kStyleSheet = R"(
#widget {
    background: white;
}

.QtTextInput {
    background-color: #FF0000;
}

.QtTextInput:disabled {
    background-color: #F2F2F2;
}

#text_input {
    background-color: #FF0000;
}
)";

static auto constexpr kTestStyleSheet = R"(
#widget {
    background: white;
}

#text_input {
    background: #FF0000;
}
)";

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto *w = new QWidget;
    w->setObjectName("widget");
    w->resize(640, 480);
    w->setStyleSheet(kTestStyleSheet);

    auto *l_btn = new QToolButton;
    l_btn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    l_btn->setText("A");
    l_btn->setCursor(Qt::PointingHandCursor);

    auto *r_btn = new QToolButton;
    r_btn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    r_btn->setText("B");
    r_btn->setCursor(Qt::PointingHandCursor);
    r_btn->setCheckable(true);

    auto *text_input = new QtTextInput(w);
    text_input->setObjectName("text_input");
    text_input->move(20, 20);
    text_input->setLeftButton(l_btn);

    auto *text_input2 = new QtTextInput(w);
    text_input->setObjectName("text_input2");
    text_input2->move(20, 60);
    text_input2->setRightButton(r_btn);
    text_input2->setEchoMode(QLineEdit::Password);
    QObject::connect(r_btn, &QAbstractButton::toggled, text_input2, [&](bool checked) {
        if (checked) {
            text_input2->setEchoMode(QLineEdit::Normal);
        } else {
            text_input2->setEchoMode(QLineEdit::Password);
        }
    });

    auto *text_input3 = new QtTextInput(w);
    text_input->setObjectName("text_input3");
    text_input3->move(20, 100);
    text_input3->setReadOnly(true);
    text_input3->setText("123456");

    w->show();

    return QApplication::exec();
}
