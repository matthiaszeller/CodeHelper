#ifndef COMMENTS_H
#define COMMENTS_H

#include <QString>
#include "setup.h"

// TODO : not implemented yet
enum CommentStyle {
	Default
};

class Comments
{
public:
	Comments(CommentStyle comment_style);
	Comments();

	void set_filling_char(QString t)  { m_FillingChar = t; }
	void set_filling_char2(QString t)  { m_FillingChar2 = t; }
	void set_spacing_char(QString t)  { m_SpacingChar = t; }
	void set_comment_char_both_sides(bool b) { m_CommentCharBothSides = b; }
	void set_capitalize(bool b) { m_Capitalize = b; }

	QString get_block_comment(QString text, int length = 90);

private:
	CommentStyle m_Style;

	QString m_FillingChar;
	QString m_FillingChar2;
	QString m_SpacingChar;
	bool m_CommentCharBothSides;
	bool m_Capitalize;
};



#endif // COMMENTS_H
