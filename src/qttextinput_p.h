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
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>

static auto constexpr kMessageSpacing = 4;
static auto constexpr kInputSpacing = 5;
#define NO_BORDER_INPUT_MARGINS 0,6,0,6
#define WITH_BORDER_INPUT_MARGINS 10,6,10,6
static auto constexpr kLineEditStyle = "background: transparent; border: hidden;";
static auto constexpr kInfoMessageStyle = "color: #666666; font-size: 12px;";
static auto constexpr kErrorMessageStyle = "color: #F24951; font-size: 12px;";
static auto constexpr kAnimationDuration = 200;
static auto constexpr kDisabledBorderColor = "#F2F2F2";
static auto constexpr kFocusBorderColor = "#3C6CFE";
static auto constexpr kNormalBorderColor = "#E4E4E4";
static auto constexpr kErrorBorderColor = "#F24951";
static auto constexpr kDefaultHeight = 32;
static auto constexpr kHeightWithMessage = 54;

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
    QGraphicsOpacityEffect *info_effect = nullptr, *error_effect = nullptr;

    // layout
    QWidget *input_container = nullptr;
    QHBoxLayout *input_layout = nullptr;
    QVBoxLayout *main_layout = nullptr;

    // border
    int border_width = 1;
    Qt::PenStyle border_style = Qt::SolidLine;
    struct BorderColor_t {
        QColor normal = QColor(kNormalBorderColor);
        QColor focus = QColor(kFocusBorderColor);
        QColor disabled = QColor(kDisabledBorderColor);
        QColor error = QColor(kErrorBorderColor);
    } border_color;
    int border_radius = 4;

    // present
    bool first_show = true;
    QColor p_border = border_color.normal, p_bg, p_text;
    int p_msg_h = 0;

    // message
    QString info_message, error_message;
    bool has_error = false;

    // copy on readonly
    bool copy_on_read_only = true;
    QCursor old_cursor;

    // animations
    QVariantAnimation
        *bd_animation = nullptr,
        *bg_animation = nullptr;
    QParallelAnimationGroup
        *msg_animation = nullptr;

 public:
    void playShowMessageAnimation();
    void playHideMessageAnimation();
    void playBorderAnimation();
    void playBackgroundAnimation();

 public:
    void copyAndSelectAll();
    void createOrStopColorAnim(QVariantAnimation *&anim, QColor &target);
    void createOrStopHeightAnim(bool isShow);
    void clearAndHideMessages();

 private:
    Q_DECLARE_PUBLIC(QtTextInput);
    QtTextInput *q_ptr;
};

FNRICE_QT_WIDGETS_END_NAMESPACE

#endif //QTWIDGETS_SRC_QTTEXTINPUT_P_H_
