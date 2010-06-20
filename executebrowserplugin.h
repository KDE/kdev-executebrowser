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

#ifndef EXECUTEBROWSERPLUGIN_H
#define EXECUTEBROWSERPLUGIN_H

#include <interfaces/iplugin.h>
#include <QtCore/QVariant>
#include <QtCore/QProcess>
#include "iexecutebrowserplugin.h"

class KUrl;
class KJob;

class ExecuteBrowserPlugin : public KDevelop::IPlugin, public IExecuteBrowserPlugin
{
    Q_OBJECT
    Q_INTERFACES( IExecuteBrowserPlugin )

  public:
    ExecuteBrowserPlugin(QObject *parent, const QVariantList & = QVariantList() );
    virtual ~ExecuteBrowserPlugin();

    static QString _browserAppConfigTypeId;
    static QString serverEntry;
    static QString pathEntry;
    static QString argumentsEntry;
    static QString browserEntry;
    
    virtual void unload();
    
    KUrl url( KDevelop::ILaunchConfiguration*, QString& err ) const;
    virtual QString browser(KDevelop::ILaunchConfiguration* ) const;
    QString browserAppConfigTypeId() const;
};

#endif // EXECUTEBROWSERPLUGIN_H

// kate: space-indent on; indent-width 2; tab-width 4; replace-tabs on; auto-insert-doxygen on
