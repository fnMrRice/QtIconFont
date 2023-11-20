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
    void setBackgroundColor(QtTextInput::InputState state, const QColor &color);

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

    void setText(const QString &text);
    void setPlaceholderText(const QString &text);
    void setMaxLength(int length);
    void setAlignment(Qt::Alignment alignment);
    void setExtraMessage(const QString &message);
    void setErrorMessage(const QString &message);
    void setValidator(const QValidator *validator);
    void setLeftButton(QAbstractButton *button);
    void setRightButton(QAbstractButton *button);
    void setEchoMode(QLineEdit::EchoMode mode);
    void setReadOnly(bool readOnly);
    void setCopyOnReadOnly(bool enable);

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

 public:
    Q_INVOKABLE void selectAll();
    Q_INVOKABLE void clearExtraMessage();
    Q_INVOKABLE void clearErrorMessage();

 public:
    [[nodiscard]] QSize sizeHint() const override;
    [[nodiscard]] QSize minimumSizeHint() const override;

 protected:
    void showEvent(QShowEvent *event) override;
    void changeEvent(QEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

 Q_SIGNALS:
    void textChanged(const QString &text);
    void textEdited(const QString &text);
    void returnPressed();
    void leftButtonClicked(bool checked);
    void rightButtonClicked(bool checked);
    void textCopied(const QString &text);

 private:
    Q_DECLARE_PRIVATE(QtTextInput);
    QtTextInputPrivate *d_ptr;
};

FNRICE_QT_WIDGETS_END_NAMESPACE

#endif //QTWIDGETS_SRC_QTTEXTINPUT_H_
