#pragma once

#include <QModbusRtuSerialClient>
#include <QSerialPort>
#include "cell-widget.h"
#include "logger.h"

class ModbusCellsState
{
private:
    QModbusRtuSerialClient *modbusClient;
    bool isConnect;
    Logger *logger;
public:
    ModbusCellsState(QObject *parent);
    void setupModbusClient(const QString & port,                 // Метод настройки клиента
                           int baudRateCombo,
                           QSerialPort::DataBits dataBits,
                           QSerialPort::StopBits stopBits,
                           QSerialPort::Parity noParity);
    bool isConnectToDevice() const noexcept;
    void disconnect() const;
    void updateCellsStates(CellWidget *cells);
    ~ModbusCellsState();
};

