#ifndef JUEGO_H
#define JUEGO_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class juego;
}
QT_END_NAMESPACE

class juego : public QMainWindow
{
    Q_OBJECT

public:
    juego(QWidget *parent = nullptr);
    ~juego();

private slots:
    void on_bt_lanzar_clicked();
    void leerDatosSerial();

private:
    Ui::juego *ui;
    QSerialPort *serial;
};
#endif // JUEGO_H
