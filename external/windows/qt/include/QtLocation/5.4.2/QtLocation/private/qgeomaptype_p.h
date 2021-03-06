/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGEOMAPTYPE_H
#define QGEOMAPTYPE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QString>
#include <QtCore/QSharedDataPointer>
#include <QtLocation/qlocationglobal.h>

QT_BEGIN_NAMESPACE

class QGeoMapTypePrivate;

class Q_LOCATION_EXPORT QGeoMapType
{
public:
    enum MapStyle {
        NoMap = 0,
        StreetMap,
        SatelliteMapDay,
        SatelliteMapNight,
        TerrainMap,
        HybridMap,
        TransitMap,
        GrayStreetMap,
        PedestrianMap,
        CarNavigationMap,
        CustomMap = 100
    };

    QGeoMapType();
    QGeoMapType(const QGeoMapType &other);
    QGeoMapType(MapStyle style, const QString &name, const QString &description, bool mobile,
                bool night, int mapId);
    ~QGeoMapType();

    QGeoMapType &operator = (const QGeoMapType &other);

    bool operator == (const QGeoMapType &other) const;
    bool operator != (const QGeoMapType &other) const;

    MapStyle style() const;
    QString name() const;
    QString description() const;
    bool mobile() const;
    bool night() const;
    int mapId() const;

private:
    QSharedDataPointer<QGeoMapTypePrivate> d_ptr;
};

QT_END_NAMESPACE

#endif // QGEOMAPTYPE_H
