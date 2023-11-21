#include <QApplication>
#include <QtTextInput>
#include <QLabel>
#include <QTimer>
#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

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
    auto *l = new QVBoxLayout(w);

    auto *l_btn = new QToolButton;
    l_btn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    l_btn->setText("A");
    l_btn->setCursor(Qt::PointingHandCursor);

    auto *text_input = new QtTextInput(w);
    text_input->setObjectName("text_input");
    text_input->setLeftButton(l_btn);
    l->addWidget(text_input);

    auto *text_input2 = new QtClearableInput(w);
    text_input->setObjectName("text_input2");
    l->addWidget(text_input2);

    auto *text_input3 = new QtTextInput(w);
    text_input->setObjectName("text_input3");
    text_input3->setReadOnly(true);
    text_input3->setText("123456");
    l->addWidget(text_input3);

    auto *buttons = new QWidget(w);
    auto *layout = new QHBoxLayout(buttons);
    auto *btn_error = new QPushButton("Set error");
    auto *btn_msg = new QPushButton("Set message");
    auto *btn_clear_err = new QPushButton("Clear error");
    auto *btn_clear_msg = new QPushButton("Clear message");
    QObject::connect(btn_error, &QAbstractButton::clicked, text_input, [text_input] {
        text_input->setErrorMessage("This is an error message.");
        text_input->setFocus();
    });
    QObject::connect(btn_msg, &QAbstractButton::clicked, text_input, [text_input] {
        text_input->setExtraMessage("This is a normal message.");
        text_input->setFocus();
    });
    QObject::connect(btn_clear_err, &QAbstractButton::clicked, text_input, [text_input] {
        text_input->clearErrorMessage();
        text_input->setFocus();
    });
    QObject::connect(btn_clear_msg, &QAbstractButton::clicked, text_input, [text_input] {
        text_input->clearExtraMessage();
        text_input->setFocus();
    });
    layout->addWidget(btn_error);
    layout->addWidget(btn_msg);
    layout->addWidget(btn_clear_err);
    layout->addWidget(btn_clear_msg);
    l->addWidget(buttons);

    w->show();

    return QApplication::exec();
}
