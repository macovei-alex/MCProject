#include "canvaspaint.h"
#include "mainwindow.h"
#include "ui_canvaspaint.h"
#include <QGuiApplication>
#include <QScreen>

CanvasPaint::CanvasPaint(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CanvasPaint),
    isDrawing(false)
{
    ui->setupUi(this);

    QLabel* GameChatLabel = ui->GameChatLabel;
    GameChatLabel->setStyleSheet("border: none;");

    QWidget* gameChatWidget = ui->GameChat;
    gameChatWidget->setStyleSheet("QWidget { border: 1px solid black; }");

    QScreen* primaryScreen = QGuiApplication::primaryScreen();
    QRect screenRect = primaryScreen->geometry();
    setGeometry(0, 0, screenRect.width(), screenRect.height());

    canvasPixmap = QPixmap(screenRect.size()); // Ajustează dimensiunile după necesități
    canvasPixmap.fill(Qt::white); // Umple canvas-ul cu culoarea albă sau culoarea dorită
  /*  connect(ui->minimize, SIGNAL(clicked()), this, SLOT(minimizeButtonClicked()));
    connect(ui->minimize, &QPushButton::clicked, this, &CanvasPaint::minimizeButtonClicked);*/


}

CanvasPaint::~CanvasPaint()
{
    delete ui;
}
void CanvasPaint::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawPixmap(0, 0, canvasPixmap);
}

void CanvasPaint::on_Button_clicked()
{
    
        QPainter painter(&canvasPixmap);
        painter.drawLine(10, 10, 100, 100);

        // Actualizează afișarea
        update();
    
}
// Actualizează metoda mousePressEvent
void CanvasPaint::mousePressEvent(QMouseEvent* event)
{
    // Verifică dacă evenimentul este un clic al mouse-ului
    if (event->button() == Qt::LeftButton)
    {
        // Începe desenarea și reține poziția curentă a cursorului
        isDrawing = true;
        lastPoint = event->pos();
    }
}

// Adaugă o nouă metodă pentru gestionarea mișcării mouse-ului
void CanvasPaint::mouseMoveEvent(QMouseEvent* event)
{
    // Verifică dacă suntem în proces de desenare
    if (event->x() < width() * 3 / 4) {
        if (isDrawing)
        {
            // Obține poziția curentă a cursorului
            QPoint currentPoint = event->pos();

            // Desenează o linie între poziția anterioară și cea curentă
            QPainter painter(&canvasPixmap);
            painter.setPen(Qt::black);
            painter.drawLine(lastPoint, currentPoint);

            // Actualizează poziția anterioară
            lastPoint = currentPoint;

            // Actualizează afișarea
            update();
        }
    }
}

// Adaugă o nouă metodă pentru gestionarea eliberării butonului mouse-ului
void CanvasPaint::mouseReleaseEvent(QMouseEvent* event)
{
    // Verifică dacă butonul stâng al mouse-ului este eliberat
    if (event->button() == Qt::LeftButton)
    {
        // Oprește desenarea
        isDrawing = false;
    }
}

void CanvasPaint::resizeEvent(QResizeEvent* event)
{
// Creează o nouă imagine cu dimensiunile actualizate
	QPixmap newPixmap(event->size());
	newPixmap.fill(Qt::white);
	QPainter painter(&newPixmap);
	painter.drawPixmap(QPoint(0, 0), canvasPixmap);
	canvasPixmap = newPixmap;

	// Actualizează afișarea
	update();
}

void CanvasPaint::clearCanvas()
{
    // sterge ce s-a desenat
    canvasPixmap.fill(Qt::white);
    update();
}


void CanvasPaint::on_LeaveServerButton_clicked()
{
    hide();
    obiect = new MainWindow(this);
    obiect->show();
}



void CanvasPaint::on_ResetCanvas_clicked()
{
    clearCanvas();
}

//void CanvasPaint::minimizeButtonClicked()
//{
//    qDebug() << "Minimize button clicked";
//	showMinimized();
//}

