/*  OpenFIRE App: a configuration utility for the OpenFIRE light gun system.
    Copyright (C) 2024  Team OpenFIRE

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "appmainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set settings scope for language preference, etc.
    QCoreApplication::setOrganizationName(QStringLiteral("TeamOpenFIRE"));
    QCoreApplication::setApplicationName(QStringLiteral("OpenFIREapp"));

#ifdef Q_OS_WIN
    // set fusion style, which will use system palette on Qt 6.5+
    // (Qt < 6.5 needs a custom dark palette.)
    a.setStyle("fusion");
#if QT_VERSION_MAJOR < 6 || (QT_VERSION_MAJOR > 5 && QT_VERSION_MINOR < 5)
    // Windows: (attempt to) respect light/dark mode setting in Qt 5.15-6.4
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=[1|2]");
    // TODO: a custom dark palette is necessary for Qt < 6.5
#endif // QT_VERSION
#endif // Q_OS_WIN

    QTranslator translator;
    QSettings settings;
    const QString langPref = settings.value(QStringLiteral("language")).toString();

    auto tryLoadTranslation = [&](const QString &baseName) -> bool {
        // Primary: compiled-in resources (when translations are embedded under ":/i18n/")
        if (translator.load(QStringLiteral(":/i18n/") + baseName)) {
            a.installTranslator(&translator);
            return true;
        }

        // Fallback: app-local directory (useful for portable builds)
        const QString appDir = QCoreApplication::applicationDirPath();
        const QString i18nDir = QDir(appDir).filePath(QStringLiteral("i18n"));
        if (translator.load(baseName, i18nDir)) {
            a.installTranslator(&translator);
            return true;
        }

        return false;
    };

    if (!langPref.isEmpty() && langPref != QLatin1String("system")) {
        const QString baseName = QStringLiteral("AppTranslations_") + langPref;
        tryLoadTranslation(baseName);
    } else {
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale : uiLanguages) {
            const QString baseName = QStringLiteral("AppTranslations_") + QLocale(locale).name();
            if (tryLoadTranslation(baseName)) {
                break;
            }
        }
    }
    guiWindow w;
    w.show();
    return a.exec();
}
