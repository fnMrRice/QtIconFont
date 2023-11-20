#include "qttextinput.h"
#include "qttextinput_p.h"
#include <QEvent>
#include <QPainter>
#include <QClipboard>
#include <QApplication>
#include <QMouseEvent>
#include <QLineEdit>

static auto constexpr kMessageSpacing = 4;
static auto constexpr kInputSpacing = 5;
#define NO_BORDER_INPUT_MARGINS 0,6,0,6
#define WITH_BORDER_INPUT_MARGINS 10,6,10,6
static auto constexpr kLineEditStyle = "background: transparent; border: hidden;";
static auto constexpr kInfoMessageStyle = "color: #666666; font-size: 12px;";
static auto constexpr kErrorMessageStyle = "color: #F24951; font-size: 12px;";

FNRICE_QT_WIDGETS_BEGIN_NAMESPACE

QtTextInput::QtTextInput(QWidget *parent)
    : QWidget(parent), d_ptr(new QtTextInputPrivate(this)) {
    Q_D(QtTextInput);

    this->setObjectName("QtTextInput");

    this->setLayout(d->main_layout);
    this->setCursor(Qt::IBeamCursor);

    connect(d->line_edit, &QLineEdit::textEdited, this, &QtTextInput::textEdited);
    connect(d->line_edit, &QLineEdit::textChanged, this, &QtTextInput::textChanged);
    connect(d->line_edit, &QLineEdit::returnPressed, this, &QtTextInput::returnPressed);
}

QtTextInput::~QtTextInput() {
    Q_D(QtTextInput);
    delete d_ptr;
}

void QtTextInput::setBorderWidth(int width) {
    Q_D(QtTextInput);
    d->border_width = width;
    this->update();
}

void QtTextInput::setBorderStyle(Qt::PenStyle style) {
    Q_D(QtTextInput);
    if (d->border_style == style) return;
    d->border_style = style;
    if (style == Qt::NoPen) {
        d->input_layout->setContentsMargins(NO_BORDER_INPUT_MARGINS);
    } else {
        d->input_layout->setContentsMargins(WITH_BORDER_INPUT_MARGINS);
    }
    this->update();
}

void QtTextInput::setBorderRadius(int radius) {
    Q_D(QtTextInput);
    d->border_radius = radius;
}

int QtTextInput::borderWidth() const {
    Q_D(const QtTextInput);
    return d->border_width;
}

Qt::PenStyle QtTextInput::borderStyle() const {
    Q_D(const QtTextInput);
    return d->border_style;
}

int QtTextInput::borderRadius() const {
    Q_D(const QtTextInput);
    return d->border_radius;
}

void QtTextInput::setText(const QString &text) {
    Q_D(QtTextInput);
    d->line_edit->setText(text);
}

void QtTextInput::setPlaceholderText(const QString &text) {
    Q_D(QtTextInput);
    d->line_edit->setPlaceholderText(text);
}

void QtTextInput::setMaxLength(int length) {
    Q_D(QtTextInput);
    d->line_edit->setMaxLength(length);
}

void QtTextInput::setAlignment(Qt::Alignment alignment) {
    Q_D(QtTextInput);
    d->line_edit->setAlignment(alignment);
}

void QtTextInput::setExtraMessage(const QString &message) {
    Q_D(QtTextInput);
    d->info_label->setText(message);
    if (d->has_error) {
        if (d->error_message.isEmpty()) {
            if (d->info_message.isEmpty()) {
                if (message.isEmpty()) {
                    // do nothing
                } else {
                    d->playShowMessageAnimation();
                }
            }
        }
    } else {
        if (d->info_message.isEmpty()) {
            if (message.isEmpty()) {
                // do nothing
            } else {
                d->playShowMessageAnimation();
            }
        }
    }
    d->info_message = message;
}

void QtTextInput::setErrorMessage(const QString &message) {
    Q_D(QtTextInput);
    d->error_label->setText(message);
    if (!d->has_error) {
        d->has_error = true;
        d->playBorderAnimation();
        if (d->info_message.isEmpty()) {
            d->playShowMessageAnimation();
        } else {
            d->error_label->hide();
            d->info_label->show();
        }
    } else {
        // already has error message
        if (d->error_message.isEmpty()) {
            if (message.isEmpty()) {
                // do nothing
            } else {
                d->playShowMessageAnimation();
            }
        }
    }
    d->error_message = message;
}

void QtTextInput::setValidator(const QValidator *validator) {
    Q_D(QtTextInput);
    d->line_edit->setValidator(validator);
}

void QtTextInput::setLeftButton(QAbstractButton *button) {
    Q_D(QtTextInput);
    if (d->left_button) {
        d->input_layout->removeWidget(d->left_button);
        d->left_button->disconnect(this);
        d->left_button->setParent(nullptr);
        d->left_button->deleteLater();
    }
    d->left_button = button;
    d->input_layout->insertWidget(0, button);
    connect(d->left_button, &QAbstractButton::clicked, this, &QtTextInput::leftButtonClicked);
}

void QtTextInput::setRightButton(QAbstractButton *button) {
    Q_D(QtTextInput);
    if (d->right_button) {
        d->input_layout->removeWidget(d->right_button);
        d->right_button->disconnect(this);
        d->right_button->setParent(nullptr);
        d->right_button->deleteLater();
    }
    d->right_button = button;
    d->input_layout->addWidget(button);
    connect(d->right_button, &QAbstractButton::clicked, this, &QtTextInput::rightButtonClicked);
}

void QtTextInput::setEchoMode(QLineEdit::EchoMode mode) {
    Q_D(QtTextInput);
    d->line_edit->setEchoMode(mode);
}

void QtTextInput::setReadOnly(bool readOnly) {
    Q_D(QtTextInput);
    if (this->isReadOnly() == readOnly) return;
    d->line_edit->setReadOnly(readOnly);
    if (d->copy_on_read_only) {
        if (readOnly) {
            d->old_cursor = this->cursor();
            this->setCursor(Qt::PointingHandCursor);
            d->line_edit->setCursor(Qt::PointingHandCursor);
        } else {
            this->setCursor(d->old_cursor);
            d->line_edit->setCursor(Qt::IBeamCursor);
        }
    }
}

void QtTextInput::setCopyOnReadOnly(bool enable) {
    Q_D(QtTextInput);
    if (d->copy_on_read_only == enable) return;
    d->copy_on_read_only = enable;
    if (this->isReadOnly()) {
        if (enable) {
            d->old_cursor = this->cursor();
            this->setCursor(Qt::PointingHandCursor);
            d->line_edit->setCursor(Qt::PointingHandCursor);
        } else {
            this->setCursor(d->old_cursor);
            d->line_edit->setCursor(Qt::IBeamCursor);
        }
    }
}

QString QtTextInput::text() const {
    Q_D(const QtTextInput);
    return d->line_edit->text();
}

QString QtTextInput::placeholderText() const {
    Q_D(const QtTextInput);
    return d->line_edit->placeholderText();
}

int QtTextInput::maxLength() const {
    Q_D(const QtTextInput);
    return d->line_edit->maxLength();
}

Qt::Alignment QtTextInput::alignment() const {
    Q_D(const QtTextInput);
    return d->line_edit->alignment();
}

QString QtTextInput::extraMessage() const {
    Q_D(const QtTextInput);
    return d->info_message;
}

QString QtTextInput::errorMessage() const {
    Q_D(const QtTextInput);
    if (d->has_error) return d->error_message;
    return {};
}

bool QtTextInput::hasError() const {
    Q_D(const QtTextInput);
    return d->has_error;
}

const QValidator *QtTextInput::validator() const {
    Q_D(const QtTextInput);
    return d->line_edit->validator();
}

QAbstractButton *QtTextInput::leftButton() const {
    Q_D(const QtTextInput);
    return d->left_button;
}

QAbstractButton *QtTextInput::rightButton() const {
    Q_D(const QtTextInput);
    return d->right_button;
}

QLineEdit::EchoMode QtTextInput::echoMode() const {
    Q_D(const QtTextInput);
    return d->line_edit->echoMode();
}

bool QtTextInput::isReadOnly() const {
    Q_D(const QtTextInput);
    return d->line_edit->isReadOnly();
}

bool QtTextInput::isCopyOnReadOnly() const {
    Q_D(const QtTextInput);
    return d->copy_on_read_only;
}

void QtTextInput::selectAll() {
    Q_D(const QtTextInput);
    d->line_edit->selectAll();
}

void QtTextInput::clearExtraMessage() {
    Q_D(QtTextInput);
    if (d->has_error) {
        // error has higher priority
        if (d->error_message.isEmpty()) {
            // do nothing
        } else {
            if (d->info_message.isEmpty()) {
                // do nothing
            } else {
                // in this case, extra message is shown, so we need to hide it
                d->playHideMessageAnimation();
            }
        }
    } else {
        if (d->info_message.isEmpty()) {
            // do nothing
        } else {
            // no error, extra message is shown
            d->playHideMessageAnimation();
        }
    }
    // clear message only, do not clear QLabel which shows the message. It will be used for animation
    d->info_message.clear();
}

void QtTextInput::clearErrorMessage() {
    Q_D(QtTextInput);
    if (d->has_error) {
        d->has_error = false;
        d->playBorderAnimation();
        if (d->error_message.isEmpty()) {
            // do nothing
        } else {
            if (d->info_message.isEmpty()) {
                // hide error message
                d->playHideMessageAnimation();
            } else {
                d->error_label->hide();
                d->info_label->show();
            }
        }
    }
    // clear message only, do not clear QLabel which shows the message. It will be used for animation
    d->has_error = false;
    d->error_message.clear();
}

QSize QtTextInput::sizeHint() const {
    Q_D(const QtTextInput);
    return d->main_layout->sizeHint();
}

QSize QtTextInput::minimumSizeHint() const {
    Q_D(const QtTextInput);
    auto min_width = 150;
    bool has_text = false;
    if ((d->has_error && !d->error_message.isEmpty()) || !d->info_message.isEmpty()) {
        has_text = true;
    }
    return {min_width, has_text ? 32 : 54};
}

void QtTextInput::showEvent(QShowEvent *event) {
    Q_D(QtTextInput);
    if (d->first_show) {
        d->first_show = false;
        // set present colors
        d->p_bg = this->palette().color(this->backgroundRole());
    }
    QWidget::showEvent(event);
}

void QtTextInput::changeEvent(QEvent *event) {
    Q_D(QtTextInput);
    switch (event->type()) {
        case QEvent::EnabledChange:
        case QEvent::PaletteChange:
        case QEvent::StyleChange:
            d->playBorderAnimation();
            d->playBackgroundAnimation();
        default:
            break;
    }
    QWidget::changeEvent(event);
}

void QtTextInput::focusInEvent(QFocusEvent *event) {
    Q_D(QtTextInput);
    d->line_edit->setFocus();
    QWidget::focusInEvent(event);
}

void QtTextInput::paintEvent(QPaintEvent *event) {
    Q_D(QtTextInput);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.save();
    auto pen = painter.pen();
    pen.setWidth(this->borderWidth());
    pen.setStyle(this->borderStyle());
    pen.setColor(d->p_border);
    painter.setPen(pen);

    painter.setBrush(d->p_bg);

    // draw rounded rect
    auto rect = this->rect();
    rect.setHeight(32);
    painter.drawRoundedRect(this->rect(), this->borderRadius(), this->borderRadius());

    painter.restore();
}

void QtTextInput::mousePressEvent(QMouseEvent *event) {
    Q_D(QtTextInput);
    QWidget::mousePressEvent(event);
}

void QtTextInput::mouseReleaseEvent(QMouseEvent *event) {
    Q_D(QtTextInput);
    d->copyAndSelectAll();
    QWidget::mouseReleaseEvent(event);
}

bool QtTextInput::eventFilter(QObject *watched, QEvent *event) {
    Q_D(QtTextInput);
    if (watched == d->line_edit) {
        switch (event->type()) {
            case QEvent::FocusIn:
            case QEvent::FocusOut:
                d->playBorderAnimation();
                d->playBackgroundAnimation();
                return false;
            case QEvent::MouseButtonRelease:
                d->copyAndSelectAll();
                return false;
            default:
                break;
        }
    }
    return false;
}

QtTextInputPrivate::QtTextInputPrivate(QtTextInput *q) : q_ptr(q) {
    this->line_edit = new QLineEdit;
    this->line_edit->setStyleSheet(kLineEditStyle);

    this->info_label = new QLabel;
    this->info_label->setStyleSheet(kInfoMessageStyle);
    this->info_label->setText(QObject::tr("Here is extra message"));
    this->info_label->hide();

    this->error_label = new QLabel;
    this->error_label->setStyleSheet(kErrorMessageStyle);
    this->error_label->setText(QObject::tr("Here is error message"));
    this->error_label->hide();

    this->main_layout = new QVBoxLayout;
    this->main_layout->setContentsMargins(0, 0, 0, 0);
    this->main_layout->setSpacing(kMessageSpacing);

    this->input_layout = new QHBoxLayout;
    this->input_layout->setContentsMargins(WITH_BORDER_INPUT_MARGINS);
    this->input_layout->setSpacing(kInputSpacing);

    this->input_layout->addWidget(this->line_edit);

    this->main_layout->addLayout(this->input_layout);
    this->main_layout->addWidget(this->info_label);
    this->main_layout->addWidget(this->error_label);

    this->line_edit->installEventFilter(q);
}

QtTextInputPrivate::~QtTextInputPrivate() {
    this->line_edit->deleteLater();
    this->info_label->deleteLater();
    this->error_label->deleteLater();
}

void QtTextInputPrivate::playShowMessageAnimation() {
    this->info_label->show();
    this->error_label->show();
    // TODO: add animation
}

void QtTextInputPrivate::playHideMessageAnimation() {
    this->info_label->hide();
    this->error_label->hide();
    // TODO: add animation
}

void QtTextInputPrivate::playBorderAnimation() {
    Q_Q(QtTextInput);
    if (!q->isEnabled()) {
        // disabled_color
        this->p_border = QColor("#F2F2F2");
    } else if (this->has_error) {
        this->p_border = QColor("#F24951");
    } else if (q->hasFocus() || this->line_edit->hasFocus()) {
        this->p_border = QColor("#3C6CFE");
    } else {
        this->p_border = QColor("#E4E4E4");
    }
    q->update();
    // TODO: add animation
}

void QtTextInputPrivate::playBackgroundAnimation() {
    Q_Q(QtTextInput);
    this->p_bg = q->palette().color(q->backgroundRole());
    q->update();
    // TODO: add animation
}

void QtTextInputPrivate::copyAndSelectAll() {
    Q_Q(QtTextInput);
    if (this->copy_on_read_only && q->isReadOnly()) {
        if (q->underMouse()) {
            auto text = this->line_edit->text();
            if (!text.isEmpty()) {
                this->line_edit->selectAll();
                auto *clipboard = QApplication::clipboard();
                clipboard->setText(text);
                emit q->textCopied(text);
            }
        }
    }
}

FNRICE_QT_WIDGETS_END_NAMESPACE
