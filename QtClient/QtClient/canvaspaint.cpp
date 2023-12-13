#include "canvaspaint.h"
#include "mainwindow.h"
#include "ui_canvaspaint.h"

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
    canvasPixmap = QPixmap(800, 600); // Ajustează dimensiunile după necesități
    canvasPixmap.fill(Qt::white); // Umple canvas-ul cu culoarea albă sau culoarea dorită
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


void CanvasPaint::on_LeaveServerButton_clicked()
{
    hide();
    obiect = new MainWindow(this);
    obiect->show();
}

