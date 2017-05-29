/*  This file is part of KDevelop
    Copyright 2009 Andreas Pakulat <apaku@gmx.de>
    Copyright 2009 Niko Sams <niko.sams@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#include "browserappconfig.h"

#include <QIcon>

#include <KConfigGroup>
#include <KOpenWithDialog>

#include <interfaces/icore.h>
#include <interfaces/ilaunchconfiguration.h>
#include <interfaces/iruncontroller.h>

#include <debug.h>
#include "browserappjob.h"
#include "executebrowserplugin.h"

QIcon BrowserAppConfigPage::icon() const
{
    return QIcon::fromTheme("system-run");
}

void BrowserAppConfigPage::loadFromConfiguration(const KConfigGroup& cfg, KDevelop::IProject* project )
{
    Q_UNUSED(project);

    bool b = blockSignals( true );
    server->setText( cfg.readEntry( ExecuteBrowserPlugin::serverEntry, "" ) );
    path->setText( cfg.readEntry( ExecuteBrowserPlugin::pathEntry, "" ) );
    arguments->setText( cfg.readEntry( ExecuteBrowserPlugin::argumentsEntry, "" ) );
    browser->setText( cfg.readEntry( ExecuteBrowserPlugin::browserEntry, "" ) );
    blockSignals( b );
}

BrowserAppConfigPage::BrowserAppConfigPage( QWidget* parent )
    : LaunchConfigurationPage( parent )
{
    setupUi(this);

    selectBrowserButton->setIcon(QIcon::fromTheme("system-run"));

    //connect signals to changed signal
    connect(server, &KLineEdit::textEdited, this, &BrowserAppConfigPage::changed); 
    connect(path, &KLineEdit::textEdited, this, &BrowserAppConfigPage::changed); 
    connect(arguments, &KLineEdit::textEdited, this, &BrowserAppConfigPage::changed); 
    connect(browser, &KLineEdit::textEdited, this, &BrowserAppConfigPage::changed); 
    connect(selectBrowserButton, &QPushButton::pressed, this, &BrowserAppConfigPage::selectDialog); 
}

void BrowserAppConfigPage::saveToConfiguration( KConfigGroup cfg, KDevelop::IProject* project ) const
{
    Q_UNUSED( project );
    cfg.writeEntry( ExecuteBrowserPlugin::serverEntry, server->text() );
    cfg.writeEntry( ExecuteBrowserPlugin::pathEntry, path->text() );
    cfg.writeEntry( ExecuteBrowserPlugin::argumentsEntry, arguments->text() );
    cfg.writeEntry( ExecuteBrowserPlugin::browserEntry, browser->text() );
}

QString BrowserAppConfigPage::title() const
{
    return i18n("Configure Browser Application");
}

void BrowserAppConfigPage::selectDialog()
{
    KOpenWithDialog *dialog = new KOpenWithDialog();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->hideNoCloseOnExit();
    dialog->hideRunInTerminal();
    if(dialog->exec()) {
        browser->setText(dialog->text().replace(" %u", "", Qt::CaseSensitivity::CaseInsensitive));
        emit changed();
    }
}

QList< KDevelop::LaunchConfigurationPageFactory* > BrowserAppLauncher::configPages() const
{
    return QList<KDevelop::LaunchConfigurationPageFactory*>();
}

QString BrowserAppLauncher::description() const
{
    return "Executes Browser Applications";
}

QString BrowserAppLauncher::id()
{
    return "browserAppLauncher";
}

QString BrowserAppLauncher::name() const
{
    return i18n("Browser Application");
}

BrowserAppLauncher::BrowserAppLauncher()
{
}

KJob* BrowserAppLauncher::start(const QString& launchMode, KDevelop::ILaunchConfiguration* cfg)
{
    Q_ASSERT(cfg);
    if( !cfg )
    {
        return nullptr;
    }
    if( launchMode == "execute" )
    {
        return new BrowserAppJob( KDevelop::ICore::self()->runController(), cfg );
        
    }
    qCWarning(KDEV_EXECUTEBROWSER) << "Unknown launch mode " << launchMode << "for config:" << cfg->name();
    return nullptr;
}

QStringList BrowserAppLauncher::supportedModes() const
{
    return QStringList() << "execute";
}

KDevelop::LaunchConfigurationPage* BrowserAppPageFactory::createWidget(QWidget* parent)
{
    return new BrowserAppConfigPage( parent );
}

BrowserAppPageFactory::BrowserAppPageFactory()
{
}

BrowserAppConfigType::BrowserAppConfigType()
{
    factoryList.append( new BrowserAppPageFactory() );
}

QString BrowserAppConfigType::name() const
{
    return i18n("Browser Application");
}


QList<KDevelop::LaunchConfigurationPageFactory*> BrowserAppConfigType::configPages() const
{
    return factoryList;
}

QString BrowserAppConfigType::id() const
{
    return ExecuteBrowserPlugin::_browserAppConfigTypeId;
}

QIcon BrowserAppConfigType::icon() const
{
    return QIcon::fromTheme("system-run");
}

bool BrowserAppConfigType::canLaunch(const QUrl& /*file*/) const
{
    return false;
}

bool BrowserAppConfigType::canLaunch(KDevelop::ProjectBaseItem* /*item*/) const
{
    return false;
}

void BrowserAppConfigType::configureLaunchFromItem(KConfigGroup /*config*/,
                                                   KDevelop::ProjectBaseItem* /*item*/) const
{

}

void BrowserAppConfigType::configureLaunchFromCmdLineArguments(KConfigGroup /*config*/,
                                                               const QStringList &/*args*/) const
{
}
