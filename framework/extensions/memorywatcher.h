#ifndef MEMORYWATCHER_H
#define MEMORYWATCHER_H

#include <QObject>
#include <QTimer>


class MemoryWatcher : public QObject {

    Q_OBJECT

    public:
        explicit MemoryWatcher(QObject *parent = 0);

        /**
         * @returns memory usage in percent (0 - 100)
         */
        int usage() const;

        /**
         * @returns the amount of memory in use in bytes
         */
        int bytesUsed() const;

        /**
         * @returns the amount of free memory in bytes
         */
        int bytesFree() const;

        /**
         * @returns the memory usage threshold for the 'warning' signal to be emmitted (0 - 100)
         */
        int warningThreshold() const;

        /**
         * @returns the memory usage threshold for the 'critical' signal to be emmitted (0 - 100)
         */
        int criticalThreshold() const;

        /**
         * Sets threshold for the 'warning' signal
         * @param threshold in percent (0 - 100)
         */
        void setWarningThreshold(int threshold);

        /**
         * Sets threshold for the 'critical' signal
         * @param threshold in percent (0 - 100)
         */
        void setCriticalThreshold(int threshold);

        Q_PROPERTY(int usage READ usage);
        Q_PROPERTY(int bytesUsed READ bytesUsed);
        Q_PROPERTY(int bytesFree READ bytesFree);
        Q_PROPERTY(int warningThreshold READ warningThreshold WRITE setWarningThreshold);
        Q_PROPERTY(int criticalThreshold READ criticalThreshold WRITE setCriticalThreshold);

    signals:
        void warning(int usage);
        void critical(int usage);

    public slots:
        /**
         * Start monitoring memory
         */
        void start();

        /**
         * Stop monitoring memory
         */
        void stop();

        /**
         * Check memory manually. Will emit 'warning' or 'critical' signal if any threshold is met
         */
        void checkMemory();

    private:
        enum MemoryAlertState { NoAlertState = 0, WarningState, CriticalState };

        QTimer m_timer;
        int m_warningThreshold;
        int m_criticalThreshold;
        MemoryAlertState m_state;
};

#endif // MEMORYWATCHER_H
