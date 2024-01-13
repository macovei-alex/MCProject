#ifndef CHOOSEWORDWINDOW_H
#define CHOOSEWORDWINDOW_H

#include <QDialog>


class CanvasPaint;

namespace Ui {
class choosewordwindow;
}

class choosewordwindow : public QDialog
{
    Q_OBJECT

public:
    explicit choosewordwindow(QWidget *parent = nullptr);
    ~choosewordwindow();
    void HandleCloseChoosingWindow();

private slots:
    void on_firstWordButton_clicked();

    void on_secondWordButton_clicked();

    void on_thirdWordButton_clicked();

signals:
    void Signal();

private:
    Ui::choosewordwindow *ui;
    CanvasPaint *canvasPaint;
    QStringList readWordsFromFile(const QString& filePath);
    QStringList loadRandomWords(int count);
    void LoadWords();
    void reloadWords();
    void openCanvasPaintWithWord(const QString& chosenWord);
   
};

#endif // CHOOSEWORDWINDOW_H
