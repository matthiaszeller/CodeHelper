#include "setup.h"

Language CURRENT_LANGUAGE = Language::Python;
bool DEBUGGING_MODE(true);

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

QString map(Language l) {
	switch(l) {
	case Python: return "Python"; break;
	default: return "CPP"; break;
	}
}

Language map(QString t) {
	if(t == "Python")
		return Python;
	return CPP;
}

CommentStyle map(int i) {
    switch(i) {
    case 0: return SingleLine; break;
    case 1: return Default; break;
    default: return Heavy; break;
    }
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ------------------- DEBUGGING -------------------
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void q(const QString &t) {
	if(!DEBUGGING_MODE)
		return;
    qDebug() << t;
    qDebug();
}

void q(int i) {
    if(!DEBUGGING_MODE)
        return;
	qDebug() << i;
}

void q(bool b) {
    if(!DEBUGGING_MODE)
        return;
    qDebug() << b;
}




