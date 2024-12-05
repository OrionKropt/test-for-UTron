#pragma once

#include <QModbusRtuSerialClient>
#include <QSerialPort>


class ModbusCellsState
{
private:
    QModbusRtuSerialClient *modbusClient;
    bool isConnect;
public:
    ModbusCellsState();
    void setupModbusClient(QObject *parent,                 // Метод настройки клиента
                            const QString & port,
                            int baudRateCombo,
                            QSerialPort::DataBits dataBits,
                            QSerialPort::StopBits stopBits,
                            QSerialPort::Parity noParity);
    bool isConnectToDevice() const noexcept;
    void disconnect() const;
    ~ModbusCellsState();
};

