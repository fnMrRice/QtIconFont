#ifndef QTICONFONT_SRC_QTICONFONT_H_
#define QTICONFONT_SRC_QTICONFONT_H_

#include <QObject>

class QFile;
class QtIconFontPrivate;

class QtIconFont : public QObject {
 Q_OBJECT
 public:
    explicit QtIconFont(const QString &font, const QString &json, QObject *parent = nullptr);
    explicit QtIconFont(QFile *font, QFile *json, QObject *parent = nullptr);
    ~QtIconFont() override;

 public:
    struct FontInfo_t {
        QString icon_id;
        QString name;
        QString font_class;
        QString unicode;
        uint32_t unicode_decimal;
    };
    using FontInfoPtr_t = QSharedPointer<FontInfo_t>;

 public:
    [[nodiscard]] bool isValid() const;
    [[nodiscard]] QString fontFamily() const;
    [[nodiscard]] QFont font() const;
    [[nodiscard]] FontInfoPtr_t fontInfoByClass(const QString &name) const;
    [[nodiscard]] QChar iconByClass(const QString &name) const;
    [[nodiscard]] FontInfoPtr_t fontInfoById(const QString &name) const;
    [[nodiscard]] QChar iconById(const QString &name) const;

 private:
    Q_DECLARE_PRIVATE(QtIconFont);
    QtIconFontPrivate *d_ptr;
};

#endif //QTICONFONT_SRC_QTICONFONT_H_
