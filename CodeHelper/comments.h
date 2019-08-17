#ifndef COMMENTS_H
#define COMMENTS_H

#include <QString>
#include "setup.h"

enum CommentStyle {
	Default
};

class Comments
{
public:
	Comments(CommentStyle comment_style);
	Comments() : Comments(CommentStyle::Default) {}

	QString get_block_comment(QString text, short unsigned int length = 50);

private:
	CommentStyle m_Style;
};

#endif // COMMENTS_H
