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

const int DEFAULT_COMMENT_BLOCK_WIDTH = 40;
const QString DEFAULT_FILLING_CHAR("=");
const QString DEFAULT_COMMENT_STYLE("Medium");
const bool DEFAULT_FILLING_CHARS_LINKAGE(true);
const Language DEFAULT_LANGUAGE(Language::Python);

const QStringList COMMENTS_FILLING_CHARACTERS(QStringList() << "=" << "-" << "*"
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



#endif // SETUP_H
