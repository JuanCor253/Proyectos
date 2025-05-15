#include "juego.h"
#include "./ui_juego.h"

juego::juego(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::juego)
    , serial(new QSerialPort(this))
{
    ui->setupUi(this);

    serial->setPortName("/tmp/ttyV0");  // Depende del puerto a usar
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
    qDebug() << "Boton Lanzar presionado";
    char cmd = 0x01;
    serial->write(&cmd, 1);
}

void juego::on_bt_pasar_clicked()
{
    char cmd = 0x03;
    serial->write(&cmd, 1);
}

void juego::on_bt_reiniciar_clicked()
{
    char cmd = 0x05;
    serial->write(&cmd, 1);
}

void juego::leerDatosSerial()
{
    buffer.append(serial->readAll());

    while (buffer.size() >= 5) {  // Ahora paquete de 5 bytes
        QByteArray paquete = buffer.left(5);
        buffer.remove(0, 5);

        int estado = static_cast<unsigned char>(paquete[0]);
        int turno  = static_cast<unsigned char>(paquete[1]);
        int tiros  = static_cast<unsigned char>(paquete[2]);
        int dado   = static_cast<unsigned char>(paquete[3]);
        int score  = static_cast<unsigned char>(paquete[4]);

        if (estado == 0xAA) {
            QString mensaje = (turno == 1) ? "¡Jugador 1 ha ganado!" : "¡Jugador 2 ha ganado!";
            QMessageBox::information(this, "¡Victoria!", mensaje);
            continue;  // Puedes salir del ciclo o seguir recibiendo
        }

        // Actualizar UI con datos normales
        ui->lcd_tiros->display(tiros);
        ui->lcd_dado->display(dado);

        if (turno == 1) {
            ui->lcd_score1->display(score);
            ui->Score_1->setGraphicsEffect(nullptr);

            QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
            effect->setOpacity(0.3);
            ui->Score_2->setGraphicsEffect(effect);
        } else if (turno == 2) {
            ui->lcd_score2->display(score);
            ui->Score_2->setGraphicsEffect(nullptr);

            QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
            effect->setOpacity(0.3);
            ui->Score_1->setGraphicsEffect(effect);
        }
    }
}
