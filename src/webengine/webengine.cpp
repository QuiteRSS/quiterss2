#include "webengine.h"

#ifndef DISABLE_BROWSER
#ifdef MOBILE
#include <QtWebView>
#else
#include <QtWebEngine>
#endif
#endif

WebEngine::WebEngine(QObject *parent)
  : QObject(parent)
{

}

void WebEngine::initialize()
{
#ifndef DISABLE_BROWSER
#ifdef MOBILE
  QtWebView::initialize();
#else
  QtWebEngine::initialize();
#endif
#endif
}
