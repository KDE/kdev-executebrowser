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

#ifndef BROWSERAPPJOB_H
#define BROWSERAPPJOB_H

#include <QtCore/QProcess>
#include <KUrl>
#include <outputview/outputjob.h>

namespace KDevelop
{
class ILaunchConfiguration;
class ProcessLineMaker;
class OutputModel;
}

class BrowserAppJob : public KDevelop::OutputJob
{
Q_OBJECT
public:
    BrowserAppJob( QObject* parent, KDevelop::ILaunchConfiguration* cfg );
    virtual void start();

private:
    KUrl m_url;
    QString m_browser;
};

#endif
