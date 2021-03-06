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

#include <QUrl>

#include <KPluginFactory>
#include <KConfigGroup>

#include <interfaces/icore.h>
#include <interfaces/iruncontroller.h>
#include <interfaces/ilaunchconfiguration.h>

#include <debug.h>
#include "browserappconfig.h"

QString ExecuteBrowserPlugin::_browserAppConfigTypeId = "Browser Application";
QString ExecuteBrowserPlugin::serverEntry = "Server";
QString ExecuteBrowserPlugin::pathEntry = "Path";
QString ExecuteBrowserPlugin::portEntry = "Port";
QString ExecuteBrowserPlugin::argumentsEntry = "Arguments";
QString ExecuteBrowserPlugin::browserEntry = "Browser";

using namespace KDevelop;

//KPluginFactory stuff to load the plugin dynamically at runtime
K_PLUGIN_FACTORY_WITH_JSON(KDevExecuteFactory, "kdevexecutebrowser.json", registerPlugin<ExecuteBrowserPlugin>();)

ExecuteBrowserPlugin::ExecuteBrowserPlugin(QObject *parent, const QVariantList&)
: KDevelop::IPlugin("kdevexecutebrowser", parent)
{
    BrowserAppConfigType* t = new BrowserAppConfigType();
    t->addLauncher( new BrowserAppLauncher() );
    qCDebug(KDEV_EXECUTEBROWSER) << "adding script launch config";
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
        err_ = i18n("Empty server in launch configuration");
        qCWarning(KDEV_EXECUTEBROWSER) << "Launch Configuration:" << cfg->name() << err_;
        return url;
    }

    QString path(grp.readEntry( ExecuteBrowserPlugin::pathEntry, "" ));
    if( !host.endsWith("/") && !path.isEmpty() && !path.startsWith("/") ) {
        path.prepend("/");
    }
    if( !host.contains(QStringLiteral("://")) )
    {
        host.prepend(QStringLiteral("http://"));
    }

    url.setUrl(host + path);
    url.setPort(grp.readEntry( ExecuteBrowserPlugin::portEntry, 80 ));
    {
        QString q = grp.readEntry( ExecuteBrowserPlugin::argumentsEntry, "" );
        if (!q.isEmpty()) {
            url.setQuery(q);
        }
    }

    if( url.toString().isEmpty() ) {
        err_ = i18n("Invalid launch configuration");
        qCWarning(KDEV_EXECUTEBROWSER) << "Launch Configuration:" << cfg->name() << err_;
        return url;
    }
    qCDebug(KDEV_EXECUTEBROWSER) << "Url:" << url.toString();
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
