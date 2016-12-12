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
#include <QUrl>
#include <QString>
#include <QDebug>

#include <KPluginFactory>
#include <kpluginloader.h>
#include <kjob.h>
#include <kparts/mainwindow.h>
#include <kmessagebox.h>
#include <kaboutdata.h>
#include <KConfigGroup>

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

#include "version.h"

QString ExecuteBrowserPlugin::_browserAppConfigTypeId = "Browser Application";
QString ExecuteBrowserPlugin::serverEntry = "Server";
QString ExecuteBrowserPlugin::pathEntry = "Path";
QString ExecuteBrowserPlugin::argumentsEntry = "Arguments";
QString ExecuteBrowserPlugin::browserEntry = "Browser";

using namespace KDevelop;

//KPluginFactory stuff to load the plugin dynamically at runtime
K_PLUGIN_FACTORY_WITH_JSON(KDevExecuteFactory, "kdevexecutebrowser.json", registerPlugin<ExecuteBrowserPlugin>();)

ExecuteBrowserPlugin::ExecuteBrowserPlugin(QObject *parent, const QVariantList&)
: KDevelop::IPlugin("kdevexecutebrowser", parent)
{
    KDEV_USE_EXTENSION_INTERFACE( IExecuteBrowserPlugin )
    BrowserAppConfigType* t = new BrowserAppConfigType();
    t->addLauncher( new BrowserAppLauncher() );
    qDebug() << "adding script launch config";
    core()->runController()->addConfigurationType( t );
}

ExecuteBrowserPlugin::~ExecuteBrowserPlugin()
{
}


void ExecuteBrowserPlugin::unload()
{
}

QUrl ExecuteBrowserPlugin::url( KDevelop::ILaunchConfiguration* cfg, QString& err_ ) const
{
    QUrl url;

    if( !cfg )
    {
        return url;
    }
    KConfigGroup grp = cfg->config();

    QString host = grp.readEntry( ExecuteBrowserPlugin::serverEntry, "" );
    if( host.isEmpty() )
    {
        err_ = i18n("No valid server specified");
        qWarning() << "Launch Configuration:" << cfg->name() << "no valid server specified";
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
