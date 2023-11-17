#ifndef QTICONFONT_SRC_QTICONFONT_H_
#define QTICONFONT_SRC_QTICONFONT_H_

#include <QObject>
#include "namespace.h"

QT_FORWARD_DECLARE_CLASS(QFile)
FNRICE_QT_WIDGETS_FORWARD_DECLARE_CLASS(QtIconFontPrivate)

FNRICE_QT_WIDGETS_BEGIN_NAMESPACE

class QtIconFont : public QObject {
 Q_OBJECT
 public:
    /**
     * @brief construct QtIconFont by font's path
     * @param [in] font font file path
     * @param [in] json json file path
     * @param parent
     */
    explicit QtIconFont(const QString &font, const QString &json, QObject *parent = nullptr);
    /**
     * @brief construct QtIconFont by file
     * @param [in] font font file
     * @param [in] json json file
     * @param parent
     */
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
    /**
     * @brief check if the font is loaded
     * @param [in] font_name font name, should be alias name or font name
     * @return
     */
    [[nodiscard]] static bool HasIconFont(const QString &font_name);
    /**
     * @brief get the object by font name
     * @param [in] font_name font name, should be alias name or font name
     * @return if the font not loaded, return nullptr
     */
    [[nodiscard]] static QtIconFont *GetIconFont(const QString &font_name);

 public:
    /**
     * @brief set font alias name, to prevent non-ascii characters
     * @param [in] alias alias name
     */
    void setAliasName(const QString &alias);
    /**
     * @brief get font alias name
     * @return alias name
     */
    [[nodiscard]] QString aliasName() const;

 public:
    /**
     * @brief get whether the font is valid
     * @return
     */
    [[nodiscard]] bool isValid() const;
    /**
     * @brief get the font name, not the alias name
     * @return
     */
    [[nodiscard]] QString fontName() const;
    /**
     * @brief get the font's description, it is read from json
     * @return
     */
    [[nodiscard]] QString description() const;
    /**
     * @brief get the font family name
     * @return
     */
    [[nodiscard]] QString fontFamily() const;
    /**
     * @brief get the font
     * @return
     */
    [[nodiscard]] QFont font() const;
    /**
     * @brief get font info by the icon's class name
     * @param [in] name class name, example: "alert", "memory"
     * @return if not found, return nullptr
     */
    [[nodiscard]] FontInfoPtr_t fontInfoByClass(const QString &name) const;
    /**
     * @brief get icon by the icon's class name
     * @param [in] name class name, example: "pause", "memory"
     * @return if not font, returns null char
     */
    [[nodiscard]] QChar iconByClass(const QString &name) const;
    /**
     * @brief get font info by the icon's id
     * @param [in] id icon id, example: "35113170", "35113171"
     * @return if not font, returns null char
     */
    [[nodiscard]] FontInfoPtr_t fontInfoById(const QString &id) const;
    /**
     * @brief get icon by the icon's id
     * @param [in] id icon id, example: "35113170", "35113171"
     * @return if not font, returns null char
     */
    [[nodiscard]] QChar iconById(const QString &id) const;

 private:
    Q_DECLARE_PRIVATE(QtIconFont);
    QtIconFontPrivate *d_ptr;
};

FNRICE_QT_WIDGETS_END_NAMESPACE

#endif //QTICONFONT_SRC_QTICONFONT_H_
