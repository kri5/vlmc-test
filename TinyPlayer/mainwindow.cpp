#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow* MainWindow::window = NULL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    ui->pushButtonPlayPause->hide();
    ui->groupBoxPrevNext->hide();
    errorHandler = new QErrorMessage(this);
    MainWindow::window = this;
    connect(this, SIGNAL(eventNewFrameFired(struct ctx*)), this, SLOT(NewFrameEventFired(struct ctx*)));
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
        ui->pushButtonLaunch->setEnabled(false);
    else
        ui->pushButtonLaunch->setEnabled(true);
}

bool MainWindow::catchException()
{
    if (libvlc_exception_raised(&ex))
    {
        errorHandler->showMessage(libvlc_exception_get_message(&ex));
        libvlc_exception_clear(&ex);
        return true;
    }
    return false;
}

struct ctx
{
    int         media_id;
    uchar*      pixels;
    QMutex*     mutex;
    libvlc_exception_t    *ex;
    libvlc_instance_t *libvlc;
    libvlc_media_player_t *mp;
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

void MainWindow::NewFrameEventFired(struct ctx *ctx)
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
    libvlc_time_t t = libvlc_media_player_get_time(ctx->mp, ctx->ex);
    QTime dummyTime = QTime();
    QTime time = dummyTime.addMSecs(t);
    MainWindow::window->ui->labelTime->setText(time.toString("hh:mm:ss"));
}

void MainWindow::initVLC()
{
    char const *vlc_argv[] =
    {
        "-verbose", "3",
        //"--plugin-path", VLC_TREE "/modules",
        //"--ignore-config", /* Don't use VLC's config files */
    };
    int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

    libvlc_exception_init(&ex);
    this->libvlc = libvlc_new(vlc_argc, vlc_argv, &ex);
    catchException();
}

void MainWindow::on_pushButtonLaunch_clicked()
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
    this->m = libvlc_media_new(libvlc, currentMedia.toLocal8Bit(), &ex);
    if (catchException()) return;
    for (int i = 0; i < media_argc; i++ )
    {
        qDebug() << media_argv[i];
        libvlc_media_add_option(this->m, media_argv[i], &ex );
        if(catchException()) return;
    }
    this->mp = libvlc_media_player_new_from_media(this->m, &ex);
    qDebug() << "After mplayer new from media";
    libvlc_media_release(this->m);
    if (catchException()) return;
    media->ex = &this->ex;
    media->libvlc = this->libvlc;
    media->mp = this->mp;
    libvlc_media_player_play(this->mp, &ex);
    qDebug() << "After Play";
    if (catchException()) return;
    this->ui->pushButtonLaunch->hide();
    this->ui->pushButtonPlayPause->show();
}

void MainWindow::on_pushButtonPlayPause_clicked()
{
    if (this->ui->pushButtonPlayPause->text().compare("Play") == 0)
    {
        libvlc_media_player_play(this->mp, &ex);
        this->ui->groupBoxPrevNext->hide();
        this->ui->pushButtonPlayPause->setText("Pause");
   }
    else
    {
        libvlc_media_player_pause(this->mp, &ex);
        this->ui->groupBoxPrevNext->show();
        this->ui->pushButtonPlayPause->setText("Play");
    }
}

void MainWindow::on_pushButtonPrevious_clicked()
{
    libvlc_time_t t = libvlc_media_player_get_time(this->mp, &ex);
    float fps = 1.0f;//libvlc_media_player_get_fps(this->mp, &ex);
    float interval = (1.0f / fps) * 1000.0f;
    if ((t - interval) > 0.0f)
    {
        t -= (libvlc_time_t)interval;
        libvlc_media_player_set_time(this->mp, t, &ex);
    }
}

void MainWindow::on_pushButtonNext_clicked()
{
    libvlc_time_t t = libvlc_media_player_get_time(this->mp, &ex);
    float fps = 1.0f;//libvlc_media_player_get_fps(this->mp, &ex);
    float interval = (1.0f / fps) * 1000.0f;
    if ((t + interval) < libvlc_media_player_get_length(this->mp, &ex))
    {
        qDebug() << "in Check";
        t += (libvlc_time_t)interval;
        libvlc_media_player_set_time(this->mp, t, &ex);
    }
}
