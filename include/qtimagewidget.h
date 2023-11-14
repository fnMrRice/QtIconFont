#ifndef QTICONFONT_SRC_QTIMAGEWIDGET_H_
#define QTICONFONT_SRC_QTIMAGEWIDGET_H_

#include <QWidget>

class QtImageWidgetPrivate;

class QtImageWidget : public QWidget {
 Q_OBJECT
 public:
    explicit QtImageWidget(QWidget *parent = nullptr);
    explicit QtImageWidget(const QPixmap &pixmap, QWidget *parent = nullptr);
    ~QtImageWidget() override;

 public:
    // basic property
    Q_PROPERTY(QPixmap pixmap WRITE setPixmap READ pixmap)
    // border properties
    Q_PROPERTY(int borderWidth WRITE setBorderWidth READ borderWidth)
    Q_PROPERTY(Qt::PenStyle borderStyle WRITE setBorderStyle READ borderStyle)
    Q_PROPERTY(QColor borderColor WRITE setBorderColor READ borderColor)
    Q_PROPERTY(int borderRadius WRITE setBorderRadius READ borderRadius)
    // image properties
    Q_PROPERTY(Qt::AspectRatioMode imageAspectRatioMode WRITE setImageAspectRatioMode READ imageAspectRatioMode)
    Q_PROPERTY(Qt::Alignment imageAlignment WRITE setImageAlignment READ imageAlignment)
    Q_PROPERTY(QSize imageMinimumPixelSize WRITE setImageMinimumPixelSize READ imageMinimumPixelSize)
    Q_PROPERTY(QSize imageMaximumPixelSize WRITE setImageMaximumPixelSize READ imageMaximumPixelSize)
    Q_PROPERTY(int imageMaximumPercent WRITE setImageMaximumPercent READ imageMaximumPercent)

 public:
    // basic property
    /**
     * @brief set background pixmap
     * @param [in] pixmap pixmap
     */
    void setPixmap(const QPixmap &pixmap);
    [[nodiscard]] QPixmap pixmap() const;
    // border properties
    /**
     * @brief set border width for all directions
     * @param [in] width border width
     */
    void setBorderWidth(int width);
    [[nodiscard]] int borderWidth() const;
    /**
     * @brief set border style for all directions
     * @param [in] style border style
     */
    void setBorderStyle(Qt::PenStyle style);
    [[nodiscard]] Qt::PenStyle borderStyle() const;
    /**
     * @brief set border color for all directions
     * @param [in] color border color
     */
    void setBorderColor(const QColor &color);
    [[nodiscard]] QColor borderColor() const;
    /**
     * @brief set border radius for all directions
     * @param [in] radius border radius
     */
    void setBorderRadius(int radius);
    [[nodiscard]] int borderRadius() const;
    // image properties
    /**
     * @brief set image aspect ratio mode
     * @param [in] mode see <a href="https://doc.qt.io/qt-5/qt.html#AspectRatioMode-enum">qt document</a> for more information
     */
    void setImageAspectRatioMode(Qt::AspectRatioMode mode);
    [[nodiscard]] Qt::AspectRatioMode imageAspectRatioMode() const;
    /**
     * @brief set image alignment
     * @param [in] mode see <a href="https://doc.qt.io/qt-5/qt.html#AlignmentFlag-enum">qt document</a> for more information
     */
    void setImageAlignment(Qt::Alignment alignment);
    [[nodiscard]] Qt::Alignment imageAlignment() const;
    /**
     * @brief set minimum image pixel size
     * @param [in] size set to an invalid QSize means not limited
     * @brief if both imageMinimumPixelSize and imageMinimumPercent are set, <br>
     *        it will calculate the real size and use the bigger one.
     */
    void setImageMinimumPixelSize(const QSize &size);
    [[nodiscard]] QSize imageMinimumPixelSize() const;
    /**
     * @brief set maximum image pixel size
     * @param [in] size set to an invalid QSize means not limited
     * @brief if both imageMaximumPixelSize and imageMaximumPercent are set, <br>
     *        it will calculate the real size and use the smaller one.
     */
    void setImageMaximumPixelSize(const QSize &size);
    [[nodiscard]] QSize imageMaximumPixelSize() const;
    /**
    * @brief set maximum image size percent
    * @param [in] percent set to a value less or equal than 0 means not limited. the default value is 100.
     * @brief if both imageMaximumPixelSize and imageMaximumPercent are set, <br>
     *        it will calculate the real size and use the smaller one.<br>
     *        if calculated maximum size is smaller than imageMinimumPixelSize, <br>
     *        it will use imageMinimumPixelSize. <br>
     *        there is no imageMinimumPercent attribute since it's useless.
    */
    void setImageMaximumPercent(int percent);
    [[nodiscard]] int imageMaximumPercent() const;
    /**
     * @brief set image margins
     * @param [in] left   margin left
     * @param [in] right  margin right
     * @param [in] top    margin top
     * @param [in] bottom margin bottom
     */
    void setImageMargins(int left, int right, int top, int bottom);

 protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

 private:
    Q_DECLARE_PRIVATE(QtImageWidget);
    QtImageWidgetPrivate *d_ptr;
};

#endif //QTICONFONT_SRC_QTIMAGEWIDGET_H_
