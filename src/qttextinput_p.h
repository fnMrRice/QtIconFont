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
#include <QVariantAnimation>

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
    QColor p_border = border_color, p_bg, p_text;
    int p_msg_h = 0;

    // message
    QString info_message, error_message;
    bool has_error = false;

    // copy on readonly
    bool copy_on_read_only = true;
    QCursor old_cursor;

    // animations
    QVariantAnimation *border_animation = nullptr;

 public:
    void playShowMessageAnimation();
    void playHideMessageAnimation();
    void playBorderAnimation();
    void playBackgroundAnimation();

 public:
    void copyAndSelectAll();
    void createOrStopAnim(QVariantAnimation *&anim, const QColor &start_color);

 private:
    Q_DECLARE_PUBLIC(QtTextInput);
    QtTextInput *q_ptr;
};

FNRICE_QT_WIDGETS_END_NAMESPACE

#endif //QTWIDGETS_SRC_QTTEXTINPUT_P_H_
