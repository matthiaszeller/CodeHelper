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
	ui->comboBox_prettify_style->setCurrentText(DEFAULT_COMMENT_STYLE);
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

    //std::cout << "\"" << text.toStdString() << "\"" << std::endl;
    //std::cout << "\"" << pattern.toStdString() << "\"" << std::endl;

    if(regexp && text.contains(QRegExp(pattern, sensitivity)))
        ui->label_tools_pattern->setText("True");
    else if(!regexp && text.contains(pattern, sensitivity))
        ui->label_tools_pattern->setText("True");
    else
        ui->label_tools_pattern->setText("False");
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
    update_comments();
}

void MainWindow::on_checkBox_tools_regexp_toggled(bool) {
    update_comments();
}

void MainWindow::on_pushButton_tools_copy_clicked() {
    m_Clipboard->setText(ui->plainTextEdit_tools->toPlainText());
}

// ================================================================ //
// ------------------------- TAB PRETTIFY ------------------------- //
// ================================================================ //

void MainWindow::write_comments(const QString &txt) {
    ui->plainTextEdit_prettify->setPlainText(txt);
}

// Whenever there is sth to update in the prettify tab
void MainWindow::update_comments() {
	m_Comments->set_filling_char(ui->comboBox_prettify_filling_char->currentText());
	m_Comments->set_filling_char2(ui->comboBox_prettify_filling_char2->currentText());
	m_Comments->set_spacing_char(ui->lineEdit_prettify_spacing->text());
	m_Comments->set_comment_char_both_sides(ui->checkBox_prettify_both_sides->isChecked());
	// Guess if the user was writing a comment (bloc)
	// to reload the comment style
	if(!ui->lineEdit_prettify_comment_block->text().isEmpty())
		// Simulate a change of text
		this->on_lineEdit_prettify_comment_block_textChanged();
}

// ----------------------------- PRIVATE SLOTS

// TAB - Text tools

void MainWindow::on_lineEdit_tools_regexp_textChanged(const QString &t) {
	QString pattern(ui->lineEdit_tools_pattern->text());

	bool b(TextTools::contains(t, pattern));
	ui->label_tools_pattern->setText(b ? "True" : "False");
}


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

void MainWindow::on_comboBox_prettify_filling_char2_currentTextChanged(QString t) {
	update_comments();
}

void MainWindow::on_comboBox_language_currentTextChanged(QString t) {
	CURRENT_LANGUAGE = map(t);
	update_comments();
}

void MainWindow::on_checkBox_prettify_both_sides_toggled(bool b) {
	update_comments();
}






