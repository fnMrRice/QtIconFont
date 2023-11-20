#include "qttextinput.h"
#include "qttextinput_p.h"
#include <QEvent>
#include <QPainter>
#include <QDebug>

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
    d->border_style = style;
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

        auto palette = this->palette();
        qDebug() << "QPalette::Window" << palette.color(QPalette::ColorRole::WindowText);
        qDebug() << "QPalette::Button" << palette.color(QPalette::ColorRole::Button);
        qDebug() << "QPalette::Light" << palette.color(QPalette::ColorRole::Light);
        qDebug() << "QPalette::Midlight" << palette.color(QPalette::ColorRole::Midlight);
        qDebug() << "QPalette::Dark" << palette.color(QPalette::ColorRole::Dark);
        qDebug() << "QPalette::Mid" << palette.color(QPalette::ColorRole::Mid);
        qDebug() << "QPalette::Shadow" << palette.color(QPalette::ColorRole::Shadow);
        qDebug() << "QPalette::Highlight" << palette.color(QPalette::ColorRole::Highlight);
        qDebug() << "QPalette::HighlightedText" << palette.color(QPalette::ColorRole::HighlightedText);
        qDebug() << "QPalette::Link" << palette.color(QPalette::ColorRole::Link);
        qDebug() << "QPalette::LinkVisited" << palette.color(QPalette::ColorRole::LinkVisited);
        qDebug() << "QPalette::ToolTipBase" << palette.color(QPalette::ColorRole::ToolTipBase);
        qDebug() << "QPalette::ToolTipText" << palette.color(QPalette::ColorRole::ToolTipText);
        qDebug() << "QPalette::PlaceholderText" << palette.color(QPalette::ColorRole::PlaceholderText);
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

bool QtTextInput::eventFilter(QObject *watched, QEvent *event) {
    Q_D(QtTextInput);
    if (watched == d->line_edit) {
        if (event->type() == QEvent::FocusIn || event->type() == QEvent::FocusOut) {
            d->playBorderAnimation();
            d->playBackgroundAnimation();
        }
    }
    return false;
}

QtTextInputPrivate::QtTextInputPrivate(QtTextInput *q) : q_ptr(q) {
    this->line_edit = new QLineEdit;
    this->line_edit->setStyleSheet("background: transparent; border: hidden;");

    this->info_label = new QLabel;
    this->info_label->setStyleSheet("color: #666666; font-size: 12px;");
    this->info_label->setText("Here is extra message");
    this->info_label->hide();

    this->error_label = new QLabel;
    this->error_label->setStyleSheet("color: #F24951; font-size: 12px;");
    this->error_label->setText("Here is error message");
    this->error_label->hide();

    this->main_layout = new QVBoxLayout;
    this->main_layout->setContentsMargins(0, 0, 0, 0);
    this->main_layout->setSpacing(4);

    this->input_layout = new QHBoxLayout;
    this->input_layout->setContentsMargins(10, 6, 10, 6);
    this->input_layout->setSpacing(5);

    this->input_layout->addWidget(this->line_edit);

    this->main_layout->addLayout(this->input_layout);
    this->main_layout->addWidget(this->info_label);
    this->main_layout->addWidget(this->error_label);
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

FNRICE_QT_WIDGETS_END_NAMESPACE