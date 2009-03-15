#include "mainwindow.h"
#include "ui_mainwindow.h"

#define mrl1 "/home/kri5/Dev/vlmc/TestingLibVLC/test.mov"

MainWindow* MainWindow::window = NULL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    errorHandler = new QErrorMessage(this);
    MainWindow::window = this;
    connect(this, SIGNAL(eventNewFrameFired(struct ctx*)), this, SLOT(on_NewFrameEventFired(struct ctx*)));
    initVLC();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionOpen_triggered()
{
    currentMedia = QFileDialog::getOpenFileName(this, tr("Open Media"), "/", tr("Video (*.avi *.mpg *.mov *.wmv *.mkv);;"
                                                                                "Audio (*.wav *.ogg *.mp3 *.wma);;"
                                                                                "All files (*.*)"));
    if (currentMedia.isEmpty())
        ui->pushButtonPlay->setEnabled(false);
    else
        ui->pushButtonPlay->setEnabled(true);
}

struct ctx
{
    int         media_id;
    uchar*      pixels;
    QMutex*     mutex;
};


void MainWindow::lock(struct ctx *ctx, void **pp_ret)
{
    ctx->mutex->lock();
    *pp_ret = ctx->pixels;
}

void MainWindow::unlock(struct ctx *ctx)
{
    MainWindow::window->emit eventNewFrameFired(ctx);
    ctx->mutex->unlock();
}

void MainWindow::on_NewFrameEventFired(struct ctx *ctx)
{
    QImage image(VIDEOWIDTH, VIDEOHEIGHT, QImage::Format_RGB32);

    int x, y;
    int* src = (int*)ctx->pixels;

    for (y = 0; y < VIDEOHEIGHT; y++)
    {
        for (x = 0; x < VIDEOWIDTH - 1; x++)
        {
            QRgb rgb = src[x];
            image.setPixel(x, y, rgb);
        }
        src += VIDEOWIDTH;
    }
    QPixmap pix = QPixmap::fromImage(image);
    MainWindow::window->ui->VideoLabel->setPixmap(pix);
}

void MainWindow::initVLC()
{
    char const *vlc_argv[] =
    {
        "-verbose", "3",
        //"--plugin-path", VLC_TREE "/modules",
        //"--ignore-config", /* Don't use VLC's config files */
    };
    int argc = sizeof(vlc_argv) / sizeof(*vlc_argv);
    this->_instance = new LibVLCpp::Instance(argc, vlc_argv);
}

void MainWindow::on_pushButtonPlay_clicked()
{
    struct ctx *media;
    media = (struct ctx*)malloc(sizeof(*media));
    media->pixels = (uchar*)malloc((sizeof(*(media->pixels)) * VIDEOHEIGHT * VIDEOWIDTH) * 4);
    media->mutex = new QMutex();

    char clock[64], cunlock[64], cdata[64];
    char width[32], height[32], pitch[32], chroma[32];
    if (currentMedia.isEmpty())
        return;

    sprintf(clock,   ":vmem-lock=%lld", (long long int)(intptr_t)lock);
    sprintf(cunlock, ":vmem-unlock=%lld", (long long int)(intptr_t)unlock);
    sprintf(cdata,   ":vmem-data=%lld", (long long int)(intptr_t)media);
    sprintf(width,   ":vmem-width=%i", VIDEOWIDTH);
    sprintf(height,  ":vmem-height=%i", VIDEOHEIGHT);
    sprintf(chroma,  ":vmem-chroma=%s", "RV32");
    sprintf(pitch,   ":vmem-pitch=%i", VIDEOWIDTH * 4);
    media->media_id = 1;

    char const *media_argv[] =
    {
        ":no-audio",
        ":vout=vmem",
        width,
        height,
        pitch,
        chroma,
        clock,
        cunlock,
        cdata
    };

    int media_argc = sizeof(media_argv) / sizeof(*media_argv);
    this->_media = new LibVLCpp::Media(this->_instance, currentMedia);

    for (int i = 0; i < media_argc; i++ )
    {
        qDebug() << media_argv[i];
        this->_media->addOption( media_argv[i] );
    }
    this->_mediaPlayer = new LibVLCpp::MediaPlayer(this->_media);
    delete this->_media;
    this->_mediaPlayer->play();
}
