#ifndef SETUP_H
#define SETUP_H

#include <QString>

enum Language {
	Python, CPP
};

Language CURRENT_LANGUAGE = Language::Python;

QString get_comment_tokens() {
	switch(CURRENT_LANGUAGE):
	case Language::Python: return '#'; break;
	case Language::CPP: return '//'; break;
	default: return "ERROR"; break;
}

#endif // SETUP_H
