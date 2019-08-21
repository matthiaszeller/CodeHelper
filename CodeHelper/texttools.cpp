#include "texttools.h"

TextTools::TextTools()
{

}


bool TextTools::contains(const QString &text, const QString &pattern) {
	q(pattern);

	return text.contains(QRegExp(pattern));
}
