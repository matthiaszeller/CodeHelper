#include "comments.h"

Comments::Comments(CommentStyle comment_style):
	m_Style(comment_style)
{

}

// ---------------

QString Comments::get_block_comment(QString text, unsigned short length) {
	QString t;
	QString c = get_comment_tokens();
	// ##################################### //
	// ######## WANT STH LIKE THIS ######### //
	// ##################################### //
	t += "\n";
	sep = c + " ";
	for (int i(0); i < )
}









