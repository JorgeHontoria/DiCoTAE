// solanogadea (Miguel Solano Gadea) @solanogadea
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QTranslator>
#include <QQmlContext>

#include "applicationui.hpp"
#include "clipboardproxy.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qputenv("QT_QUICK_CONTROLS_STYLE", "material");

    QGuiApplication app(argc, argv);

    QTranslator translator;
    if (translator.load(QLocale(), QLatin1String("c2gDiCoTAE_x"), QLatin1String("_"), QLatin1String(":/translations"))) {
        qDebug() << "Load translator " << QLocale::system().name() << ". Check content of translations.qrc";
        qDebug() << QLocale();
        app.installTranslator(&translator);
    } else {
        qDebug() << "cannot load translator " << QLocale::system().name() << " check content of translations.qrc";
    }

    ApplicationUI appui;

    QQmlApplicationEngine engine;

    // from QML we have access to ApplicationUI as myApp
    QQmlContext* context = engine.rootContext();
    context->setContextProperty("myApp", &appui);
    appui.addContextProperty(context);

    // Register new types
    qmlRegisterType<ClipboardProxy>("Clipboard", 1, 0, "Clipboard");
    //engine.rootContext()->setContextProperty("clipboard", new QClipboardProxy(QGuiApplication::clipboard()));

    // lifecycle management
    QObject::connect(&app, SIGNAL(aboutToQuit()), &appui, SLOT(onAboutToQuit()));
    QObject::connect(&app, SIGNAL(applicationStateChanged(Qt::ApplicationState)), &appui, SLOT(onApplicationStateChanged(Qt::ApplicationState)));

    // Start QML
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    return app.exec();
}
