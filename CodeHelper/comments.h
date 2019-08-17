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
	Comments();

	QString get_block_comment(QString text, int length = 90);

private:
	CommentStyle m_Style;
};

#endif // COMMENTS_H
