#include "docstringmanager.h"

DocstringManager::DocstringManager() :
	m_FileName(QString())
{

}

// FILE CONTENT ANALYSIS DEDICATED METHODS
void DocstringManager::process_file() { // public
	// 1. Open the file
	QFile file(m_FileName);
	if(!file.open(QIODevice::ReadOnly, QIODevice::Text))
		return; // TODO: process this in another way
	QTextStream in(&file);
	// 2. Read the file
	while(!in.atEnd()) {
		process_line(in.readLine());
	}
}

void DocstringManager::process_line(const QString &t) {

}

// FILE-DEDICATED PRIVATE METHODS

bool DocstringManager::file_exists(const QString &filename) {
	return QFile::exists(filename);
}

// SETTERS

void DocstringManager::set_file_name(const QString &filename) {
	if(!file_exists(filename))
		return; // TODO: manage this in another way

	m_FileName = filename;
}







