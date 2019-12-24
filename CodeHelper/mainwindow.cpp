#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_Comments(new Comments()), m_Clipboard(QApplication::clipboard())
{
	ui->setupUi(this);

    QTimer::singleShot(80, this, SLOT(init_gui()));
    QTimer::singleShot(120, this, SLOT(update_comments()));
    QTimer::singleShot(120, this, SLOT(on_plainTextEdit_tools_textChanged()));
}

MainWindow::~MainWindow()
{
    save_gui();
	delete ui;
	delete m_Comments;
}

// ================================================================ //
// ---------------------- GUI INITIALIZATION ---------------------- //
// ================================================================ //

void MainWindow::init_gui() {
    // GUI PARAMETERS
    QString prettify_filling_char(DEFAULT_FILLING_CHAR),
            prettify_filling_char2(DEFAULT_FILLING_CHAR);

    // Look if INI file exists
    QSettings settings(FILENAME_SETTINGS, QSettings::IniFormat);
    settings.beginGroup("tools");
	ui->checkBox_tools_case_sensitivity->setChecked(settings.value("case_sensitive", DEFAULT_TOOLS_SENSITIVE).toBool());
	ui->checkBox_tools_regexp->setChecked(settings.value("regexp", DEFAULT_TOOLS_REGEXP).toBool());

	// Setup default values for widgets

	// TAB - Prettify

	// Comment style
    ui->comboBox_prettify_style->setCurrentIndex(DEFAULT_COMMENT_STYLE);
	// Filling characters
	// QPlainTextEdit paragraph symbols
	QTextOption option;
	option.setFlags(QTextOption::ShowLineAndParagraphSeparators);
	ui->plainTextEdit_prettify->document()->setDefaultTextOption(option);
	// QPlainTextEdit font (monospaced)
	QFont f("unexistent");
	f.setStyleHint(QFont::Monospace);
	ui->plainTextEdit_prettify->setFont(f);
	// QSlider for comment width
	ui->horizontalSlider_prettify_comment_block->setValue(DEFAULT_COMMENT_BLOCK_WIDTH);
	this->on_horizontalSlider_prettify_comment_block_sliderMoved(DEFAULT_COMMENT_BLOCK_WIDTH);
	// Default filling char
	ui->comboBox_prettify_filling_char->addItems(COMMENTS_FILLING_CHARACTERS);
	ui->comboBox_prettify_filling_char->setCurrentText(DEFAULT_FILLING_CHAR);
	ui->comboBox_prettify_filling_char2->addItems(COMMENTS_FILLING_CHARACTERS);
	ui->comboBox_prettify_filling_char2->setCurrentText(DEFAULT_FILLING_CHAR);
	ui->checkBox_prettify_link->setChecked(DEFAULT_FILLING_CHARS_LINKAGE);
}

void MainWindow::save_gui() {
    QSettings settings(FILENAME_SETTINGS, QSettings::IniFormat);

    settings.beginGroup("tools");
    settings.setValue("regexp", ui->checkBox_tools_regexp->isChecked());
    settings.setValue("case_sensitive", ui->checkBox_tools_case_sensitivity->isChecked());
    settings.endGroup();
}

// ================================================================ //
// ------------------------ TAB TEXT TOOLS ------------------------ //
// ================================================================ //

void MainWindow::update_tools() {
    QString pattern(ui->lineEdit_tools_pattern->text());
    QString text(ui->plainTextEdit_tools->toPlainText());
    Qt::CaseSensitivity sensitivity(ui->checkBox_tools_case_sensitivity->isChecked()
                                    ? Qt::CaseSensitive : Qt::CaseInsensitive);
	bool regexp(ui->checkBox_tools_regexp->isChecked());

	bool logical_value;
    if(regexp && text.contains(QRegExp(pattern, sensitivity)))
		logical_value = true;
    else if(!regexp && text.contains(pattern, sensitivity))
		logical_value = true;
	else
		logical_value = false;
	// Text formatting
	QString t(QString("<b><font color=\"%1\">%2</font></b>")
			  .arg(logical_value ? "green" : "red")
			  .arg(logical_value ? "True" : "False"));
	ui->label_tools_pattern->setText(t);
}

void MainWindow::on_lineEdit_tools_pattern_textChanged(QString) {
    update_tools();
}

void MainWindow::on_plainTextEdit_tools_textChanged() {
    const QString &t(ui->plainTextEdit_tools->toPlainText());
    // Update number of characters
    ui->label_tools_size->setText(QString::number(t.size()));
    // Update number of lines
    int n(0);
    if(t.isEmpty())
        n = 0;
    else
        n = t.count("\n") + 1;
    ui->label_tools_lines->setText(QString::number(n));
    //
    update_tools();
}

void MainWindow::on_checkBox_tools_case_sensitivity_toggled(bool) {
	update_tools();
}

void MainWindow::on_checkBox_tools_regexp_toggled(bool) {
	update_tools();
}

void MainWindow::on_pushButton_tools_copy_clicked() {
    m_Clipboard->setText(ui->plainTextEdit_tools->toPlainText());
}

void MainWindow::on_pushButton_tools_clear_clicked() {
	// This would delete the undo/redo history :
	//		ui->plainTextEdit_tools->setPlainText("");
	// So we find another way of doing this
	//		https://www.qtcentre.org/threads/43268-Setting-Text-in-QPlainTextEdit-without-Clearing-Undo-Redo-History?p=197703#post197703
	QTextDocument *doc = ui->plainTextEdit_tools->document();
	QTextCursor curs(doc);
	curs.select(QTextCursor::Document);
	curs.removeSelectedText();
	ui->plainTextEdit_tools->setFocus();
}

// ================================================================ //
// ------------------------- TAB PRETTIFY ------------------------- //
// ================================================================ //

void MainWindow::write_comments(const QString &txt) {
    ui->plainTextEdit_prettify->setPlainText(txt);
}

// Whenever there is sth to update in the prettify tab
void MainWindow::update_comments() {
    // Coding style, filling characters
	m_Comments->set_filling_char(ui->comboBox_prettify_filling_char->currentText());
	m_Comments->set_filling_char2(ui->comboBox_prettify_filling_char2->currentText());
	m_Comments->set_spacing_char(ui->lineEdit_prettify_spacing->text());
	m_Comments->set_comment_char_both_sides(ui->checkBox_prettify_both_sides->isChecked());
	m_Comments->set_capitalize(ui->checkBox_prettify_capitalize->isChecked());
    // We use map to convert int to CommentStyle (enum)
    m_Comments->set_style(map(ui->comboBox_prettify_style->currentIndex()));

	// Guess if the user was writing a comment (bloc)
	// to reload the comment style
	if(!ui->lineEdit_prettify_comment_block->text().isEmpty())
		// Simulate a change of text
		this->on_lineEdit_prettify_comment_block_textChanged();
}

// ----------------------------- PRIVATE SLOTS

// TAB - Text tools

// /////////

void MainWindow::on_lineEdit_prettify_comment_block_textChanged() {
	// Get text form the QLineEdit
	QString txt(ui->lineEdit_prettify_comment_block->text());
	// Write in the QPlainTextEdit the block comment
	write_comments(m_Comments->get_block_comment(txt,
					ui->horizontalSlider_prettify_comment_block->value()));
	// If no more text is present in the LineEdit,
	// but the user has just pressed the delete key -> wipe out
	if(txt.isEmpty())
		ui->plainTextEdit_prettify->clear();
}

void MainWindow::on_horizontalSlider_prettify_comment_block_sliderMoved(int i) {
	ui->label_prettify_block_comment_width->setText(QString::number(i));
	update_comments();
}

void MainWindow::on_pushButton_prettify_copy_clicked(bool) {
	// Copy content of the QPlainTextEdit to clipboard
	m_Clipboard->setText(ui->plainTextEdit_prettify->toPlainText());
}

void MainWindow::on_comboBox_prettify_filling_char_currentTextChanged(QString t) {
	// Manage the link with filling char 2
	if(ui->checkBox_prettify_link->isChecked())
		ui->comboBox_prettify_filling_char2->setCurrentText(t);

	update_comments();
}

void MainWindow::on_comboBox_prettify_filling_char2_currentTextChanged(QString) {
	update_comments();
}

void MainWindow::on_comboBox_language_currentTextChanged(QString t) {
	CURRENT_LANGUAGE = map(t);
	update_comments();
}

void MainWindow::on_checkBox_prettify_both_sides_toggled(bool) {
	update_comments();
}

void MainWindow::on_checkBox_prettify_capitalize_stateChanged(int) {
	update_comments();
}

void MainWindow::on_comboBox_prettify_style_currentIndexChanged(int) {
    update_comments();
}

// ================================================================ //
// ------------------------ TAB C++ HELPER ------------------------ //
// ================================================================ //

void MainWindow::update_cpp_helper() {
	// Get the content of the directory
	QString dir(ui->lineEdit_cpp_dir->text());
	QStringList files(QDir(dir).entryList(QDir::NoDotAndDotDot | QDir::Files
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
}


void MainWindow::on_toolButton_cpp_dir_clicked() {
	QString dir(QFileDialog::getExistingDirectory(this,
				"Select a directory"));
	ui->lineEdit_cpp_dir->setText(dir);
	update_cpp_helper();
}





