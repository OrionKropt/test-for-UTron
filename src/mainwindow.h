#pragma once

#include <QMainWindow>
#include <QVector>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QModbusRtuSerialClient>
#include "cellwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QModbusRtuSerialClient *modbusClient;                  // Новый клиент Modbus
    QVector<CellWidget *> cells;                           // Массив всех ячеек
    QComboBox *modeSelector;                               // Переключатель режима
    QLabel *modbusStatus;                                  // Статус Modbus
    QComboBox *baudRateCombo;                              // Выбор скорости Modbus
    bool isSimulation;                                     // Флаг режима (эмуляция или реальный Modbus)
    QPushButton *applySettingsButton;
    QPushButton *manualUpdateButton;
    QFile logFile;

    // Поля и методы для настройки Modbus
    QLineEdit *portEdit;
    QLineEdit *baudRateEdit;

    void setupModbusUI(QVBoxLayout *layout);
    void setupModbusClient();                              // Метод настройки клиента
    void connectModbusSignals();                           // Подключение сигналов для работы с Modbus
    void logMessage(const QString &message);
private slots:
    void onModeChanged(int index);
    void onApplySettingsClicked();
    void onManualUpdateClicked();
    void updateCellStates();                               // Обновление состояния ячеек
};

