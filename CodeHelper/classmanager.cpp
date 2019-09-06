#include "classmanager.h"

ClassManager::ClassManager(const QString &directory) :
	m_Directory(directory),
	m_Classes(new QVector<Class>())
{
	load();
}

void ClassManager::load() {
	load_files();
}

void ClassManager::load_files() {
	// Get the content of the directory
	QStringList files(QDir(m_Directory).entryList(QDir::NoDotAndDotDot | QDir::Files
										  | QDir::Readable | QDir::Writable));
	// Filter & keep only C++ files without extension
	QStringList cpp_files;
	for(QString file : files) {
		if(file.right(2) == ".h")
			cpp_files.append(file.remove(".h"));
		else if(file.right(4) == ".cpp")
			cpp_files.append(file.remove(".cpp"));
	}
	cpp_files.removeDuplicates();

	// Check that there is at least a header file
	for(const QString &file : cpp_files) {
		if(!files.contains(file+".h"))
			cpp_files.removeOne(file);
	}
	/* --------> From this point, we have the list of C++ files contained
	 *			 in the directory. Assuming a given C++ has at least a .h file,
	 *			 and optionnaly a .cpp file.
	 */
	const QStringList file_names(cpp_files);
	QVector<bool> have_cpp_file;

	// Create the list of booleans about having both .h and .cpp files or not
	for(const QString &file : cpp_files)
		have_cpp_file.append( (files.contains(file+".cpp")) );

	load_classes(file_names, have_cpp_file);
}

void ClassManager::load_classes(QStringList files, QVector<bool> cpp_files) {
	// Read the header files and look for the classes
	for(QString file_name : files) {
		file_name = m_Directory+"/"+file_name+".h";
		QFile file(file_name);
		// Open the file
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			q("ERROR: can't open "+file_name);
			continue;
		}
		// Read the file
		QTextStream in(&file);
		// Send the file content to a custom processing function
		this->extract_classes(in.readAll());
		file.close();
	}
}

void ClassManager::extract_classes(const QString &file_content) {

}

/*
 * 	// Check that there is at least a header file
	for(QString file : cpp_files) {
		if(!files.contains(file+".h"))
			cpp_files.removeOne(file);
	}
	// Read the header files and look for the classes
	for(QString file_name : cpp_files) {
		file_name = ui->lineEdit_cpp_dir->text()+"/"+file_name+".h";
		QFile file(file_name);
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			q("ERROR: can't open "+file_name);
			continue;
		}
		QTextStream in(&file);
		file.close();
	}
	// Set the list of files to the QListWidget
	ui->listWidget_cpp_classes->clear();
	ui->listWidget_cpp_classes->addItems(cpp_files);
*/
