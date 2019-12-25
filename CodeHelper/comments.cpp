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
    // Manage the report of success/failure
    ok = true;
    // Initialize data structure of parameters
    CommentParams p;
    // Here we need to get the settings for the different languages
    QString commentChar(LANGUAGE_COMMENT_STR[CURRENT_LANGUAGE]);
    // --- PROCESS
    // 1. Detect if comment at all, look at comment string (`//`, `#`, ...)
    int idx(t.indexOf(commentChar));
    if(idx == -1) ok = false;
    // 2. Comment style: detect number of comment lines
    int idx2(t.indexOf(commentChar, idx));
    int idxOfNextLine(t.indexOf("\n", idx));
    QString textLine;
    if(idx2 == -1) { // Single line
        p.style = CommentStyle::SingleLine;
        textLine = t.mid(idx, idxOfNextLine);
    // TODO : add if clause for CommentStyle::Heavy
    } else {
        // The second line does not follow the first line with good format
        if(idx2 != idxOfNextLine + 2) // + 2 for `\n`
            ok = false;
        p.style = CommentStyle::Default;
        // Load the second line
        int idxOfThirdLine(t.indexOf("\n", idxOfNextLine));
        textLine = t.mid(idxOfNextLine + 2, idxOfThirdLine); // +2 for `\n`
    }
    // 3. Length
    // idxOfNextLine - idx + 1 represents the length of the first commented line
    p.length = idxOfNextLine - idx + 1;
    // 4. Filling char 1 : at position commentChar.size + 1 (space)
    QString firstLine(t.mid(idx, idxOfNextLine - idx));
    QString fillingChar(firstLine.at(commentChar.size() + 1));
    if(!COMMENTS_FILLING_CHARACTERS.contains(fillingChar))
        ok = false;
    p.fillingChar = fillingChar;
    // 5. Filling char 2
    // If style is SingleLine, set the same as filling char 1
    if(p.style == CommentStyle::SingleLine)
        p.fillingChar2 = p.fillingChar;
    // TODO: manage heavy of CommentStyle::Heavy
    else {  // CommentStyle::Default
        QString fillingChar2(textLine.at(commentChar.size() + 1)); // +1 for space
        p.fillingChar2 = fillingChar2;
    }
    // 6. Char on both sides
    // Look at the commentChar in a substring of textLine
    if(textLine.mid(commentChar.size()).indexOf(commentChar) == -1)
        p.commentCharBoth = false;
    else
        p.commentCharBoth = true;

    q(ok);
    // 7. Capitalize
    // Extract the text
    int idxTextStart(textLine.indexOf(p.fillingChar2 + " "));
    int idxTextEnd(textLine.indexOf(" " + p.fillingChar2, idxTextStart));
    if(idxTextStart == -1 || idxTextEnd == -1)
        ok = false;
    QString text(textLine.mid(idxTextStart, idxTextStart+1));
    q(text);
    q(ok);

    q(firstLine.at(commentChar.size() + 1));


    q(commentChar);
    q(idx);
    q(idxOfNextLine);
    q(t.mid(idx, idxOfNextLine - idx));
    q(idxOfNextLine - idx);

    return p;
}

// ==================================
// =============== ff ================
// ==================================
// ---------------


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






