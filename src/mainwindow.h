#pragma once

#include <QMainWindow>
#include <QVector>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QModbusRtuSerialClient>
#include "cell-widget.h"
#include "modbus-cells-state.h"
#include "logger.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    ModbusCellsState* mcs;
    QVector<CellWidget *> cells;                           // Массив всех ячеек
    const int cellsCount;
    QComboBox *modeSelector;                               // Переключатель режима
    QLabel *modbusStatus;                                  // Статус Modbus
    QComboBox *baudRateCombo;                              // Выбор скорости Modbus
    bool isSimulation;                                     // Флаг режима (эмуляция или реальный Modbus)
    QPushButton *applySettingsButton;
    QPushButton *manualUpdateButton;
    Logger *logger;

    // Поля и методы для настройки Modbus
    QLineEdit *portEdit;
    QLineEdit *baudRateEdit;

    void setupModbusUI(QVBoxLayout *layout);
    void connectModbusSignals();                           // Подключение сигналов для работы с Modbus
private slots:
    void onModeChanged(int index);
    void onApplySettingsClicked();
    void onManualUpdateClicked();
    void updateCellStates();                               // Обновление состояния ячеек
};

