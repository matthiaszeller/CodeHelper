#ifndef SETUP_H
#define SETUP_H

#include <QStringList>
#include <iostream>
#include <QMap>
#include <QDebug>


enum Language { Python, CPP };
extern Language CURRENT_LANGUAGE;
QString map(Language);
Language map(QString);
//! Maps the enum Language
const QStringList LANGUAGE_COMMENT_STR(QStringList() << "#" << "//");

enum CommentStyle {
    SingleLine, Default, Heavy
};

CommentStyle map(int i); // TODO: change this, bad idea (used for language)

const int DEFAULT_COMMENT_LINE_NUMBER(3);
const int DEFAULT_COMMENT_BLOCK_WIDTH = 40;
const QString DEFAULT_FILLING_CHAR("=");
const int DEFAULT_COMMENT_STYLE(1);
const bool DEFAULT_FILLING_CHARS_LINKAGE(true);
const Language DEFAULT_LANGUAGE(Language::Python);

const bool DEFAULT_TOOLS_REGEXP(true);
const bool DEFAULT_TOOLS_SENSITIVE(false);

const QString FILENAME_SETTINGS("setup.ini");

const QStringList COMMENTS_FILLING_CHARACTERS(QStringList() << "=" << "-"
											  << "+" << "#" << " " << "<" << ">");

class TextHandler {
public:
	TextHandler() {

	}
	static QString get_comment_tokens();

	static QString get_filling_token();
};


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ------------------- DEBUGGING -------------------
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void q(const QString &t);
void q(int i);
void q(bool b);


#endif // SETUP_H
