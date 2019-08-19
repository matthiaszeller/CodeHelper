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
}

MainWindow::~MainWindow()
{
	delete ui;
	delete m_Comments;
}

void MainWindow::init_gui() {
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

void MainWindow::on_plainTextEdit_tools_textChanged() {
	ui->label_tools_size->setText(QString::number(ui->plainTextEdit_tools->toPlainText().size()));
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








