#ifndef DOCSTRINGMANAGER_H
#define DOCSTRINGMANAGER_H

#include <QString>
#include <QFile>
#include <QTextStream>

/* This class implement the features represented in the Docstring tab of the
 * mainwindow.
 *
 * Features implemented with mainwindow:
 *		- extract functions (and classes) from the content of a file
 *		- allow to write the description, parameters, input, outputs, ...
 *		  in a standardized manner (and nice visual)
 */


class DocstringManager
{
public:
	DocstringManager();

	// FILE CONTENT ANALYSIS DEDICATED METHODS
	QStringList get_functions() const;

	void process_file();

	// SETTERS
	void set_file_name(const QString &filename);

private:
	QString m_FileName;

	// FILE-DEDICATED METHODS
	static bool file_exists(const QString &filename);

	// FILE CONTENT ANALYSIS DEDICATED METHODS
	void process_line(const QString &t);
};

#endif // DOCSTRINGMANAGER_H



