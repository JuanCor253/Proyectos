#include "juego.h"
#include "./ui_juego.h"

juego::juego(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::juego)
    , serial(new QSerialPort(this))
{
    ui->setupUi(this);

    serial->setPortName("COM3");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Puerto serial abierto";
    } else {
        qDebug() << "Error al abrir puerto:" << serial->errorString();
    }

    // Conectar señal de lectura de datos
    connect(serial, &QSerialPort::readyRead, this, &juego::leerDatosSerial);

}

juego::~juego()
{
    delete ui;
}

void juego::on_bt_lanzar_clicked()
{

}

void juego::leerDatosSerial()
{
    QByteArray data = serial->readAll();
    bool ok;
    int valor = data.trimmed().toInt(&ok);
    if (ok) {
        ui->lcd_dado->display(valor);
        qDebug() << "Recibido:" << valor;
    } else {
        qDebug() << "Dato no válido:" << data;
    }
}
