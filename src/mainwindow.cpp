#include "mainwindow.h"
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSerialPort>
#include <QTimer>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isSimulation(true) {
    // Открытие файла для логов
    logFile.setFileName("modbus_log.txt");
    logFile.open(QIODevice::Append | QIODevice::Text);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Верхняя панель управления
    QHBoxLayout *controlPanel = new QHBoxLayout();
    QLabel *modeLabel = new QLabel("Режим работы:", this);
    modeSelector = new QComboBox(this);
    modeSelector->addItem("Эмуляция Modbus");
    modeSelector->addItem("Реальный Modbus");
    connect(modeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onModeChanged);

    modbusStatus = new QLabel("Текущий режим: Эмуляция", this);
    controlPanel->addWidget(modeLabel);
    controlPanel->addWidget(modeSelector);
    controlPanel->addWidget(modbusStatus);
    mainLayout->addLayout(controlPanel);

    // Настройки Modbus
    setupModbusUI(mainLayout);

    // Область прокрутки
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *gridContainer = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(gridContainer);

    // Добавляем 50 ячеек
    for (int i = 0; i < 50; ++i) {
        CellWidget *cell = new CellWidget(i + 1, this);
        cells.append(cell);
        gridLayout->addWidget(cell, i / 10, i % 10);
    }

    gridContainer->setLayout(gridLayout);
    scrollArea->setWidget(gridContainer);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);

    // Кнопка ручного обновления
    manualUpdateButton = new QPushButton("Обновить состояние ячеек", this);
    connect(manualUpdateButton, &QPushButton::clicked, this, &MainWindow::onManualUpdateClicked);
    mainLayout->addWidget(manualUpdateButton);

    setCentralWidget(centralWidget);
    setWindowTitle("Терминал обслуживания");
    resize(800, 600);

    QTimer *updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() {
        if (!isSimulation) {
            updateCellStates();
        }
    });
    updateTimer->start(1000);
}

MainWindow::~MainWindow() {
    if (modbusClient) {
        modbusClient->disconnectDevice();
        delete modbusClient;
    }
    logFile.close();
}

void MainWindow::setupModbusUI(QVBoxLayout *layout) {
    QGroupBox *modbusSettings = new QGroupBox("Настройки Modbus", this);
    QGridLayout *settingsLayout = new QGridLayout(modbusSettings);

    QLabel *portLabel = new QLabel("COM-порт:");
    portEdit = new QLineEdit("COM1", this);
    QLabel *baudRateLabel = new QLabel("Скорость:");
    baudRateCombo = new QComboBox(this);

    // Заполняем предопределённые скорости
    baudRateCombo->addItems({"9600", "19200", "38400", "57600", "115200"});

    applySettingsButton = new QPushButton("Применить", this);
    connect(applySettingsButton, &QPushButton::clicked, this, &MainWindow::onApplySettingsClicked);

    settingsLayout->addWidget(portLabel, 0, 0);
    settingsLayout->addWidget(portEdit, 0, 1);
    settingsLayout->addWidget(baudRateLabel, 1, 0);
    settingsLayout->addWidget(baudRateCombo, 1, 1);
    settingsLayout->addWidget(applySettingsButton, 2, 0, 1, 2);

    layout->addWidget(modbusSettings);
}

void MainWindow::onModeChanged(int index) {
    isSimulation = (index == 0);
    modbusStatus->setText(QString("Текущий режим: %1")
                              .arg(isSimulation ? "Эмуляция" : "Реальный Modbus"));

    if (!isSimulation && modbusClient == nullptr) {
        setupModbusClient();
    } else if (isSimulation && modbusClient) {
        modbusClient->disconnectDevice();
        delete modbusClient;
        modbusClient = nullptr;
        modbusStatus->setText("Соединение Modbus закрыто (Эмуляция)");
    }
}

void MainWindow::onApplySettingsClicked() {
    if (modbusClient) {
        modbusClient->disconnectDevice();
        setupModbusClient();
        logMessage("Modbus настройки применены.");
    }
}

void MainWindow::onManualUpdateClicked() {
    if (isSimulation) {
        updateCellStates();
        logMessage("Состояние ячеек обновлено вручную.");
    }
}

void MainWindow::updateCellStates() {
    if (isSimulation) {
        for (CellWidget *cell : cells) {
            cell->simulateState();
        }
    } else {
        // Вызов методов для чтения регистров
    }

}

void MainWindow::setupModbusClient() {
    if (modbusClient) {
        delete modbusClient;
    }
    modbusClient = new QModbusRtuSerialClient(this);

    modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter, portEdit->text());
    modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baudRateCombo->currentText().toInt());
    modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
    modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);

    modbusClient->setTimeout(2000);
    modbusClient->setNumberOfRetries(3);

    if (modbusClient->connectDevice()) {
        modbusStatus->setText("Modbus подключён");
    } else {
        modbusStatus->setText(QString("Ошибка подключения Modbus: %1").arg(modbusClient->errorString()));
    }
}

void MainWindow::logMessage(const QString &message) {
    QTextStream logStream(&logFile);
    logStream << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << " - " << message << "\n";
    logFile.flush();
}
