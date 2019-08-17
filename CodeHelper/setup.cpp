#include "setup.h"

Language CURRENT_LANGUAGE = Language::Python;

QString TextHandler::get_comment_tokens() {
	switch(CURRENT_LANGUAGE) {
	case Language::Python: return "#"; break;
	case Language::CPP: return "//"; break;
	default: return "ERROR"; break;
	}
}

QString TextHandler::get_filling_token() {
	return "-";
}
