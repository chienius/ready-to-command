#include "execution-thread.hpp"
#include <mutex>

CommandExecutionThread::CommandExecutionThread(MainWindow& mainWindow, const std::string& shellExec, int initSeconds)
    : mainWindow(mainWindow), countdown(initSeconds),
      shellExec(shellExec) {
          countdownThread = std::thread(&CommandExecutionThread::timerTask, this);
      }

void CommandExecutionThread::delay(int seconds) {
    MutexGuard {countdownLock};
    countdown += seconds;
    mainWindow.updateCountdown(countdown);
}

void CommandExecutionThread::triggerExecution() {
    {
        MutexGuard{countdownLock};
        countdown = 0;
        mainWindow.updateCountdown(0);
        countdownInterrupted = true;
    }

    executeThread = std::thread(&CommandExecutionThread::execute, this, false);
}

void CommandExecutionThread::cancel() {
    {
        MutexGuard{countdownLock};
        mainWindow.updateCountdown(-1);
        countdownInterrupted = true;
    }

    executeThread = std::thread(&CommandExecutionThread::execute, this, true);
}

void CommandExecutionThread::timerTask() {
    while(countdown>=0) {
        {
            MutexGuard{countdownLock};
            if(countdownInterrupted) return;
            mainWindow.updateCountdown(countdown);
            if(countdown == 0) break;
        }
        sleep(1);
        {
            MutexGuard{countdownLock};
            countdown--;
        }
    }

    execute();
}

void CommandExecutionThread::execute(bool skip) {
    MutexGuard{countdownLock};
    if(!executed) {
        executed = true;
        if(!skip) ::system(shellExec.c_str());

        // Completed. Close window now.
        sleep(2);
        mainWindow.getWindow().close();
    }
}