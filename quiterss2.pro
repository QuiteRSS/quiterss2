lessThan(QT_MAJOR_VERSION, 5) {
  error("QuiteRSS requires at least Qt 5.9!")
}
isEqual(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 9) {
  error("QuiteRSS requires at least Qt 5.9!")
}

TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += \
    src/main.cpp \

INCLUDEPATH += \
    $$PWD/src \

RESOURCES += \
    resources/qml/qml.qrc

DISTFILES += \
    CHANGELOG \
    LICENSE \
    AUTHORS \
    INSTALL \
    README.md \
    .appveyor.yml \
    .travis.yml \

android {
  DISTFILES += \
      android/AndroidManifest.xml \
      android/gradle/wrapper/gradle-wrapper.jar \
      android/gradlew \
      android/res/values/libs.xml \
      android/build.gradle \
      android/gradle/wrapper/gradle-wrapper.properties \
      android/gradlew.bat

  ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

DESTDIR = $$OUT_PWD/bin
OBJECTS_DIR = $$OUT_PWD/obj
MOC_DIR = $$OUT_PWD/moc
RCC_DIR = $$OUT_PWD/rcc
UI_DIR = $$OUT_PWD/ui

isEmpty(SYSTEMQTSA) {
  include($$PWD/src/3rdparty/qtsingleapplication/qtsingleapplication.pri)
} else {
  CONFIG += qtsingleapplication
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
