MainWindow::MainWindow(QWidget *parent) :
                       QMainWindow(parent),
                       ui(new Ui::MainWindow)
{
    /* only one instance of application should be running at a time */
    if (isAppInstanceRunning("SomeName"))
    {
        QMessageBox::information(NULL, "Check Application Instance",
                                 "Application is already running.\n\n"
                                 "You cannot run multiple instances of this application");
        QTimer::singleShot(10, qApp, SLOT(quit()));
    }
}

startLocalServer("SomeName");

/**
 * @brief determine if an instance of this application is already running;

 * we do this by attempting connection to a specified server; if connection
 * succeedes, an instance is already running
 *
 * @param  serverName  name of listening server
 *
 * @return true if instance is running, false otherwise
 ******************************************************************************/

bool MainWindow::isAppInstanceRunning(QString serverName)
{
    QLocalSocket socket;

    socket.connectToServer(serverName);
    if (socket.waitForConnected(500))
    {
        socket.disconnectFromServer();
        return true;
    }

    return false;
}

/**
 * start listening for local socket connections
 *
 * in Unix, this is implemented as Unix Domain Socket
 * in Windows, this is implemented as a pipe
 *
 *  @param  serverName  name of listening server
 ******************************************************************************/

void MainWindow::startLocalServer(QString serverName)
{
    localServer = new QLocalServer(this);

    connect(localServer, SIGNAL(newConnection()),
            this, SLOT(newLocalSocketConnection()));

    localServer->listen(serverName);
}

/**
 * @brief called when a local socket connects to our server
 ******************************************************************************/

void MainWindow::newLocalSocketConnection()
{
}
