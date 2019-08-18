#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_Comments(new Comments())
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
	delete m_Comments;
}

void MainWindow::write_comments(const QString &txt) {
	ui->plainTextEdit_prettify->setPlainText(txt);
}

// ----------------------------- PRIVATE SLOTS

void MainWindow::on_lineEdit_prettify_comment_block_returnPressed() {
	write_comments(m_Comments->get_block_comment(ui->lineEdit_prettify_comment_block->text()));
}

void MainWindow::on_plainTextEdit_tools_textChanged() {
	ui->label_tools_size->setText(QString::number(ui->plainTextEdit_tools->toPlainText().size()));
}













