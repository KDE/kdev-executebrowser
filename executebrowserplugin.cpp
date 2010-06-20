/*
  * This file is part of KDevelop
 *
 * Copyright 2007 Hamish Rodda <rodda@kde.org>
 * Copyright 2009 Niko Sams <niko.sams@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "executebrowserplugin.h"

#include <QApplication>

#include <klocale.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <kdebug.h>
#include <kjob.h>
#include <kparts/mainwindow.h>
#include <kmessagebox.h>
#include <kaboutdata.h>

#include <interfaces/icore.h>
#include <interfaces/iruncontroller.h>
#include <interfaces/ilaunchconfiguration.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/iuicontroller.h>
#include <util/environmentgrouplist.h>

#include "browserappconfig.h"
#include <project/projectmodel.h>
#include <project/builderjob.h>
#include <kshell.h>
#include <util/kdevstringhandler.h>

QString ExecuteBrowserPlugin::_browserAppConfigTypeId = "Browser Application";
QString ExecuteBrowserPlugin::serverEntry = "Server";
QString ExecuteBrowserPlugin::pathEntry = "Path";
QString ExecuteBrowserPlugin::argumentsEntry = "Arguments";
QString ExecuteBrowserPlugin::browserEntry = "Browser";

using namespace KDevelop;

K_PLUGIN_FACTORY(KDevExecuteFactory, registerPlugin<ExecuteBrowserPlugin>(); )
K_EXPORT_PLUGIN(KDevExecuteFactory(KAboutData("kdevexecutebrowser", "kdevexecutebrowser", ki18n("Execute browser support"), "1.0 Beta 1", ki18n("Allows running of browsers"), KAboutData::License_GPL)
    .addAuthor(ki18n("Niko Sams"), ki18n("Author"), "niko.sams@gmail.com", "http://nikosams.blogspot.com")
))

ExecuteBrowserPlugin::ExecuteBrowserPlugin(QObject *parent, const QVariantList&)
    : KDevelop::IPlugin(KDevExecuteFactory::componentData(), parent)
{
    KDEV_USE_EXTENSION_INTERFACE( IExecuteBrowserPlugin )
    BrowserAppConfigType* t = new BrowserAppConfigType();
    t->addLauncher( new BrowserAppLauncher() );
    kDebug() << "adding script launch config";
    core()->runController()->addConfigurationType( t );
}

ExecuteBrowserPlugin::~ExecuteBrowserPlugin()
{
}


void ExecuteBrowserPlugin::unload()
{
}

KUrl ExecuteBrowserPlugin::url( KDevelop::ILaunchConfiguration* cfg, QString& err_ ) const
{
    KUrl url;

    if( !cfg )
    {
        return url;
    }
    KConfigGroup grp = cfg->config();

    QString host = grp.readEntry( ExecuteBrowserPlugin::serverEntry, "" );
    if( host.isEmpty() )
    {
        err_ = i18n("No valid server specified");
        kWarning() << "Launch Configuration:" << cfg->name() << "no valid server specified";
        return url;
    }
    url.setScheme("http");
    url.setHost(host);
    url.setPath(grp.readEntry( ExecuteBrowserPlugin::pathEntry, "" ));
    {
        QString q = grp.readEntry( ExecuteBrowserPlugin::argumentsEntry, "" );
        if (!q.isEmpty()) {
            url.setQuery(q);
        }
    }
    return url;
}

QString ExecuteBrowserPlugin::browser( ILaunchConfiguration* cfg ) const
{
    return cfg->config().readEntry( ExecuteBrowserPlugin::browserEntry, "" );
}

QString ExecuteBrowserPlugin::browserAppConfigTypeId() const
{
    return _browserAppConfigTypeId;
}


#include "executebrowserplugin.moc"
