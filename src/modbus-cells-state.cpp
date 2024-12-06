#include "modbus-cells-state.h"
#include "cell-widget.h"
#include <QVariant>

ModbusCellsState::ModbusCellsState(QObject *parent) : modbusClient(nullptr), logger(Logger::instance()) {
    modbusClient = new QModbusRtuSerialClient(parent);
}


void ModbusCellsState::setupModbusClient(const QString & port,
                                         int baudRateCombo,
                                         QSerialPort::DataBits dataBits,
                                         QSerialPort::StopBits stopBits,
                                         QSerialPort::Parity noParity) {
    if(!modbusClient) return;
    if (modbusClient->state() != QModbusDevice::ConnectedState) {
        modbusClient->disconnectDevice();
    }

    modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter, port);
    modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,  baudRateCombo);
    modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, dataBits);
    modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopBits);
    modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter, noParity);

    modbusClient->setTimeout(1000);
    modbusClient->setNumberOfRetries(3);

    isConnect = modbusClient->connectDevice();
    if (!isConnect) {
        qDebug() << "Modbus настройки применены.";
        logger->logMessage(QString("Не удалось подключиться к последовательному порту: %1").arg(modbusClient->errorString()));
    } else {
        logger->logMessage(QString("Успешное подключение к последовательному порту"));
    }
}

bool ModbusCellsState::isConnectToDevice() const noexcept {
    return isConnect;
}

void ModbusCellsState::disconnect() const {
    if (modbusClient) {
        modbusClient->disconnectDevice();
        delete modbusClient;
    }
}

void ModbusCellsState::updateCellsStates(CellWidget *cells) {
    int startAddress = 0;
    QSize numberOfRegisters = cells->size();


}

ModbusCellsState::~ModbusCellsState() {
    if (modbusClient) {
        modbusClient->disconnectDevice();
        delete modbusClient;
    }
}
