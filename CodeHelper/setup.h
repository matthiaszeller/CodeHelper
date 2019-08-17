#ifndef SETUP_H
#define SETUP_H

#include <QString>
#include <iostream>


enum Language { Python, CPP };
extern Language CURRENT_LANGUAGE;

class TextHandler {
public:
	TextHandler() {

	}
	static QString get_comment_tokens();

	static QString get_filling_token();
};

#endif // SETUP_H
