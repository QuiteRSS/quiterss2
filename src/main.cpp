#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  QTranslator translator_;
  translator_.load(QString("translations/quiterss_%1").arg("ru"));
  app.installTranslator(&translator_);

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/mainwindow.qml")));
  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  return app.exec();
}
