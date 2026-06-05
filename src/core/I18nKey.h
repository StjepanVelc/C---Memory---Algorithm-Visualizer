#pragma once

#include <QString>
#include <QStringList>

namespace i18nkey
{
    inline QString make(const QString &key, const QStringList &args = {})
    {
        if (args.isEmpty())
        {
            return QString("@k:%1").arg(key);
        }
        return QString("@k:%1|%2").arg(key, args.join('|'));
    }

    inline bool isKeyed(const QString &value)
    {
        return value.startsWith("@k:");
    }

    inline QString key(const QString &value)
    {
        if (!isKeyed(value))
        {
            return {};
        }

        const QString payload = value.sliced(3);
        const int sep = payload.indexOf('|');
        if (sep < 0)
        {
            return payload;
        }
        return payload.left(sep);
    }

    inline QStringList args(const QString &value)
    {
        if (!isKeyed(value))
        {
            return {};
        }

        const QString payload = value.sliced(3);
        const int sep = payload.indexOf('|');
        if (sep < 0)
        {
            return {};
        }
        return payload.sliced(sep + 1).split('|');
    }
} // namespace i18nkey
