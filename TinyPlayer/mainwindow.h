#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QErrorMessage>
#include <QFileDialog>
#include <QtDebug>
#include <QMutex>

#include "VLCInstance.h"
#include "VLCMedia.h"
#include "VLCMediaPlayer.h"
#include <QTime>

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
    static void lock(LibVLCpp::Media::DataCtx* ctx, void**);
    static void unlock(LibVLCpp::Media::DataCtx* ctx);

//    struct ctx* media;
    QErrorMessage* errorHandler;
    QString currentMedia;

    // libvlc
    LibVLCpp::Instance*     _instance;
    LibVLCpp::Media*        _media;
    LibVLCpp::MediaPlayer*  _mediaPlayer;

private slots:
    void on_pushButtonNext_clicked();
    void on_pushButtonPrevious_clicked();
    void on_pushButtonPlayPause_clicked();
    void on_pushButtonLaunch_clicked();
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
    void NewFrameEventFired(LibVLCpp::Media::DataCtx*);

signals:
    void eventNewFrameFired(LibVLCpp::Media::DataCtx*);
};

#endif // MAINWINDOW_H
