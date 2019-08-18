#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QtTest>
#include "../CodeHelper/comments.h"
#include <memory>

class Test : public QObject
{
	Q_OBJECT
public:
	explicit Test(QObject *parent = nullptr);

private:
	std::unique_ptr<Comments> m_Comments;

public slots:

private slots:
	void dummyTest();

	void comments_block();
};

#endif // TEST_H
