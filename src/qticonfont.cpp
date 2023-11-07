#include "qticonfont.h"
#include <QFile>
#include <QFont>
#include <QFontDatabase>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using FontInfo_t = QtIconFont::FontInfo_t;
using FontInfoPtr_t = QtIconFont::FontInfoPtr_t;

class QtIconFontPrivate {
 public:
    QString font_name;
    QString description;
    QString font_family;
    QMap<QString, FontInfoPtr_t> icon_id_map;
    QMap<QString, FontInfoPtr_t> name_map;
    QMap<QString, FontInfoPtr_t> font_class_map;

 public:
    static bool openFile(QFile *file);
    static FontInfoPtr_t ReadFontInfo(const QJsonObject &obj);
    bool parseJsonData(const QByteArray &data, const QString &file_name);
    void loadFontFromFile(QFile *font, QFile *json);
};

QtIconFont::QtIconFont(const QString &font, const QString &json, QObject *parent)
    : QObject(parent), d_ptr(new QtIconFontPrivate) {
    Q_D(QtIconFont);
    auto font_file = new QFile(font);
    auto json_file = new QFile(json);
    d->loadFontFromFile(font_file, json_file);
}

QtIconFont::QtIconFont(QFile *font, QFile *json, QObject *parent)
    : QObject(parent), d_ptr(new QtIconFontPrivate) {
    Q_D(QtIconFont);
    d->loadFontFromFile(font, json);
}

QtIconFont::~QtIconFont() {
    delete d_ptr;
}

bool QtIconFont::isValid() const {
    Q_D(const QtIconFont);
    return !d->font_family.isEmpty();
}

QString QtIconFont::fontFamily() const {
    Q_D(const QtIconFont);
    return d->font_family;
}

QFont QtIconFont::font() const {
    Q_D(const QtIconFont);
    return {d->font_family};
}

template<class T>
FontInfoPtr_t FindInfo(const QMap<T, FontInfoPtr_t> &map, const T &name) {
    auto iter = map.find(name);
    if (iter == map.end()) {
        return nullptr;
    }
    return iter.value();
}

template<class T>
QChar FindIcon(const QMap<T, FontInfoPtr_t> &map, const T &name) {
    auto iter = map.find(name);
    if (iter == map.end()) {
        return {};
    }
    return {iter.value()->unicode_decimal};
}

FontInfoPtr_t QtIconFont::fontInfoByClass(const QString &name) const {
    Q_D(const QtIconFont);
    return FindInfo(d->font_class_map, name);
}

QChar QtIconFont::iconByClass(const QString &name) const {
    Q_D(const QtIconFont);
    return FindIcon(d->font_class_map, name);
}

FontInfoPtr_t QtIconFont::fontInfoById(const QString &name) const {
    Q_D(const QtIconFont);
    return FindInfo(d->icon_id_map, name);
}

QChar QtIconFont::iconById(const QString &name) const {
    Q_D(const QtIconFont);
    return FindIcon(d->icon_id_map, name);
}

bool QtIconFontPrivate::openFile(QFile *file) {
    if (!file->isOpen()) {
        if (!file->open(QIODevice::ReadOnly)) {
            qWarning("[QtIconFont] Cannot open font file: %s", qUtf8Printable(file->fileName()));
            return false;
        }
    }
    if (!file->isReadable()) {
        qWarning("[QtIconFont] Cannot read font file: %s", qUtf8Printable(file->fileName()));
    }
    file->seek(0);
    return true;
}

FontInfoPtr_t QtIconFontPrivate::ReadFontInfo(const QJsonObject &obj) {
    auto icon_id = obj.value("icon_id");
    auto name = obj.value("name");
    auto font_class = obj.value("font_class");
    auto unicode = obj.value("unicode");
    auto unicode_decimal = obj.value("unicode_decimal");
    if (!icon_id.isString()) return nullptr;
    if (!name.isString()) return nullptr;
    if (!font_class.isString()) return nullptr;
    if (!unicode.isString()) return nullptr;
    if (!unicode_decimal.isDouble()) return nullptr;
    auto ptr = new QtIconFont::FontInfo_t;
    ptr->icon_id = icon_id.toString();
    ptr->name = name.toString();
    ptr->font_class = font_class.toString();
    ptr->unicode = unicode.toString();
    ptr->unicode_decimal = unicode_decimal.toInt();
    return QSharedPointer<FontInfo_t>(ptr);
}

bool QtIconFontPrivate::parseJsonData(const QByteArray &data, const QString &file_name) {
    QJsonParseError err{};
    auto doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError) {
        qWarning("[QtIconFont] Cannot parse json file: %s, error: %s",
                 qUtf8Printable(file_name), qUtf8Printable(err.errorString()));
        return false;
    }
    if (!doc.isObject()) {
        qWarning("[QtIconFont] Json file is not a object: %s", qUtf8Printable(file_name));
        return false;
    }
    auto obj = doc.object();
    auto glyphs = obj.value("glyphs").toArray();
    this->font_name = obj.value("name").toString();
    this->description = obj.value("description").toString();
    for (auto &&item : glyphs) {
        auto glyph = item.toObject();
        auto ptr = ReadFontInfo(glyph);
        if (!ptr) continue;
        this->icon_id_map.insert(ptr->icon_id, ptr);
        this->name_map.insert(ptr->name, ptr);
        this->font_class_map.insert(ptr->font_class, ptr);
    }
    return true;
}

void QtIconFontPrivate::loadFontFromFile(QFile *font, QFile *json) {
    if (!openFile(font)) return;
    if (!openFile(json)) return;
    font->seek(0);
    json->seek(0);
    auto font_data = font->readAll();
    auto json_data = json->readAll();
    font->close();
    json->close();

    if (!this->parseJsonData(json_data, json->fileName())) return;
    auto id = QFontDatabase::addApplicationFontFromData(font_data);
    QStringList families = QFontDatabase::applicationFontFamilies(id);
    if (families.empty()) {
        qWarning("[QtIconFont] Cannot load font from file: %s", qUtf8Printable(font->fileName()));
        return;
    }
    this->font_family = families.first();
}
