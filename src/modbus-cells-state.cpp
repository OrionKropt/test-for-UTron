#include "modbus-cells-state.h"
#include <QVariant>

ModbusCellsState::ModbusCellsState() {}


void ModbusCellsState::setupModbusClient(QObject *parent,
                                         const QString & port,
                                         int baudRateCombo,
                                         QSerialPort::DataBits dataBits,
                                         QSerialPort::StopBits stopBits,
                                         QSerialPort::Parity noParity) {
    if (modbusClient) {
        delete modbusClient;
    }
    modbusClient = new QModbusRtuSerialClient(parent);

    modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter, port);
    modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,  baudRateCombo);
    modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, dataBits);
    modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopBits);
    modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter, noParity);

    modbusClient->setTimeout(1000);
    modbusClient->setNumberOfRetries(3);

    isConnect = modbusClient->connectDevice();
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

ModbusCellsState::~ModbusCellsState() {
    if (modbusClient) {
        modbusClient->disconnectDevice();
        delete modbusClient;
    }
}
