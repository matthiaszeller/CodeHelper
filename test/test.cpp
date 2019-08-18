#include "test.h"
#include <iostream>

Test::Test(QObject *parent) : QObject(parent),
	m_Comments(new Comments())
{

}

void Test::dummyTest() {
	QString t("ok");
	QCOMPARE(t, QString("ok"));
}

void Test::comments_block() {
	QString text("dummy");
	int width = 15;
	// Want this:
/*
123456789.....15 -> width 15
# ########### #
# ## dummy ## #	 -> t
# ########### #
 1  1     1  1   -> 4 spaces in middle column
*/
	std::cout << m_Comments->get_block_comment(text, width).toStdString() << std::endl;
}







