lessThan(QT_MAJOR_VERSION, 5) {
  error("QuiteRSS requires at least Qt 5.9!")
}
isEqual(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 9) {
  error("QuiteRSS requires at least Qt 5.9!")
}

APP_REVISION = 0
exists(.git) {
  APP_REVISION = $$system(git rev-list --count head)
  !count(APP_REVISION, 1):APP_REVISION = 0
}
!build_pass:message(VCS revision: $$APP_REVISION)

TEMPLATE = app

os2|win32|mac|android {
  TARGET = QuiteRSS
} else {
  TARGET = quiterss
}

QT += qml quick
CONFIG += c++11

APP_VERSION      = 0.0.1
APP_DESCRIPTION  = "QuiteRSS - News feed reader"
APP_COMPANY      = "QuiteRSS Team"
APP_COPYRIGHT    = "Copyright (C) 2011-2017 $$APP_COMPANY"
VERSION = $${APP_VERSION}.$${APP_REVISION}

DEFINES += APP_VERSION=\\\"$$APP_VERSION\\\"
DEFINES += APP_REVISION=\\\"$$APP_REVISION\\\"

DESTDIR = $$OUT_PWD/bin
OBJECTS_DIR = $$OUT_PWD/obj
MOC_DIR = $$OUT_PWD/moc
RCC_DIR = $$OUT_PWD/rcc
UI_DIR = $$OUT_PWD/ui

HEADERS +=

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
    tools/build/appveyor-build.bat \

OTHER_FILES += \
    .appveyor.yml \
    .travis.yml \
    platforms/macosx/Info.plist \
    platforms/linux/quiterss.desktop \

isEmpty(SYSTEMQTSA) {
  include($$PWD/src/3rdparty/qtsingleapplication/qtsingleapplication.pri)
} else {
  CONFIG += qtsingleapplication
}
include(translations/translations.pri)

win32 {
  RC_ICONS = resources/icons/quiterss.ico
  QMAKE_TARGET_PRODUCT = $$TARGET
  QMAKE_TARGET_DESCRIPTION = $$APP_DESCRIPTION
  QMAKE_TARGET_COMPANY = $$APP_COMPANY
  QMAKE_TARGET_COPYRIGHT = $$APP_COPYRIGHT
}

mac {
  QMAKE_INFO_PLIST = $$PWD/platforms/macosx/Info.plist
  ICON = $$PWD/resources/icons/quiterss.icns

  bundle_target.files += $$PWD/AUTHORS
  bundle_target.files += $$PWD/LICENSE
  bundle_target.files += $$PWD/CHANGELOG
  bundle_target.files += $$PWD/README.md
  bundle_target.files += $$PWD/resources/sound
  bundle_target.path = Contents/Resources
  QMAKE_BUNDLE_DATA += bundle_target

  translations.files = $$quote($$DESTDIR/translations)
  translations.path =  Contents/Resources
  QMAKE_BUNDLE_DATA += translations

  INSTALLS += bundle_target translations
}

unix:!mac:!android {
  isEmpty(PREFIX) {
    PREFIX = /usr
  }
  DATA_DIR = $$PREFIX/share/quiterss
  DEFINES += RESOURCES_DIR='\\\"$${DATA_DIR}\\\"'

  target.path = $$quote($$PREFIX/bin)

  target1.files = $$PWD/platforms/linux/quiterss.desktop
  target1.path =  $$quote($$PREFIX/share/applications)

  target2.files = $$PWD/platforms/hicolor/48x48/quiterss.png
  target2.path =  $$quote($$PREFIX/share/pixmaps)

  icon16.files =  $$PWD/platforms/hicolor/16x16/quiterss.png
  icon32.files =  $$PWD/platforms/hicolor/32x32/quiterss.png
  icon48.files =  $$PWD/platforms/hicolor/48x48/quiterss.png
  icon64.files =  $$PWD/platforms/hicolor/64x64/quiterss.png
  icon128.files = $$PWD/platforms/hicolor/128x128/quiterss.png
  icon256.files = $$PWD/platforms/hicolor/256x256/quiterss.png
  icon16.path =  $$quote($$PREFIX/share/icons/hicolor/16x16/apps)
  icon32.path =  $$quote($$PREFIX/share/icons/hicolor/32x32/apps)
  icon48.path =  $$quote($$PREFIX/share/icons/hicolor/48x48/apps)
  icon64.path =  $$quote($$PREFIX/share/icons/hicolor/64x64/apps)
  icon128.path = $$quote($$PREFIX/share/icons/hicolor/128x128/apps)
  icon256.path = $$quote($$PREFIX/share/icons/hicolor/256x256/apps)

  translations.files = $$quote($$DESTDIR/translations)
  translations.path =  $$quote($$DATA_DIR)
  translations.CONFIG += no_check_exist

  sound.files = $$PWD/resources/sound
  sound.path = $$quote($$DATA_DIR)

  INSTALLS += target target1 target2
  INSTALLS += icon16 icon32 icon48 icon64 icon128 icon256
  INSTALLS += translations sound
}

android {
  DISTFILES += \
      android/AndroidManifest.xml \
      android/res/values/libs.xml \
      android/res/values/apptheme.xml \
      android/res/drawable/splash.xml

  ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =
