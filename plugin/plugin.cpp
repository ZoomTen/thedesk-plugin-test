// our local defines
#include "plugin.h"

// theDesk stuff
#include <statemanager.h>
#include <statuscentermanager.h>
#include <localemanager.h>

// qt stuff
#include <QApplication>
#include <QDebug>
#include <QDir>

// the-libs stuff
#include <tsettings.h>

// app functionality
#include <tmessagebox.h>

// hacks
#define XSTR(x) #x
#define STR(x) XSTR(x)

// directories
#define LOCAL_TRANSLATIONS "/../plugins/" STR(PLUGIN_NAME) "/translations"
#define SYSTEM_TRANSLATIONS "/usr/share/thedesk/" STR(PLUGIN_NAME) "/translations"
#define LOCAL_DEFAULTS "/../plugins/" STR(PLUGIN_NAME) "/defaults.conf"
#define SYSTEM_DEFAULTS "/etc/theSuite/theDesk/" STR(PLUGIN_NAME) "/defaults.conf"

struct PluginPrivate{
    // class variables here
    int translationSet;
};

Plugin::Plugin()
{
    // setup class-wide private variables
    d = new PluginPrivate();
}

Plugin::~Plugin()
{
    // release private variables
    delete d;
}

void Plugin::activate() {
    // register translation stuff
    d->translationSet = StateManager::localeManager()->addTranslationSet({
        QDir::cleanPath(qApp->applicationDirPath() + LOCAL_TRANSLATIONS),
        SYSTEM_TRANSLATIONS
    });

    // register defaults
    tSettings::registerDefaults(QDir::cleanPath(qApp->applicationDirPath() + LOCAL_DEFAULTS));
    tSettings::registerDefaults(SYSTEM_DEFAULTS);
}

void Plugin::deactivate() {
    // emit message saying we've deactivated the plugin
    // this is a custom plugin function, thus not needed (in fact, obtrusive)
    tMessageBox msgbox;
    msgbox.setWindowTitle(tr("Plugin deactivated"));
    msgbox.setText(tr("%1 has been deactivated.").arg(STR(PLUGIN_NAME)));
    msgbox.exec();

    // remove translation
    StateManager::localeManager()->removeTranslationSet(d->translationSet);
}
