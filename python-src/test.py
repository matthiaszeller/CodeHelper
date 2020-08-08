
from PyQt5.QtWidgets import QApplication
from mainwindow import MainWindow
import sys


app = QApplication(sys.argv)
main = MainWindow()
main.show()
app.exec()
