/*
 *  Copyright (C) 2011 Felix Geyer <debfx@fobos.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KEEPASSX_DATABASETABWIDGET_H
#define KEEPASSX_DATABASETABWIDGET_H

#include <QDateTime>
#include <QHash>
#include <QTabWidget>
#include <QSet>

#include "format/KeePass2Writer.h"
#include "gui/DatabaseWidget.h"

class DatabaseWidget;
class DatabaseOpenWidget;
class QFile;
class QFileSystemWatcher;

struct DatabaseManagerStruct
{
    DatabaseManagerStruct();

    DatabaseWidget* dbWidget;
    QString filePath;
    QString canonicalFilePath;
    QString fileName;
    bool saveToFilename;
    bool modified;
    bool readOnly;
    QDateTime lastModified;
};

Q_DECLARE_TYPEINFO(DatabaseManagerStruct, Q_MOVABLE_TYPE);

class DatabaseTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit DatabaseTabWidget(QWidget* parent = Q_NULLPTR);
    ~DatabaseTabWidget();
    void openDatabase(const QString& fileName, const QString& pw = QString(),
                      const QString& keyFile = QString(), const CompositeKey& key = CompositeKey(),
                      int index = -1);
    DatabaseWidget* currentDatabaseWidget();
    bool hasLockableDatabases();

    static const int LastDatabasesCount;

    enum ReloadBehavior {
        AlwaysAsk,
        ReloadUnmodified,
        IgnoreAll
    };

public Q_SLOTS:
    void newDatabase();
    void openDatabase();
    void importKeePass1Database();
    void fileChanged(const QString& fileName);
    void checkReloadDatabases();
    void saveDatabase(int index = -1);
    void saveDatabaseAs(int index = -1);
    bool closeDatabase(int index = -1);
    void closeDatabaseFromSender();
    bool closeAllDatabases();
    void changeMasterKey();
    void changeDatabaseSettings();
    bool readOnly(int index = -1);
    void performGlobalAutoType();
    void lockDatabases();

Q_SIGNALS:
    void tabNameChanged();
    void databaseWithFileClosed(QString filePath);

private Q_SLOTS:
    void updateTabName(Database* db);
    void updateTabNameFromDbSender();
    void updateTabNameFromDbWidgetSender();
    void modified();
    void toggleTabbar();
    void changeDatabase(Database* newDb);

private:
    void saveDatabase(Database* db);
    void saveDatabaseAs(Database* db);
    bool closeDatabase(Database* db);
    void deleteDatabase(Database* db);
    int databaseIndex(Database* db);
    Database* indexDatabase(int index);
    DatabaseManagerStruct indexDatabaseManagerStruct(int index);
    Database* databaseFromDatabaseWidget(DatabaseWidget* dbWidget);
    void insertDatabase(Database* db, const DatabaseManagerStruct& dbStruct, int index = -1);
    void updateRecentDatabases(const QString& filename);
    void connectDatabase(Database* newDb, Database* oldDb = Q_NULLPTR);
    void expectFileChange(const DatabaseManagerStruct& dbStruct);
    void unexpectFileChange(DatabaseManagerStruct& dbStruct);

    KeePass2Writer m_writer;
    QHash<Database*, DatabaseManagerStruct> m_dbList;
    QSet<QString> m_changedFiles;
    QSet<QString> m_expectedFileChanges;
    QFileSystemWatcher* m_fileWatcher;
};

#endif // KEEPASSX_DATABASETABWIDGET_H
