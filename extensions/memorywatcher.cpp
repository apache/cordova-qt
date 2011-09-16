#include "memorywatcher.h"

#include <e32std.h>

#include <QDebug>


MemoryWatcher::MemoryWatcher(QObject *parent) :
    QObject(parent),
    m_timer(),
    m_warningThreshold(75),
    m_criticalThreshold(90),
    m_state(NoAlertState) {

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(checkMemory()));
}

int MemoryWatcher::usage() const {

    return 100 * bytesUsed() / User::Heap().MaxLength();
}

int MemoryWatcher::bytesUsed() const {

    TInt largestBlock;
    return User::Available(largestBlock);
}

int MemoryWatcher::bytesFree() const {

    return User::Heap().MaxLength() - bytesUsed();
}

int MemoryWatcher::warningThreshold() const {

    return m_warningThreshold;
}

int MemoryWatcher::criticalThreshold() const {

    return m_criticalThreshold;
}

void MemoryWatcher::setWarningThreshold(int threshold) {

    m_warningThreshold = threshold;
}

void MemoryWatcher::setCriticalThreshold(int threshold) {

    m_criticalThreshold = threshold;
}

void MemoryWatcher::start() {

    m_timer.start(1000);
}

void MemoryWatcher::stop() {

    m_timer.stop();
}

void MemoryWatcher::checkMemory() {

    MemoryAlertState previousState = m_state;
    int use = usage();
    m_state = (use < m_warningThreshold) ? NoAlertState : (use < m_criticalThreshold) ? WarningState : CriticalState;

    if (previousState != m_state) {
        switch (m_state) {
            case CriticalState:
                qWarning() << "Memory usage critical: " << use << "%";
                emit critical(use);
                break;
            case WarningState:
                qWarning() << "Memory usage warning: " << use << "%";
                emit warning(use);
                break;
            default:
                qDebug() << "Memory usage back to normal: " << use << "%";
                break;
        }
    }
}
