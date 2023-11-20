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
#include <QSequentialAnimationGroup>

static auto constexpr kMessageSpacing = 4;
static auto constexpr kInputSpacing = 5;
static auto constexpr kHorMargin = 6;
#define NO_BORDER_INPUT_MARGINS 0,kHorMargin,0,kHorMargin
#define WITH_BORDER_INPUT_MARGINS 10,kHorMargin,10,kHorMargin
static auto constexpr kLineEditStyle = "background: transparent; border: hidden;";
static auto constexpr kInfoMessageStyle = "color: #666666; font-size: 12px;";
static auto constexpr kErrorMessageStyle = "color: #F24951; font-size: 12px;";
static auto constexpr kAnimationDuration = 200;
static auto constexpr kDisabledBorderColor = "#F2F2F2";
static auto constexpr kFocusBorderColor = "#3C6CFE";
static auto constexpr kNormalBorderColor = "#E4E4E4";
static auto constexpr kErrorBorderColor = "#F24951";
static auto constexpr kDisabledBgColor = "#F2F2F2";
static auto constexpr kFocusBgColor = "#FFFFFF";
static auto constexpr kNormalBgColor = "#FFFFFF";
static auto constexpr kErrorBgColor = "#FFFFFF";
static auto constexpr kDefaultHeight = 32;
static auto constexpr kHeightWithMessage = 54;
static auto constexpr kCopyHintStyleSheet = R"(color: white; font-size: 12px; background: #00C589; border-radius: 4px; padding: 0 4px;)";
static auto constexpr kCopyHintHeight = 24;
static auto constexpr kShowCopyHintDelay = 1000;

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
    struct Colors_t {
        QColor normal, focus, disabled, error;
    };
    Colors_t border_color, bg_color;
    int border_radius = 4;

    // present
    bool first_show = true;
    QColor p_border, p_bg, p_text;
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

    // copy hint
    QLabel *copy_hint = nullptr;
    QSequentialAnimationGroup *hint_anim = nullptr;
    QGraphicsOpacityEffect *hint_effect = nullptr;

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
    void updateCopyHintPosition();

 private:
    Q_DECLARE_PUBLIC(QtTextInput);
    QtTextInput *q_ptr;
};

FNRICE_QT_WIDGETS_END_NAMESPACE

#endif //QTWIDGETS_SRC_QTTEXTINPUT_P_H_
