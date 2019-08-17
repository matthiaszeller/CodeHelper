#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

	// Graphic interface signals
	void on_lineEdit_prettify_comment_block_returnPressed();
};

#endif // MAINWINDOW_H
