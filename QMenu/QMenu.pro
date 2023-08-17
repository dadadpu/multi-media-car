# 添加 Qt 核心和图形用户界面模块
QT       += core gui

# 如果 Qt 版本大于 4，则添加 widgets 模块
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 配置使用 C++11 标准
CONFIG += c++11

# 如果你的代码使用了已弃用的 API，取消下面一行的注释可以让编译过程中产生错误
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # 禁用所有在 Qt 6.0.0 之前已弃用的 API

# 添加源文件
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mypushbutton.cpp

# 添加头文件
HEADERS += \
    mainwindow.h \
    mypushbutton.h

# 添加窗口设计文件
FORMS += \
    mainwindow.ui

# 部署的默认规则
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 添加资源文件
RESOURCES += \
    pic.qrc

# 添加其他文件
DISTFILES += \
    ../../pic/video.jpeg
