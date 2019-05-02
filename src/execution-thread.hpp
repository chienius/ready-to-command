#pragma once
#include "main-window.hpp"
#include <thread>

class CommandExecutionThread
{
public:
    using MutexGuard = std::lock_guard<std::mutex>;

    CommandExecutionThread(MainWindow& mainWindow, const std::string& shellExec, int initSeconds=30);

    void delay(int seconds);
    void cancel();
    void triggerExecution();

    void joinAll() {
        countdownThread.join();
        if(executeThread.joinable()) executeThread.join();
    }

private:
    MainWindow& mainWindow;
    const std::string shellExec;

    std::thread countdownThread, executeThread;
    std::mutex countdownLock, executeLock;

    int countdown;
    bool countdownInterrupted;
    bool executed = false;

    void timerTask();
    void execute(bool skip=false);
};
