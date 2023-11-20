#ifndef QTWIDGETS_SRC_QTTEXTAREA_H_
#define QTWIDGETS_SRC_QTTEXTAREA_H_

#include <QWidget>
#include "namespace.h"

FNRICE_QT_WIDGETS_FORWARD_DECLARE_CLASS(QtTextAreaPrivate)

FNRICE_QT_WIDGETS_BEGIN_NAMESPACE

class QtTextArea : public QWidget {
 Q_OBJECT
 public:
    explicit QtTextArea(QWidget *parent = nullptr);
    ~QtTextArea() override;

 private:
    Q_DECLARE_PRIVATE(QtTextArea);
    QtTextAreaPrivate *d_ptr;
};

FNRICE_QT_WIDGETS_END_NAMESPACE

#endif //QTWIDGETS_SRC_QTTEXTAREA_H_
