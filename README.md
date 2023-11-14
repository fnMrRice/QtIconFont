# QtWidgets

## Introduction

Some common qt widgets.

## Widgets

- "&cross;" means not supported
- "&check;" means supported
- "-" means not tested, but should be supported

| Name          | Class         | Delivered | Qt4     | Qt5     | Qt6 | QML     |  
|---------------|---------------|-----------|---------|---------|-----|---------|
| QtIconFont    | QtIconFont    | QObject   | &cross; | &check; | -   | &cross; |
| QtImageWidget | QtImageWidget | QWidget   | &cross; | &check; | -   | -       |

## Details

- ### QtIconFont

A class to load iconfont, which is download from [https://www.iconfont.cn/](https://www.iconfont.cn/)

Must init with a font file and a json file. The json file should contain necessary data.

Here is an example of the json file:

```json 
{
  "id": "123456",
  "name": "iconfont",
  "font_family": "iconfont",
  "description": "description",
  "glyphs": [
    {
      "icon_id": "123456",
      "name": "whatever",
      "font_class": "font_class",
      "unicode": "e700",
      "unicode_decimal": 59136
    }
  ]
}
```

example file at `tests/iconfont.cpp`

- ### QtImageWidget

An image widget. It can display an image as background.

#### Features

- Supports set background-color via qss
- Supports set border properties via properties
- Supports limit image minimum and maximum size
- Supports background image position offset
- Supports background image alignment

example file at `tests/imagewidget.cpp`
