# QtWidgets

## 简单介绍

一些通用的Qt组件.

## 组件

- "&cross;" 表示不支持
- "&check;" 表示支持
- "-" 表示未测试, 但是理论上可以支持

| Name          | Class         | Delivered | Qt4     | Qt5     | Qt6 | QML     |  
|---------------|---------------|-----------|---------|---------|-----|---------|
| QtIconFont    | QtIconFont    | QObject   | &cross; | &check; | -   | &cross; |
| QtImageWidget | QtImageWidget | QWidget   | &cross; | &check; | -   | -       |

## 详细介绍

- ### QtIconFont

用来加载iconfont的类, iconfont可以从[https://www.iconfont.cn/](https://www.iconfont.cn/)中下载

必须用一个字体文件和一个json文件来初始化. json文件中需要包含必要的数据

json文件示例:

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

- ### QtImageWidget

图像展示组件. 可以将图像作为背景展示出来.

#### Features

- 支持通过 qss 设置 background-color
- 支持通过 properties 设置边框属性
- 支持限制图像大小
- 支持图像位置偏移
- 支持图像位置对齐
