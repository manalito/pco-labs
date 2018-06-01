#ifndef TST_THREADPOOL_H
#define TST_THREADPOOL_H

#include <QString>
#include <QtTest>
#include <QTime>

#include "threadpool.h"

#define RUNTIME 10000
#define RUNTIMEINMS 10

///
/// \brief The ThreadpoolTest class
/// This class embeds all the tests of the ThreadPool
class ThreadpoolTest : public QObject
{
    Q_OBJECT

public:
    ThreadpoolTest();

    ///
    /// \brief runnableStarted Called before sending the Runnable to the pool
    /// \param id Id of the Runnable
    ///
    void runnableStarted(QString id);

    ///
    /// \brief runnableTerminated Called when the runnable finishes
    /// \param id Id of the Runnable
    /// It has to be called by the Runnable itself, and record the ending time
    void runnableTerminated(QString id);

private:

    /// \brief m_runningState A map to represent the running state of the Runnables
    /// True : running. False : not running
    QMap<QString, bool> m_runningState;

    ///
    /// \brief startingTime To store the time the testcase started
    ///
    QTime startingTime;

    ///
    /// \brief endingTime To store the time of the end of the last Runnable
    ///
    QTime endingTime;

    //! A mutex to protect internal variables
    QMutex mutex;

    ///
    /// \brief initTestCase Function to init the testcase.
    /// Needs to be called at the beginning of each testcase.
    void initTestCase();

private Q_SLOTS:

    ///
    /// \brief testCase1 A testcase with a pool of 10 threads running 10 runnables
    /// Each runnable just waits 10 ms and finishes.
    /// Check is done on the termination of the Runnables (everyone finished),
    /// and on the time required to run all the runnables.
    ///
    void testCase1();

    ///
    /// \brief testCase2 A testcase with a pool of 10 threads running 100 runnables
    /// Each runnable just waits 10 ms and finishes.
    /// Check is done on the termination of the Runnables (everyone finished),
    /// and on the time required to run all the runnables.
    ///
    void testCase2();
};

///
/// \brief The TestRunnable class
/// This class is a subclass of Runnable, and therefore is used by the testcases
/// as Runnables
class TestRunnable : public Runnable
{
    //! A reference on the main Test class
    ThreadpoolTest *m_tester;

    //! The Id of the Runnable
    QString m_id;

public:

    ///
    /// \brief TestRunnable Simple constructor
    /// \param tester Reference to the main Test class
    /// \param id Id of the Runnable (should be unique)
    ///
    TestRunnable(ThreadpoolTest *tester, QString id) : m_tester(tester), m_id(id){
    }

    //! Empty destructor
    virtual ~TestRunnable() {}

    //! Method ran by the thread of the pool
    virtual void run() {
        QThread::usleep(RUNTIME);
        m_tester->runnableTerminated(m_id);
    }

    ///
    /// \brief id returns the Id of the Runnable
    /// \return The Id of the Runnable
    ///
    virtual QString id() {
        return m_id;
    }
};


#endif // TST_THREADPOOL_H
