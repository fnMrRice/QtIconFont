#ifndef QTWIDGETS_SRC_QTTEXTINPUT_P_H_
#define QTWIDGETS_SRC_QTTEXTINPUT_P_H_

#include "namespace.h"
FNRICE_QT_WIDGETS_USE_NAMESPACE

#include "qttextinput.h"
#include <QLineEdit>
#include <QLabel>
#include <QAbstractButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

FNRICE_QT_WIDGETS_BEGIN_NAMESPACE

class QtTextInputPrivate {
 public:
    explicit QtTextInputPrivate(QtTextInput *q);
    ~QtTextInputPrivate();

 public:
    // widgets
    QLineEdit *line_edit = nullptr;
    QLabel *info_label = nullptr, *error_label = nullptr;
    QAbstractButton *left_button = nullptr, *right_button = nullptr;

    // layout
    QHBoxLayout *input_layout = nullptr;
    QVBoxLayout *main_layout = nullptr;

    // border
    int border_width = 1;
    Qt::PenStyle border_style = Qt::SolidLine;
    QColor border_color = QColor("#E4E4E4");
    int border_radius = 4;

    // present
    bool first_show = true;
    QColor p_border, p_bg, p_text;
    int p_msg_h = 0;

    // message
    QString info_message, error_message;
    bool has_error = false;

 public:
    void playShowMessageAnimation();
    void playHideMessageAnimation();
    void playBorderAnimation();
    void playBackgroundAnimation();

 private:
    Q_DECLARE_PUBLIC(QtTextInput);
    QtTextInput *q_ptr;
};

class QtTextInputPresentObject : public QObject {
 public:
    QtTextInputPresentObject() = default;
    ~QtTextInputPresentObject() override = default;

 public:
    QColor border_color;
    QColor background_color;
};

FNRICE_QT_WIDGETS_END_NAMESPACE

#endif //QTWIDGETS_SRC_QTTEXTINPUT_P_H_
