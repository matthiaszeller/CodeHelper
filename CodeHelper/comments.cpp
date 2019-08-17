#include "comments.h"

Comments::Comments(): Comments(CommentStyle::Default) {

}

Comments::Comments(CommentStyle comment_style):
	m_Style(comment_style)
{

}

// ---------------

QString Comments::get_block_comment(QString text, int length) {
	QString t;
	QString c = TextHandler::get_comment_tokens();
	bool comment_token_both_sides = true;
	QString f(TextHandler::get_filling_token());
	QString side(" ");

	QString l1 = c + side + f.repeated(length - 2*side.size() - 2*c.size()) + side + c;
	t = l1 + "\n";
	int N = text.size();
	int N_side = length - 2*side.size() - 2*c.size() - 2 - N;
	N_side /= 2;
	t += c + side + f.repeated(N_side) + " " + text + " " + f.repeated(N_side) + side + c;

	t += "\n" + l1;

	return t;
}









