from PyQt5.QtWidgets import QMainWindow, QApplication, QMessageBox, QFileDialog
from PyQt5.QtGui import QFont
from ui_mainwindow import Ui_MainWindow
from comments import Comment
from language import Python


class MainWindow(QMainWindow):

    def __init__(self):
        super().__init__()

        # Attributes
        self.comment_options = Comment.default_options
        self.language = Python()

        # UI setup
        self._ui = Ui_MainWindow()
        self._ui.setupUi(self)
        self.__setup_ui()

        # Widgets connections
        self.__connect_widgets()

    def __setup_ui(self):
        # TextEdit
        font = QFont('Monospace')
        font.setStyleHint(QFont.TypeWriter)
        self._ui.textEdit.setFont(font)

        # ===== Tab Pretty
        # Defaults
        self._ui.comboBox_pretty_filling.setCurrentText(self.comment_options['filling'])
        self._ui.comboBox_pretty_filling_2.setCurrentText(self.comment_options['filling2'])
        self._ui.checkBox_pretty_capitalize.setChecked(self.comment_options['capitalize'])
        self._ui.horizontalSlider_pretty_ncols.setValue(self.comment_options['length'])
        self._ui.checkBox_pretty_right.setChecked(self.comment_options['right-char'])

        # Filling characters
        self._ui.comboBox_pretty_filling.addItems(Comment.filling_chars)
        self._ui.comboBox_pretty_filling_2.addItems(Comment.filling_chars)

        # Other
        self._ui.label_pretty_ncols.setText(str(self._ui.horizontalSlider_pretty_ncols.value()))

    def __connect_widgets(self):
        # UI display
        self._ui.horizontalSlider_pretty_ncols.valueChanged.connect(
            lambda val: self._ui.label_pretty_ncols.setText(str(val))
        )
        # User interactions

        # Tab Pretty
        def link_filling_chars(current):
            if self._ui.checkBox_pretty_link.isChecked():
                self._ui.comboBox_pretty_filling_2.setCurrentText(current)
        self._ui.comboBox_pretty_filling.currentTextChanged.connect(link_filling_chars)

        self._ui.pushButton_pretty_load_text.clicked.connect(
            lambda: self.load_comment_options(self._ui.textEdit.toPlainText())
        )
        self._ui.pushButton_pretty_options_file.clicked.connect(
            lambda b: self.load_comment_options()
        )

        self._ui.pushButton_copy_textEdit.clicked.connect(
            lambda: QApplication.clipboard().setText(self._ui.textEdit.toPlainText())
        )
        self._ui.horizontalSlider_pretty_ncols.valueChanged.connect(
            lambda i: self.update_comments('length', i)
        )
        self._ui.lineEdit_pretty_comment.textChanged.connect(
            lambda t: self.update_comments()
        )
        self._ui.checkBox_pretty_capitalize.clicked.connect(
            lambda b: self.update_comments('capitalize', b)
        )
        self._ui.comboBox_pretty_filling.currentTextChanged.connect(
            lambda c: self.update_comments('filling', c)
        )
        self._ui.comboBox_pretty_filling_2.currentTextChanged.connect(
            lambda c: self.update_comments('filling2', c)
        )
        self._ui.checkBox_pretty_right.clicked.connect(
            lambda b: self.update_comments('right-char', b)
        )

        # # Data structure to manage signals of Pretty tab:
        # # tuples (trigger signal, key of comment options)
        # connections_prettytab = [
        #     (self._ui.horizontalSlider_pretty_ncols.valueChanged, lambda i: ('length', i)),
        #     (self._ui.lineEdit_pretty_comment.textChanged, lambda t: ('dummy', None)),
        #     (self._ui.comboBox_pretty_filling.currentTextChanged, lambda c: ('filling', c)),
        #     (self._ui.checkBox_pretty_capitalize.clicked, lambda b: ('capitalize', b))
        # ]
        #
        # for x in connections_prettytab:
        #     triggerfun, argsfun = x[0], x[1]
        #     print(*argsfun(1))
        #     triggerfun.connect(lambda arg: self.update_comments(*argsfun(arg)))

    def update_comments(self, key=None, new_value=None):
        if key is not None:
            self.comment_options[key] = new_value
        text = Comment.gen_comment(self._ui.lineEdit_pretty_comment.text(), self.language,
                                   self.comment_options)
        self._ui.textEdit.setText(text)

    def load_comment_options(self, data=None):
        print(data)
        try:
            if data is None:
                file, _ = QFileDialog.getOpenFileName(self, 'Open file')
                with open(file, 'r') as f:
                    data = f.read()

            self.comment_options = Comment.parse_options(data)
            self.__setup_ui()
        except ValueError:
            QMessageBox.information(self, 'Error', 'Parsing failed')

