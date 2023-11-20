#include "qttextarea.h"

FNRICE_QT_WIDGETS_BEGIN_NAMESPACE

class QtTextAreaPrivate {
 public:
    explicit QtTextAreaPrivate(QtTextArea *q) : q_ptr(q) {}
    ~QtTextAreaPrivate() = default;

 private:
    Q_DECLARE_PUBLIC(QtTextArea);
    QtTextArea *q_ptr;
};

QtTextArea::QtTextArea(QWidget *parent)
    : QWidget(parent), d_ptr(new QtTextAreaPrivate(this)) {
    Q_D(QtTextArea);
}

QtTextArea::~QtTextArea() {
    Q_D(QtTextArea);
    delete d_ptr;
}

FNRICE_QT_WIDGETS_END_NAMESPACE
