#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "setup.h"
#include "comments.h"

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
	Comments *m_Comments;

private slots:
	// Graphic interface signals
	void on_lineEdit_prettify_comment_block_returnPressed();
	void on_plainTextEdit_tools_textChanged();
};

#endif // MAINWINDOW_H
