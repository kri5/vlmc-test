#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QErrorMessage>
#include <QFileDialog>
#include <QtDebug>
#include <QMutex>
#include "vlc/vlc.h"

#define VIDEOWIDTH 854
#define VIDEOHEIGHT 480

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static MainWindow* window;
    Ui::MainWindowClass *ui;

private:
    void initVLC();
    bool catchException();
    static void lock(struct ctx*, void**);
    static void unlock(struct ctx*);

    struct ctx* media;
    QErrorMessage* errorHandler;
    QString currentMedia;

    // libvlc
    libvlc_exception_t ex;
    libvlc_instance_t *libvlc;
    libvlc_media_t *m;
    libvlc_media_player_t *mp;

private slots:
    void on_pushButtonPlay_clicked();
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
    void NewFrameEventFired(struct ctx*);

signals:
    void eventNewFrameFired(struct ctx*);
};

#endif // MAINWINDOW_H
