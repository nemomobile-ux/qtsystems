/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSystems module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdeviceinfo.h"

#if defined(QT_SIMULATOR)
#  include "qsysteminfo_simulator_p.h"
#elif defined(Q_OS_LINUX)
#  include "qdeviceinfo_linux_p.h"
#elif defined(Q_OS_WIN)
#  include "qdeviceinfo_win_p.h"
#elif defined(Q_OS_MAC)
#  include "qdeviceinfo_mac_p.h"
#else
QT_BEGIN_NAMESPACE
class QDeviceInfoPrivate
{
public:
    QDeviceInfoPrivate(QDeviceInfo *) {}

    bool hasFeature(QDeviceInfo::Feature) { return false; }
    int imeiCount() { return -1; }
    QDeviceInfo::LockTypeFlags activatedLocks() { return QDeviceInfo::NoLock; }
    QDeviceInfo::LockTypeFlags enabledLocks() { return QDeviceInfo::NoLock; }
    QDeviceInfo::ThermalState thermalState() { return QDeviceInfo::UnknownThermal; }
    QString imei(int) { return QString(); }
    QString manufacturer() { return QString(); }
    QString model() { return QString(); }
    QString productName() { return QString(); }
    QString uniqueDeviceID() { return QString(); }
    QString version(QDeviceInfo::Version) { return QString(); }
};
QT_END_NAMESPACE
#endif

#include <QtCore/qmetaobject.h>

QT_BEGIN_NAMESPACE

/*!
    \class QDeviceInfo
    \inmodule QtSystemInfo
    \brief The QDeviceInfo class provides various information of the system.
    \ingroup systeminfo
*/

/*!
    \enum QDeviceInfo::Feature
    This enum describes the features of the device.

    \value BluetoothFeature      Bluetooth feature.
    \value CameraFeature         Camera feature.
    \value FmRadioFeature        Frequency modulation (FM) radio feature.
    \value FmTransmitterFeature  Frequency modulation (FM) radio transmitter feature.
    \value InfraredFeature       Infrared communication feature.
    \value LedFeature            Light-emitting diode (LED) feature.
    \value MemoryCardFeature     Memory card feature.
    \value UsbFeature            Universal system bus (USB) feature.
    \value VibrationFeature      Vibration feature.
    \value WlanFeature           Wireless local area network (WLAN) feature.
    \value SimFeature            Subscriber identity module (SIM) feature.
    \value PositioningFeature    Positioning feature, e.g. Global Positioning System (GPS).
    \value VideoOutFeature       Video out feature.
    \value HapticsFeature        Haptics feature, the platform can provide audio and/or visual and/or vibration feedback.
    \value NfcFeature            Near Field Communication (NFC) feature.
*/

/*!
    \enum QDeviceInfo::LockType
    This enum describes lock type on the device.

    \value NoLock               No lock.
    \value PinLock              Device can be locked by PIN code or password.
    \value TouchOrKeyboardLock  Device can be locked by touch or keyboard.
    \value UnknownLock          lock types requested but no result received yet.
*/

/*!
    \enum QDeviceInfo::ThermalState
    This enum describes the thermal state:

    \value UnknownThermal   The thermal state is unknown.
    \value NormalThermal    The thermal state is normal.
    \value WarningThermal   The thermal state is warning.
    \value AlertThermal     The thermal state is alert.
    \value ErrorThermal     The thermal state is error.
*/

/*!
    \enum QDeviceInfo::Version
    This enum describes the version component.

    \value Os                    Operating system version. For Linux, it returns the version of the
                                 distribution if any.
    \value Firmware              Version of (flashable) system as a whole. For Linux, it's the version
                                 of the kernel.
*/

/*!
    \fn void QDeviceInfo::activatedLocksChanged(QDeviceInfo::LockTypeFlags types)

    This signal is emitted when the activated locks have changed to \a types.
*/

/*!
    \fn void QDeviceInfo::enabledLocksChanged(QDeviceInfo::LockTypeFlags types)

    This signal is emitted when the enabled locks have changed to \a types.
*/

/*!
    \fn void QDeviceInfo::thermalStateChanged(QDeviceInfo::ThermalState state)

    This signal is emitted when the thermal state has changed to \a state.
*/

/*!
    Constructs a QDeviceInfo object with the given \a parent.
*/
QDeviceInfo::QDeviceInfo(QObject *parent)
    : QObject(parent)
#if !defined(QT_SIMULATOR)
    , d_ptr(new QDeviceInfoPrivate(this))
#else
    , d_ptr(new QDeviceInfoSimulator(this))
#endif // QT_SIMULATOR
{
}

/*!
    Destroys the object
*/
QDeviceInfo::~QDeviceInfo()
{
#if !defined(Q_OS_LINUX)
    delete d_ptr;
#endif
}

/*!
    \property QDeviceInfo::activatedLocks
    \brief The activated lock types.

    The current activated lock types of the device. It suggests that these lock types are also
    enabled.
*/
QDeviceInfo::LockTypeFlags QDeviceInfo::activatedLocks() const
{
    return d_ptr->activatedLocks();
}

/*!
    \property QDeviceInfo::enabledLocks
    \brief The enabled lock types.

    The current lock types that are enabled on the device. Note that it doesn't mean the device is
    currently locked.
*/
QDeviceInfo::LockTypeFlags QDeviceInfo::enabledLocks() const
{
    return d_ptr->enabledLocks();
}

/*!
    \property QDeviceInfo::thermalState
    \brief The thermal state.

    The current thermal state of the device. If there are more than one thermal zone devices available,
    the state of the most critical one is reported.
*/
QDeviceInfo::ThermalState QDeviceInfo::thermalState() const
{
    return d_ptr->thermalState();
}

/*!
    Returns true if the \a feature is supported, otherwise false.
*/
bool QDeviceInfo::hasFeature(QDeviceInfo::Feature feature) const
{
    return d_ptr->hasFeature(feature);
}

/*!
    Returns the count of available International Mobile Equipment Identity (IMEI) of the device. In
    case of error, or the information is not available, -1 is returned.
*/
int QDeviceInfo::imeiCount() const
{
    return d_ptr->imeiCount();
}

/*!
    Returns the International Mobile Equipment Identity (IMEI) of the given \a interface on the device.
    In case of error, or the information is not available, an empty string is returned.
*/
QString QDeviceInfo::imei(int interface) const
{
    return d_ptr->imei(interface);
}

/*!
    Returns the name of the manufacturer of this device, or the name of the vendor of the motherboard
    as a fallback. In case of error, or the information is not available, an empty string is returned.
*/
QString QDeviceInfo::manufacturer() const
{
    return d_ptr->manufacturer();
}

/*!
    Returns the model information of the device, e.g. N8, or the CPU architect as a fallback. In case
    of error, or the information is not available, an empty string is returned.
*/
QString QDeviceInfo::model() const
{
    return d_ptr->model();
}

/*!
    Returns the internal product name of the device, e.g. RM-774. In case of error, or the information
    is not available, an empty string is returned.

    For Linux, it returns the codename of the distribution if any.
*/
QString QDeviceInfo::productName() const
{
    return d_ptr->productName();
}

/*!
    Returns a unique identifier for the device, or an empty string if on error or not available.
*/
QString QDeviceInfo::uniqueDeviceID() const
{
    return d_ptr->uniqueDeviceID();
}

/*!
    Returns the version of \a type. In case of error, or the version is unknown, an empty string
    is returned.
*/
QString QDeviceInfo::version(QDeviceInfo::Version type) const
{
    return d_ptr->version(type);
}

extern QMetaMethod proxyToSourceSignal(const QMetaMethod &, QObject *);

/*!
    \internal
*/
void QDeviceInfo::connectNotify(const QMetaMethod &signal)
{
#if defined(Q_OS_LINUX) || defined(QT_SIMULATOR)
    QMetaMethod sourceSignal = proxyToSourceSignal(signal, d_ptr);
    connect(d_ptr, sourceSignal, this, signal, Qt::UniqueConnection);
#else
    Q_UNUSED(signal)
#endif
}

/*!
    \internal
*/
void QDeviceInfo::disconnectNotify(const QMetaMethod &signal)
{
#if defined(Q_OS_LINUX) || defined(QT_SIMULATOR)
    // We can only disconnect with the private implementation, when there is no receivers for the signal.
    if (isSignalConnected(signal))
        return;

    QMetaMethod sourceSignal = proxyToSourceSignal(signal, d_ptr);
    disconnect(d_ptr, sourceSignal, this, signal);
#else
    Q_UNUSED(signal)
#endif
}

QT_END_NAMESPACE
