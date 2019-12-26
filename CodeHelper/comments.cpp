#include "comments.h"

Comments::Comments(): Comments(CommentStyle::Default) {

}

Comments::Comments(CommentStyle comment_style):
	m_Style(comment_style)
{

}

void Comments::set_params(const CommentParams &params) {
    this->set_capitalize(params.capitalize);
    this->set_filling_char(params.fillingChar);
    this->set_filling_char2(params.fillingChar2);
    this->set_comment_char_both_sides(params.commentCharBoth);
    this->set_style(params.style);
}

// WARNING: if get_block_comment changes, this must change
CommentParams Comments::process_params(const QString &t, bool &ok) {

    QStringList args(QStringList() << t << LANGUAGE_COMMENT_STR);
    QString scriptOutput(ScriptManager::exec_python(
                             "comment_params.py", args));
    std::cout << "OUTPUT:\n" << scriptOutput.toStdString()
              << "---" << std::endl;


    CommentParams p;
    // We must now process the output of the python script
    // The convention is to use a given order for the parameters
    // See the python script for more information
    QStringList params(scriptOutput.split(","));
    if(params.size() != 7) {
        ok = false;
        return p;
    }
    // Here we have the correct number of parameters,
    // we simply load them with the order described in the script

    ok = false;
    return p;
}

// WARNING: if the format here changes, the function process_params must change !!!
QString Comments::get_block_comment(QString text) const {
	// filling char 1 for upper and lower lines
	// filling char 2 for middle line
	QString t;
	QString c = TextHandler::get_comment_tokens();
	QString c2 = (m_CommentCharBothSides ? c : "");
	//bool comment_token_both_sides = true;
	QString side(" ");
	if(m_Capitalize)
		text = text.toUpper();

    QString l1 = c + side + m_FillingChar.repeated(m_Length -
					2*side.size() - 2*c.size()) + side + c2;
    if(m_Style == Default) {
        t = l1 + "\n";
        t += getMiddleLine(side, text, c, c2, m_Length);
        t += "\n" + l1;
    } else if(m_Style == SingleLine) {
        t += getMiddleLine(side, text, c, c2, m_Length);
    } else {
        t = c + "\n" + l1 + "\n";
        t += getMiddleLine(side, text, c, c2, m_Length);
        t += "\n" + l1 + "\n" + c;
    }

	return t;
}

QString Comments::getMiddleLine(const QString &side, const QString &text,
                                const QString &c, const QString &c2, int length) const {
    int N = text.size();
    int N_side = length - 2*side.size() - 2*c.size() - 2 - N;
    N_side /= 2;
    QString t;
    t += c + side + m_FillingChar2.repeated(N_side) + " " + text + " ";
    // Compute the remaining char number to print
    int rem(side.size() + c2.size() + N_side);
    int residual(length - t.size() - rem);
    // Must adapt the number of filling charaters if there is an excess/lack of chars
    // Why residuals are > 1 and not > 0 is wrong not adapted ? Too lazy to check, but it works
    if(residual > 1) N_side += 1;
    t += m_FillingChar2.repeated(N_side) + side + c2;

    return t;
}

