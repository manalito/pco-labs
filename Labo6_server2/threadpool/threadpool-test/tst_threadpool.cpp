#include <QString>
#include <QtTest>
#include <QTime>

#include "threadpool.h"
#include "tst_threadpool.h"


ThreadpoolTest::ThreadpoolTest()
{
}


void ThreadpoolTest::initTestCase()
{
    startingTime = QTime::currentTime();
}


void ThreadpoolTest::runnableStarted(QString id)
{
    // No concurrent access, no need to protect it
    m_runningState[id] = true;
}

void ThreadpoolTest::runnableTerminated(QString id)
{
    mutex.lock();
    // Erases the last value, protection between runnables is mandatory
    endingTime = QTime::currentTime();
    mutex.unlock();
    // Only a single access, no need to protect it
    m_runningState[id] = false;
}


void ThreadpoolTest::testCase1()
{
    initTestCase();
    ThreadPool pool(10);

    // Starts the runnables
    for(int i = 0; i < 10; i++) {
        QString runnableId = QString("Run%1").arg(i);
        TestRunnable *runnable = new TestRunnable(this, runnableId);
        runnableStarted(runnableId);
        pool.start(runnable);
    }

    QThread::usleep(1000 * (RUNTIMEINMS + 5));

    // Check that every runnable is really finished
    QMapIterator<QString, bool> i(m_runningState);
    while (i.hasNext()) {
        i.next();
        QVERIFY2(i.value() == false, "Failed");
    }

    QVERIFY2(startingTime.msecsTo(endingTime) < (RUNTIMEINMS + 5), "Too long execution time");

    QVERIFY2(startingTime.msecsTo(endingTime) > (RUNTIMEINMS - 2), "Too short execution time");
}

void ThreadpoolTest::testCase2()
{
    initTestCase();
    ThreadPool pool(10);

    // Starts the runnables
    for(int i = 0; i < 100; i++) {
        QString runnableId = QString("Run%1").arg(i);
        TestRunnable *runnable = new TestRunnable(this, runnableId);
        runnableStarted(runnableId);
        pool.start(runnable);
    }

    QThread::usleep(1000 * (10 * RUNTIMEINMS + 30));

    // Check that every runnable is really finished
    QMapIterator<QString, bool> i(m_runningState);
    while (i.hasNext()) {
        i.next();
        QVERIFY2(i.value() == false, "Failed");
    }

    QVERIFY2(startingTime.msecsTo(endingTime) < (10 * RUNTIMEINMS + 30), "Too long execution time");

    QVERIFY2(startingTime.msecsTo(endingTime) > (10 * RUNTIMEINMS - 30), "Too short execution time");
}

QTEST_APPLESS_MAIN(ThreadpoolTest)
