/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2015  Malte Veerman maldela@halloarsch.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "pwmfanmodel.h"
#include "pwmfan.h"


namespace Fancontrol
{

PwmFanModel::PwmFanModel(QObject *parent) : QStringListModel(parent)
{
}

void PwmFanModel::setPwmFans(const QList<PwmFan *> &fans)
{
    m_fans = fans;
    emit fansChanged();

    QStringList list;

    foreach (PwmFan *const fan, fans)
    {
        connect(fan, SIGNAL(nameChanged()), this, SLOT(updateFan()));
        list << fan->name() + "  (" + fan->path() + ")";
    }

    setStringList(list);
}

void PwmFanModel::addPwmFans(const QList<PwmFan *> &fans)
{
    if (!fans.isEmpty())
    {
        m_fans += fans;
        emit fansChanged();

        int oldSize = rowCount();

        insertRows(oldSize, fans.size());

        foreach (PwmFan *const fan, fans)
        {
            connect(fan, SIGNAL(nameChanged()), this, SLOT(updateFan()));
            updateFan(fan);
        }
    }
}

void PwmFanModel::updateFan(PwmFan *fan)
{
    if (!fan)
        return;

    int i = m_fans.indexOf(fan);
    if (i == -1)
        return;

    QString string = fan->name() + "  (" + fan->path() + ")";
    setData(index(i, 0), string);
    emit dataChanged(index(i, 0), index(i, 0));
}

void PwmFanModel::updateFan()
{
    PwmFan *fan = qobject_cast<PwmFan *>(sender());

    if (!fan)
        return;

    int i = m_fans.indexOf(fan);
    if (i == -1)
        return;

    QString string = fan->name() + "  (" + fan->path() + ")";
    setData(index(i, 0), string);
    emit dataChanged(index(i, 0), index(i, 0));
}

QList<QObject *> PwmFanModel::fans() const
{
    QList<QObject *> list;
    foreach(PwmFan *const fan, m_fans)
        list << qobject_cast<QObject *>(fan);
    return list;
}

}