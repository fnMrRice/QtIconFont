#ifndef QTWIDGETS_SRC_QTTEXTINPUT_H_
#define QTWIDGETS_SRC_QTTEXTINPUT_H_

#include <QWidget>
#include <QLineEdit>
#include "namespace.h"

QT_FORWARD_DECLARE_CLASS(QValidator);
QT_FORWARD_DECLARE_CLASS(QAbstractButton);
FNRICE_QT_WIDGETS_FORWARD_DECLARE_CLASS(QtTextInputPrivate)

FNRICE_QT_WIDGETS_BEGIN_NAMESPACE

class QtTextInput : public QWidget {
 Q_OBJECT
 public:
    explicit QtTextInput(QWidget *parent = nullptr);
    ~QtTextInput() override;

 public: // enums
    enum InputState {
        StateNormal = 0,
        StateDisabled,
        StateFocus,
        StateError,
    };
    Q_ENUM(InputState)

 public: // style properties
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(Qt::PenStyle borderStyle READ borderStyle WRITE setBorderStyle)
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius)

    void setBorderWidth(int width);
    void setBorderStyle(Qt::PenStyle style);
    void setBorderRadius(int radius);
    void setBorderColor(QtTextInput::InputState state, const QColor &color);
    void setBorderColor(const QColor &color);
    void setBackgroundColor(QtTextInput::InputState state, const QColor &color);
    void setBackgroundColor(const QColor &color);

    [[nodiscard]] int borderWidth() const;
    [[nodiscard]] Qt::PenStyle borderStyle() const;
    [[nodiscard]] int borderRadius() const;
    [[nodiscard]] QColor borderColor(QtTextInput::InputState type) const;
    [[nodiscard]] QColor backgroundColor(QtTextInput::InputState type) const;

 public: // normal properties
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)
    Q_PROPERTY(int maxLength READ maxLength WRITE setMaxLength)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(QString extraMessage READ extraMessage WRITE setExtraMessage)
    Q_PROPERTY(QString errorMessage READ errorMessage WRITE setErrorMessage)
    Q_PROPERTY(bool hasError READ hasError)
    Q_PROPERTY(const QValidator *validator READ validator WRITE setValidator)
    Q_PROPERTY(QAbstractButton * leftButton READ leftButton WRITE setLeftButton)
    Q_PROPERTY(QAbstractButton * rightButton READ rightButton WRITE setRightButton)
    Q_PROPERTY(QLineEdit::EchoMode echoMode READ echoMode WRITE setEchoMode)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool copyOnReadOnly READ isCopyOnReadOnly WRITE setCopyOnReadOnly)
    Q_PROPERTY(QString copyHint READ copyHint WRITE setCopyHint)

    void setText(const QString &text); // same as QLineEdit
    void setPlaceholderText(const QString &text); // same as QLineEdit
    void setMaxLength(int length); // same as QLineEdit
    void setAlignment(Qt::Alignment alignment); // same as QLineEdit
    void setExtraMessage(const QString &message); // extra message, which will be shown under the input
    void setErrorMessage(const QString &message); // error message, which will be shown under the input, it has higher priority than the extra message
    void setValidator(const QValidator *validator); // same as QLineEdit
    void setLeftButton(QAbstractButton *button); // left button, which will be shown before the input
    void setRightButton(QAbstractButton *button); // right button, which will be shown after the input
    void setEchoMode(QLineEdit::EchoMode mode); // same as QLineEdit
    void setReadOnly(bool readOnly); // same as QLineEdit
    void setCopyOnReadOnly(bool enable); // whether it will copy the content when it is clicked when it is readonly or not
    void setCopyHint(const QString &hint); // set copy message, will be shown after text copied on the text end. useful when you want to translate to other languages without qt linguist

    [[nodiscard]] QString text() const;
    [[nodiscard]] QString placeholderText() const;
    [[nodiscard]] int maxLength() const;
    [[nodiscard]] Qt::Alignment alignment() const;
    [[nodiscard]] QString extraMessage() const;
    [[nodiscard]] QString errorMessage() const;
    [[nodiscard]] bool hasError() const;
    [[nodiscard]] const QValidator *validator() const;
    [[nodiscard]] QAbstractButton *leftButton() const;
    [[nodiscard]] QAbstractButton *rightButton() const;
    [[nodiscard]] QLineEdit::EchoMode echoMode() const;
    [[nodiscard]] bool isReadOnly() const;
    [[nodiscard]] bool isCopyOnReadOnly() const;
    [[nodiscard]] QString copyHint() const;

 public:
    Q_INVOKABLE void selectAll();
    Q_INVOKABLE void clearText();
    Q_INVOKABLE void clearExtraMessage();
    Q_INVOKABLE void clearErrorMessage();
    Q_INVOKABLE void clearAll();

 public:
    [[nodiscard]] QSize sizeHint() const override;
    [[nodiscard]] QSize minimumSizeHint() const override;

 protected:
    void showEvent(QShowEvent *event) override; // not used yet
    void resizeEvent(QResizeEvent *event) override; // to move left/right buttons
    void changeEvent(QEvent *event) override; // for border/bg change
    void focusInEvent(QFocusEvent *event) override; // for border/bg change
    void paintEvent(QPaintEvent *event) override; // for border/bg change
    void mousePressEvent(QMouseEvent *event) override; // for border/bg change
    void mouseReleaseEvent(QMouseEvent *event) override; // for border/bg change
    bool eventFilter(QObject *watched, QEvent *event) override; // installed for internal QLineEdit

 Q_SIGNALS:
    void textChanged(const QString &text); // same as QLineEdit
    void textEdited(const QString &text); // same as QLineEdit
    void returnPressed(); // same as QLineEdit
    void textCopied(const QString &text); // when clicked and copied, this signal will be emitted

 private:
    Q_DECLARE_PRIVATE(QtTextInput);
    QtTextInputPrivate *d_ptr;
};

class QtTextOutput : public QtTextInput {
 public:
    explicit QtTextOutput(const QString &text, QWidget *parent = nullptr);
    ~QtTextOutput() override;

    // ------ helpful notes
    // call setCopyHint(QString) for chinese
    // call setStyleSheet("color: color;") for text color
    // call setCopyOnReadOnly(bool) for enable/disable copy on click
};

class QtClearableInputPrivate;

/**
 * @brief A input with a clear button at right
 */
class QtClearableInput : public QtTextInput {
 public:
    explicit QtClearableInput(QWidget *parent = nullptr);
    ~QtClearableInput() override;

 public:
    Q_PROPERTY(QPixmap clearButtonPixmap READ clearButtonPixmap WRITE setClearButtonPixmap)

    /**
     * @brief set clear button pixmap, it must be set before use, otherwise it shows noting
     * @param [in] pixmap
     */
    void setClearButtonPixmap(const QPixmap &pixmap);
    [[nodiscard]] QPixmap clearButtonPixmap() const;

 protected:
    void enterEvent(QEvent *event) override; // for show animation
    void leaveEvent(QEvent *event) override; // for hide animation

 private:
    Q_DECLARE_PRIVATE(QtClearableInput);
    QtClearableInputPrivate *d_ptr;
};

FNRICE_QT_WIDGETS_END_NAMESPACE

#endif //QTWIDGETS_SRC_QTTEXTINPUT_H_
