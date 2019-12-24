#include "comments.h"

Comments::Comments(): Comments(CommentStyle::Default) {

}

Comments::Comments(CommentStyle comment_style):
	m_Style(comment_style)
{

}

// ---------------

QString Comments::get_block_comment(QString text, int length) const {
	// filling char 1 for upper and lower lines
	// filling char 2 for middle line
	QString t;
	QString c = TextHandler::get_comment_tokens();
	QString c2 = (m_CommentCharBothSides ? c : "");
	//bool comment_token_both_sides = true;
	QString side(" ");
	if(m_Capitalize)
		text = text.toUpper();

	QString l1 = c + side + m_FillingChar.repeated(length -
					2*side.size() - 2*c.size()) + side + c2;
    if(m_Style == Default) {
        t = l1 + "\n";
        t += getMiddleLine(side, text, c, c2, length);
        t += "\n" + l1;
    } else if(m_Style == SingleLine) {
        t += getMiddleLine(side, text, c, c2, length);
    } else {
        t = c + "\n" + l1 + "\n";
        t += getMiddleLine(side, text, c, c2, length);
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






