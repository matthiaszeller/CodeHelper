#include "comments.h"

Comments::Comments(): Comments(CommentStyle::Default) {

}

Comments::Comments(CommentStyle comment_style):
	m_Style(comment_style)
{

}

// ---------------

QString Comments::get_block_comment(QString text, int length) {
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
	t = l1 + "\n";
	int N = text.size();
	int N_side = length - 2*side.size() - 2*c.size() - 2 - N;
	// We want the same number of characters on the left & right hand side
	if(N_side % 2 != 0)
		return get_block_comment(text, length+1);
	N_side /= 2;
	t += c + side + m_FillingChar2.repeated(N_side) + " " + text
			+ " " + m_FillingChar2.repeated(N_side) + side + c2;

	t += "\n" + l1;

	return t;
}









