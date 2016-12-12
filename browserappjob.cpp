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

#include "browserappjob.h"

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

#include <klocale.h>
#include <kmessagebox.h>
#include <kconfiggroup.h>
#include <kprocess.h>

#include <interfaces/ilaunchconfiguration.h>
#include <outputview/outputmodel.h>
#include <util/processlinemaker.h>
#include <util/environmentgrouplist.h>

#include <kshell.h>
#include <interfaces/icore.h>
#include <interfaces/iuicontroller.h>
#include <interfaces/iplugincontroller.h>
#include <project/projectmodel.h>

#include "iexecutebrowserplugin.h"

BrowserAppJob::BrowserAppJob(QObject* parent, KDevelop::ILaunchConfiguration* cfg)
    : KDevelop::OutputJob( parent )
{
    qDebug() << "creating browser app job";
    setCapabilities(NoCapabilities);

    IExecuteBrowserPlugin* iface = KDevelop::ICore::self()->pluginController()->pluginForExtension("org.kdevelop.IExecuteBrowserPlugin")->extension<IExecuteBrowserPlugin>();
    Q_ASSERT(iface);

    QString err;
    m_url = iface->url(cfg, err);
    if (!err.isEmpty()) {
        m_url.clear();
        setError( -1 );
        setErrorText( err );
        return;
    }
    m_browser = iface->browser(cfg);

    setTitle(cfg->name());
}


void BrowserAppJob::start()
{
    qDebug() << "launching?" << m_url;
    if (m_browser.isEmpty()) {
		if (!QDesktopServices::openUrl(m_url)) {
            qWarning() << "openUrl failed, something went wrong when creating the job";
        }
    } else {
	KProcess proc(this);
	proc << m_browser << m_url.url();

	proc.startDetached();
    }
    emitResult();
}


#include "browserappjob.moc"
