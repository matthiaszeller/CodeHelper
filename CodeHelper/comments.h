#ifndef COMMENTS_H
#define COMMENTS_H

#include <QString>
#include "setup.h"

// ======================================================
// -------------------- PARAM STRUCT ---------------------
// ======================================================

//! Represents all parameters
struct CommentParams {
    QString fillingChar;
    QString fillingChar2;
    QString spacingChar;
    bool commentCharBoth;
    bool capitalize;
    CommentStyle style;
    int length;
};

// ======================================================
// ------------------- CLASS COMMENT --------------------
// ======================================================

//! Generate comment blocks with given style
class Comments
{
public:
	Comments(CommentStyle comment_style);
	Comments();

    // ========= SET PARAMETERS ==========

    //! Change settings all at once with a struct
    void set_params(const CommentParams &params);
    //! Default style: filling character of top and bottom lines
	void set_filling_char(QString t)  { m_FillingChar = t; }
    //! Default style: filling character if the middle line (surrounding the text)
	void set_filling_char2(QString t)  { m_FillingChar2 = t; }
    //! String that separates the text from the filling characters
	void set_spacing_char(QString t)  { m_SpacingChar = t; }
    //! Whether we should put the comment characters on the right-hand side
	void set_comment_char_both_sides(bool b) { m_CommentCharBothSides = b; }
    //! Whether to capitalize the text
	void set_capitalize(bool b) { m_Capitalize = b; }
    //! Set the coding style
    void set_style(CommentStyle style) { m_Style = style; }
    //! Set the length of the line, i.e. number of columns
    void set_length(int len) { m_Length = len; }

    // ============ GENERATE =============

    //! Generate the comment block as a QString
    QString get_block_comment(QString text) const;

    // ============= STATIC ==============

    //! Process text and return the corresponding CommentParams
    //! @param ok : assign value depending on validity of the content of \param t
    static CommentParams process_params(const QString &t, bool &ok);

private:

    // =========== ATTRIBUTES ============

	CommentStyle m_Style;
	QString m_FillingChar;
	QString m_FillingChar2;
	QString m_SpacingChar;
	bool m_CommentCharBothSides;
	bool m_Capitalize;
    int m_Length;

    // ============ GENERATE =============

    //! Generate line containing the text
    QString getMiddleLine(const QString &side, const QString &text, const QString &c, const QString &c2, int length) const;
};



#endif // COMMENTS_H
