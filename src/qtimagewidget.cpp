#include "qtimagewidget.h"
#include <QPainter>
#include <QPaintEvent>

class QtImageWidgetPrivate {
 public:
    explicit QtImageWidgetPrivate(QtImageWidget *q) : q_ptr(q) {}
    ~QtImageWidgetPrivate() = default;

 public:
    QPixmap pixmap, scaled_pixmap;
    Qt::AspectRatioMode image_aspect_ratio_mode = Qt::KeepAspectRatio;
    Qt::Alignment image_alignment = Qt::AlignCenter;
    QSize minimum_pixel_size{}, maximum_pixel_size{};
    int maximum_percent = 100;

    int border_width = 0;
    Qt::PenStyle border_style = Qt::SolidLine;
    QColor border_color;
    int border_radius = 0;

    int margins[4] = {0};

    bool size_changed = false;
    bool pixmap_changed = false;
    bool align_changed = false;
    QSize image_size;
    QPoint image_pos;

 public:
    QSize calculateMinimumSize();
    QSize calculateMaximumSize();
    QSize calculatePixmapSize();

 private:
    Q_DECLARE_PUBLIC(QtImageWidget);
    QtImageWidget *q_ptr;
};

QtImageWidget::QtImageWidget(QWidget *parent)
    : QWidget(parent), d_ptr(new QtImageWidgetPrivate(this)) {
}

QtImageWidget::QtImageWidget(const QPixmap &pixmap, QWidget *parent)
    : QWidget(parent), d_ptr(new QtImageWidgetPrivate(this)) {
    Q_D(QtImageWidget);
    d->pixmap = pixmap;
}

QtImageWidget::~QtImageWidget() {
    delete d_ptr;
}

void QtImageWidget::setPixmap(const QPixmap &pixmap) {
    Q_D(QtImageWidget);
    d->pixmap = pixmap;
    d->pixmap_changed = true;
    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update));
}

QPixmap QtImageWidget::pixmap() const {
    Q_D(const QtImageWidget);
    return d->pixmap;
}

void QtImageWidget::setBorderWidth(int width) {
    Q_D(QtImageWidget);
    d->border_width = width;
    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update));
}

int QtImageWidget::borderWidth() const {
    Q_D(const QtImageWidget);
    return d->border_width;
}

void QtImageWidget::setBorderStyle(Qt::PenStyle style) {
    Q_D(QtImageWidget);
    d->border_style = style;
    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update));
}

Qt::PenStyle QtImageWidget::borderStyle() const {
    Q_D(const QtImageWidget);
    return d->border_style;
}

void QtImageWidget::setBorderColor(const QColor &color) {
    Q_D(QtImageWidget);
    d->border_color = color;
    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update));
}

QColor QtImageWidget::borderColor() const {
    Q_D(const QtImageWidget);
    return d->border_color;
}

void QtImageWidget::setBorderRadius(int radius) {
    Q_D(QtImageWidget);
    d->border_radius = radius;
    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update));
}

int QtImageWidget::borderRadius() const {
    Q_D(const QtImageWidget);
    return d->border_radius;
}

void QtImageWidget::setImageAspectRatioMode(Qt::AspectRatioMode mode) {
    Q_D(QtImageWidget);
    d->image_aspect_ratio_mode = mode;
    d->pixmap_changed = true;
    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update));
}

Qt::AspectRatioMode QtImageWidget::imageAspectRatioMode() const {
    Q_D(const QtImageWidget);
    return d->image_aspect_ratio_mode;
}

void QtImageWidget::setImageAlignment(Qt::Alignment alignment) {
    Q_D(QtImageWidget);
    d->image_alignment = alignment;
    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update));
}

Qt::Alignment QtImageWidget::imageAlignment() const {
    Q_D(const QtImageWidget);
    return d->image_alignment;
}

void QtImageWidget::setImageMinimumPixelSize(const QSize &size) {
    Q_D(QtImageWidget);
    d->minimum_pixel_size = size;
    d->size_changed = true;
    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update));
}

QSize QtImageWidget::imageMinimumPixelSize() const {
    Q_D(const QtImageWidget);
    return d->minimum_pixel_size;
}

void QtImageWidget::setImageMaximumPixelSize(const QSize &size) {
    Q_D(QtImageWidget);
    d->maximum_pixel_size = size;
    d->size_changed = true;
    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update));
}

QSize QtImageWidget::imageMaximumPixelSize() const {
    Q_D(const QtImageWidget);
    return d->maximum_pixel_size;
}

void QtImageWidget::setImageMaximumPercent(int percent) {
    Q_D(QtImageWidget);
    d->maximum_percent = percent;
    d->size_changed = true;
    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update));
}

int QtImageWidget::imageMaximumPercent() const {
    Q_D(const QtImageWidget);
    return d->maximum_percent;
}

void QtImageWidget::setImageMargins(int left, int right, int top, int bottom) {
    Q_D(QtImageWidget);
    d->margins[0] = left;
    d->margins[1] = right;
    d->margins[2] = top;
    d->margins[3] = bottom;
    d->align_changed = true;
    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update));
}

void QtImageWidget::resizeEvent(QResizeEvent *event) {
    Q_D(QtImageWidget);
    d->size_changed = true;
    d->align_changed = true;
    QWidget::resizeEvent(event);
}

void QtImageWidget::paintEvent(QPaintEvent *event) {
    Q_D(QtImageWidget);
    auto const &palette = this->palette();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // ------ draw background begin ------
    painter.save();
    // read border config from properties
    QPen pen;
    pen.setColor(this->borderColor());
    pen.setStyle(this->borderStyle());
    pen.setWidth(this->borderWidth());
    painter.setPen(pen);
    // read background from palette
    painter.setBrush(palette.brush(this->backgroundRole()));
    // draw rounded rect
    painter.drawRoundedRect(this->rect(), this->borderRadius(), this->borderRadius());
    painter.restore();
    // ------ draw background end ------

    // ------ draw pixmap begin ------
    if (d->pixmap.isNull()) { return; } // draw nothing
    bool regen_pixmap = false;
    bool regen_pos = false;
    if (d->size_changed) {
        auto size = d->calculatePixmapSize();
        if (size != d->image_size) {
            d->image_size = size;
            regen_pixmap = true;
            regen_pos = true;
        }
        d->size_changed = false;
    }
    if (d->pixmap_changed) {
        regen_pixmap = true;
        regen_pos = true;
        d->pixmap_changed = false;
    }
    if (d->align_changed) {
        regen_pos = true;
        d->align_changed = false;
    }
    if (regen_pixmap) {
        d->scaled_pixmap = d->pixmap.scaled(d->image_size, this->imageAspectRatioMode());
    }
    if (regen_pos) {
        int x = 0, y = 0;
        auto size = d->scaled_pixmap.size();
        if (d->image_alignment.testFlag(Qt::AlignLeft)) {
            x = d->margins[0];
        } else if (d->image_alignment.testFlag(Qt::AlignHCenter)) {
            x = (this->width() - size.width()) / 2 + d->margins[0] - d->margins[1];
        } else if (d->image_alignment.testFlag(Qt::AlignRight)) {
            x = this->width() - size.width() - d->margins[1];
        }
        if (d->image_alignment.testFlag(Qt::AlignTop)) {
            y = 0 + d->margins[2];
        } else if (d->image_alignment.testFlag(Qt::AlignVCenter)) {
            y = (this->height() - size.height()) / 2 + d->margins[2] - d->margins[3];
        } else if (d->image_alignment.testFlag(Qt::AlignBottom)) {
            x = this->height() - size.height() - d->margins[3];
        }
        d->image_pos = {x, y};
    }
    painter.drawPixmap(d->image_pos, d->scaled_pixmap);
    // ------ draw pixmap end ------
}

QSize QtImageWidgetPrivate::calculateMinimumSize() {
    Q_Q(const QtImageWidget);
    if (this->minimum_pixel_size.isValid()) {
        return this->minimum_pixel_size;
    }
    return {};
}

QSize QtImageWidgetPrivate::calculateMaximumSize() {
    Q_Q(const QtImageWidget);
    if (this->maximum_pixel_size.isValid() && this->maximum_percent > 0) {
        auto w = q->width() * this->maximum_percent / 100;
        auto h = q->height() * this->maximum_percent / 100;
        return {std::min(this->maximum_pixel_size.width(), w),
                std::min(this->maximum_pixel_size.height(), h)};
    }
    if (this->maximum_pixel_size.isValid()) {
        return this->maximum_pixel_size;
    }
    if (this->maximum_percent > 0) {
        auto w = q->width() * this->maximum_percent / 100;
        auto h = q->height() * this->maximum_percent / 100;
        return {w, h};
    }
    return {};
}

QSize QtImageWidgetPrivate::calculatePixmapSize() {
    Q_Q(const QtImageWidget);
    auto min_size = calculateMinimumSize();
    auto max_size = calculateMaximumSize();
    auto size = q->size();
    if (!min_size.isValid() && !max_size.isValid()) {
        // neither min_size nor max_size is valid
        return size;
    }
    if (min_size.isValid() && max_size.isValid()) {
        // ensure min size and max size correct
        if (max_size.width() < min_size.width()) {
            max_size.setWidth(min_size.width());
        }
        if (max_size.height() < min_size.height()) {
            max_size.setHeight(min_size.height());
        }
    }
    if (min_size.isValid()) {
        if (size.width() < min_size.width()) {
            size.setWidth(min_size.width());
        }
        if (size.height() < min_size.height()) {
            size.setHeight(min_size.height());
        }
    }
    if (max_size.isValid()) {
        if (size.width() > max_size.width()) {
            size.setWidth(max_size.width());
        }
        if (size.height() > max_size.height()) {
            size.setHeight(max_size.height());
        }
    }
    return size;
}
