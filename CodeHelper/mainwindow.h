#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QClipboard>
#include "setup.h"
#include "comments.h"
#include "texttools.h"
#include <iostream>
#include <QSettings>
#include <QFileDialog>
#include "scriptmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
	Ui::MainWindow *ui;

	// --------- Custom private functions
	void write_comments(const QString &txt);



	// --------- Custom private objects
	//! Comments managing class
	Comments *m_Comments;
	//! Clipboard manager
	QClipboard *m_Clipboard;

	// --------- Manage QActions
	//! Map of QActions, easy deletion as destruction
	QMap<QString, QAction*> m_MapAction;

	//! Save parameters of widgets
    void save_gui();

private slots:

	// --------- Setting up
	//! Trigger this function (delay !) when the program starts
	void init_gui();

	// --------- GRAPHICAL INTERFACE - SIGNALS

	// TAB - Text tools
	void on_plainTextEdit_tools_textChanged();
    void on_lineEdit_tools_pattern_textChanged(QString);
    void on_checkBox_tools_case_sensitivity_toggled(bool);
    void on_checkBox_tools_regexp_toggled(bool);
	void on_pushButton_tools_copy_clicked();
	void on_pushButton_tools_clear_clicked();

	void update_tools();

	// TAB - Prettify

	// Update comments
	void update_comments();
    void update_comments_gui(const CommentParams &p);

	void on_lineEdit_prettify_comment_block_textChanged();
	void on_horizontalSlider_prettify_comment_block_sliderMoved(int);
	void on_pushButton_prettify_copy_clicked(bool);
	void on_comboBox_prettify_filling_char_currentTextChanged(QString);
	void on_comboBox_prettify_filling_char2_currentTextChanged(QString);
	void on_comboBox_language_currentTextChanged(QString);
    void on_checkBox_prettify_both_sides_toggled(bool);
	void on_checkBox_prettify_capitalize_stateChanged(int);
    void on_comboBox_prettify_style_currentIndexChanged(int);
    void on_pushButton_prettify_load_clicked(bool);

	// TAB - C++ helper
	void on_toolButton_cpp_dir_clicked();

	void update_cpp_helper();
};

#endif // MAINWINDOW_H
