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
	// Save
    save_gui();
	// Delete actions
	for (QAction *a : m_MapAction)
		delete a;
	// Other
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

	// /////// TAB - Prettify

	// Comment style
<<<<<<< HEAD
	ui->comboBox_prettify_style->setCurrentText(DEFAULT_COMMENT_STYLE);
=======
    ui->comboBox_prettify_style->setCurrentIndex(DEFAULT_COMMENT_STYLE);
	// Filling characters
>>>>>>> master
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
	// Default filling chars
	ui->comboBox_prettify_filling_char->addItems(COMMENTS_FILLING_CHARACTERS);
	ui->comboBox_prettify_filling_char->setCurrentText(DEFAULT_FILLING_CHAR);
	ui->comboBox_prettify_filling_char2->addItems(COMMENTS_FILLING_CHARACTERS);
	ui->comboBox_prettify_filling_char2->setCurrentText(DEFAULT_FILLING_CHAR);
	ui->checkBox_prettify_link->setChecked(DEFAULT_FILLING_CHARS_LINKAGE);
	// Default number of lines for a comment block
	bool defaultIs1 = (DEFAULT_COMMENT_LINE_NUMBER == 1);
	if (defaultIs1)
		ui->radioButton_prettify_1line->click();
	else
		ui->radioButton_prettify_3lines->click();

	// /////// TAB - CPP HELPER
	QFont font("Monospace");
	font.setStyleHint(QFont::TypeWriter);
	ui->lineEdit_tools_pattern->setFont(font);
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

    int idx;
    if(regexp)
        idx = text.indexOf(QRegExp(pattern, sensitivity));
    else
        idx = text.indexOf(pattern, sensitivity);
	// Text formatting
    QString t(QString("<b><font color=\"%1\">%2</font></b>")
              .arg((idx != -1) ? "green" : "red")
              .arg(idx));
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
    m_Comments->set_length(ui->horizontalSlider_prettify_comment_block->value());
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
    write_comments(m_Comments->get_block_comment(txt));

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

void MainWindow::on_pushButton_prettify_load_clicked(bool) {
    // Load configuration of comments from the content of the clipboard
    QString clipContent = m_Clipboard->text();
    bool ok;
    CommentParams params(Comments::process_params(clipContent, ok));
    if(!ok) {
        std::cerr << "The processing of params failed !" << std::endl;
        return;
    }
    //m_Comments->set_params(params);
    update_comments_gui(params);
}

void MainWindow::update_comments_gui(const CommentParams &p) {
    ui->checkBox_prettify_capitalize->setChecked(p.capitalize);
    ui->comboBox_prettify_style->setCurrentIndex(p.style);
    ui->comboBox_prettify_filling_char->setCurrentText(p.fillingChar);
    ui->comboBox_prettify_filling_char2->setCurrentText(p.fillingChar2);
    ui->checkBox_prettify_both_sides->setChecked(p.commentCharBoth);
    ui->horizontalSlider_prettify_comment_block->setValue(p.length);

    update_comments();
}

// ================================================================ //
// ------------------------ TAB C++ HELPER ------------------------ //
// ================================================================ //

void MainWindow::update_cpp_helper() {


/*
 *
	// Set the list of files to the QListWidget
	ui->listWidget_cpp_classes->clear();
	ui->listWidget_cpp_classes->addItems(cpp_files);
*/
}


void MainWindow::on_toolButton_cpp_dir_clicked() {
	QString dir(QFileDialog::getExistingDirectory(this,
				"Select a directory"));
	ui->lineEdit_cpp_dir->setText(dir);
	update_cpp_helper();
}





