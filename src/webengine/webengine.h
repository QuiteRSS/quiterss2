#ifndef WEBENGINE_H
#define WEBENGINE_H

#include <QObject>

class WebEngine : public QObject
{
  Q_OBJECT
public:
  explicit WebEngine(QObject *parent = nullptr);

  static void initialize();

};

#endif // WEBENGINE_H
