#ifndef TEXTTOOLS_H
#define TEXTTOOLS_H

#include <QString>
#include "setup.h"

class TextTools
{
public:
	TextTools();

	static bool contains(const QString &text, const QString &pattern);

};

#endif // TEXTTOOLS_H
