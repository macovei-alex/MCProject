#include "canvaspaint.h"
#include "mainwindow.h"
#include "ui_canvaspaint.h"
#include <QGuiApplication>
#include <QScreen>

CanvasPaint::CanvasPaint(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CanvasPaint),
    isDrawing(false),
    isErasing(false),
    isUndoing(false)
{
    ui->setupUi(this);

    QLabel* GameChatLabel = ui->GameChatLabel;
    GameChatLabel->setStyleSheet("border: none;");

    QWidget* gameChatWidget = ui->GameChat;
    gameChatWidget->setStyleSheet("QWidget { border: 1px solid black; }");

    QScreen* primaryScreen = QGuiApplication::primaryScreen();
    QRect screenRect = primaryScreen->geometry();
    setGeometry(0, 0, screenRect.width(), screenRect.height());
    setStyleSheet("QDialog { border: 2px solid black; }");
    canvasPixmap = QPixmap(screenRect.size()); // Ajustează dimensiunile după necesități
    canvasPixmap.fill(Qt::white); // Umple canvas-ul cu culoarea albă sau culoarea dorită
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
  //  connect(ui->minimize, SIGNAL(clicked()), this, SLOT(minimizeButtonClicked()));
    //connect(ui->minimize, &QPushButton::clicked, this, &CanvasPaint::minimizeButtonClicked);
    /*  connect(ui->minimize, SIGNAL(clicked()), this, SLOT(minimizeButtonClicked()));
    connect(ui->minimize, &QPushButton::clicked, this, &CanvasPaint::minimizeButtonClicked);*/


}
//void CanvasPaint::minimizeButtonClicked()
//{
//    qDebug() << "Minimize button clicked";
//    showMinimized();
//}

CanvasPaint::~CanvasPaint()
{
    delete ui;
}

void CanvasPaint::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPen pen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap,Qt::BevelJoin);
    painter.setPen(pen);
    QRect canvasRect=rect();
    painter.drawRect(canvasRect);
    painter.drawPixmap(2, 2, canvasPixmap);
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
            if (event->x() < width() * 3 / 4)
            {
                if (isDrawing)
                {
                    isDrawing = true;
                    isErasing = false;
                    isUndoing = false;
                }
                else if(isErasing)
                {
                    isDrawing = false;
                    isErasing = true;
                    isUndoing = false;
                }
                else
                {
                    isDrawing = false;
                    isErasing = false;
                    isUndoing = true;
                }
                lastPoint = event->pos();
            }
    }
}

// Adaugă o nouă metodă pentru gestionarea mișcării mouse-ului
void CanvasPaint::mouseMoveEvent(QMouseEvent* event)
{
    // Verifică dacă suntem în proces de desenare
    if (event->x() < width() * 3 / 4) {
        if (isDrawing || isErasing)
        {
            // Obține poziția curentă a cursorului
            QPoint currentPoint = event->pos();

            // Desenează o linie între poziția anterioară și cea curentă
            QPainter painter(&canvasPixmap);
            if (isDrawing)
            {
                painter.setPen(Qt::black);
                currentLine.isDrawing = true;
                currentLine.points.append(currentPoint);
            }
            else
            {
                painter.setPen(QPen(Qt::white, 20, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                currentLine.isDrawing = true;
                currentLine.points.append(currentPoint);
            }

            for (int i = 1; i < currentLine.points.size(); ++i) {
                painter.drawLine(currentLine.points[i - 1], currentLine.points[i]);
            }

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
        if(isDrawing)
        {
            isErasing = false;
            isUndoing = false;
            drawnLines.append(currentLine);
            currentLine.points.clear();
        }
        else if(isErasing)
        {
            isDrawing = false;
            isUndoing = false;
        }
        else if(isUndoing)
        {
            isDrawing = false;
            isErasing = false;
        }
    }
}

void CanvasPaint::resizeEvent(QResizeEvent* event)
{
// Creează o nouă imagine cu dimensiunile actualizate
	QPixmap newPixmap(event->size());
	newPixmap.fill(Qt::white);
	QPainter painter(&newPixmap);
    painter.drawPixmap(QRect(0, 0, event->size().width(), event->size().height()), canvasPixmap);
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
    isDrawing= true;
}

//void CanvasPaint::minimizeButtonClicked()
//{
//    qDebug() << "Minimize button clicked";
//	showMinimized();
//}

void CanvasPaint::on_DrawButton_clicked()
{
    isDrawing = true;
    isErasing = false;
    isUndoing = false;
}

void CanvasPaint::on_EraseButton_clicked()
{
    isDrawing = false;
    isErasing = true;
    isUndoing = false;
}

void CanvasPaint::on_UndoButton_clicked()
{
    isDrawing = false;
    isErasing = false;
    if (!drawnLines.isEmpty())
    {
        drawnLines.pop_back();
       // clearCanvas();
        QPixmap newPixmap(size());
        newPixmap.fill(Qt::white);


        QPainter painter(&newPixmap);
        for(const auto& line : drawnLines)
        {
            for (int i = 1; i < line.points.size(); ++i) {
                if (line.isDrawing)
                
                    painter.setPen(Qt::black);
               
                else 
                
                {
                    painter.setPen(QPen(Qt::white, 20, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                }
                painter.drawLine(line.points[i - 1], line.points[i]);
            }

        }
        canvasPixmap = newPixmap;

        update();
    }
}
