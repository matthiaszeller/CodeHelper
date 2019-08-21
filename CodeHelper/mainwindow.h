#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QClipboard>
#include "setup.h"
#include "comments.h"
#include "texttools.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
	// --------- Setting up
	void init_gui();

private:
	Ui::MainWindow *ui;

	// --------- Custom private functions
	void write_comments(const QString &txt);



	// --------- Custom private objects
	Comments *m_Comments;
	QClipboard *m_Clipboard;

private slots:
	// Graphic interface signals

	// TAB - Text tools
	void on_lineEdit_tools_regexp_textChanged(const QString &t);
	void on_plainTextEdit_tools_textChanged(const QString &t);


	// TAB - Prettify

	// Update comments
	void update_comments();

	void on_lineEdit_prettify_comment_block_textChanged();
	void on_horizontalSlider_prettify_comment_block_sliderMoved(int);
	void on_pushButton_prettify_copy_clicked(bool);
	void on_comboBox_prettify_filling_char_currentTextChanged(QString);
	void on_comboBox_prettify_filling_char2_currentTextChanged(QString);
	void on_comboBox_language_currentTextChanged(QString);
	void on_checkBox_prettify_both_sides_toggled(bool);
};

#endif // MAINWINDOW_H
