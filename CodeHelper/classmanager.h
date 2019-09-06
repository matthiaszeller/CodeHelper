#ifndef CLASSMANAGER_H
#define CLASSMANAGER_H

#include <QString>
#include <memory>
#include <QDir>
#include <QVector>
#include <QFile>
#include "setup.h"

/** @brief Modify classes contained in a directory.
 *
 **/

class ClassManager
{
	/**
	 * @brief The Class struct stores C++ classes and their associated filenames
	 */
	struct Class {
		QString name;
		bool hasCppFile;
		QString fileName;
		int def_location;
	};

public:
	/**
	 * @brief ClassManager reads the content of a directory and allows to tune the classes contained in it.
	 * @param directory
	 */
	ClassManager(const QString &directory);
	~ClassManager();

private:
	// METHODS
	void load();
	void load_files();
	void load_classes(QStringList files, QVector<bool> cpp_files);

	/**
	 * @brief extract_classes detects the classes in a string and stores the line where the class is defined.
	 * @param file_content
	 */
	void extract_classes(const QString &file_content);

	// ATTRIBUTES
	QString m_Directory;
	std::unique_ptr< QVector<Class> > m_Classes;
};

#endif // CLASSMANAGER_H






