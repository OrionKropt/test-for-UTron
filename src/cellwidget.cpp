#include "cellwidget.h"
#include <QMessageBox>
#include <QTimer>

CellWidget::CellWidget(int cellId, QWidget *parent)
    : QWidget(parent), id(cellId), chargeCurrent(0), isTegInstalled(false),
      isFaulty(false), isShortCircuit(false), lightState(0) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Отображение ID ячейки
    QLabel *idLabel = new QLabel(QString("Ячейка #%1").arg(id), this);
    idLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(idLabel);

    // Состояния
    statusLabel = new QLabel("Состояние: Закрыто", this);
    tegLabel = new QLabel("ТЭГ: Не установлен", this);
    currentLabel = new QLabel("Ток заряда: 0 мА", this);
    faultLabel = new QLabel("Неисправности: Нет", this);
    shortCircuitLabel = new QLabel("КЗ: Нет", this);
    lightStatusLabel = new QLabel("Свет: Выкл", this); // Статус световой индикации

    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(tegLabel);
    mainLayout->addWidget(currentLabel);
    mainLayout->addWidget(faultLabel);
    mainLayout->addWidget(shortCircuitLabel);
    mainLayout->addWidget(lightStatusLabel);

    // Индикатор цвета света
    lightIndicator = new QLabel(this);
    lightIndicator->setFixedSize(30, 30);  // Размер индикатора
    lightIndicator->setStyleSheet("background-color: grey;"); // Изначально серый
    mainLayout->addWidget(lightIndicator);

    // Управляющие кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    openButton = new QPushButton("Открыть", this);
    chargeButton = new QPushButton("Заряд: Вкл", this);
    lockButton = new QPushButton("Замок: Закрыть", this);
    lightButton = new QPushButton("Свет: Выкл", this);

    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(chargeButton);
    buttonLayout->addWidget(lockButton);
    buttonLayout->addWidget(lightButton);
    mainLayout->addLayout(buttonLayout);

    // Подключение кнопок
    connect(openButton, &QPushButton::clicked, this, &CellWidget::onOpenClicked);
    connect(chargeButton, &QPushButton::clicked, this, &CellWidget::onChargeClicked);
    connect(lockButton, &QPushButton::clicked, this, &CellWidget::onLockClicked);
    connect(lightButton, &QPushButton::clicked, this, &CellWidget::onLightClicked);

    // Таймер для мигающей индикации
    lightTimer = new QTimer(this);
    connect(lightTimer, &QTimer::timeout, this, &CellWidget::onLightTimerTimeout);
}

void CellWidget::simulateState() {
    // Эмуляция изменения состояния
    isTegInstalled = !isTegInstalled;
    chargeCurrent = isTegInstalled ? (rand() % 500 + 100) : 0;
    isFaulty = (rand() % 10) < 3; // 30% вероятность неисправности
    isShortCircuit = (rand() % 10) < 2; // 20% вероятность КЗ

    // Световая индикация
    lightState = (lightState + 1) % 5;

    updateUI();
}

void CellWidget::updateStateFromModbus(int data) {
    // Пример обработки данных Modbus
    isTegInstalled = (data & 0x01);                // 1-й бит
    isFaulty = (data & 0x02);                      // 2-й бит
    isShortCircuit = (data & 0x04);                // 3-й бит
    chargeCurrent = (data >> 4) & 0xFF;            // Биты 4-11 для тока заряда
    lightState = (data >> 12) & 0x03;              // Биты 12-13 для света

    updateUI();
}

void CellWidget::updateUI() {
    statusLabel->setText(QString("Состояние: %1").arg(isTegInstalled ? "Открыто" : "Закрыто"));
    tegLabel->setText(QString("ТЭГ: %1").arg(isTegInstalled ? "Установлен" : "Не установлен"));
    currentLabel->setText(QString("Ток заряда: %1 мА").arg(chargeCurrent));
    faultLabel->setText(QString("Неисправности: %1").arg(isFaulty ? "Да" : "Нет"));
    shortCircuitLabel->setText(QString("КЗ: %1").arg(isShortCircuit ? "Да" : "Нет"));

    lightButton->setText(QString("Свет: %1").arg(getLightStatus(lightState)));

    // Обновление световой индикации
    updateLightIndicator();
}

QString CellWidget::getLightStatus(int lightState) {
    switch (lightState) {
    case 0: return "Выкл";
    case 1: return "Синий";
    case 2: return "Красный";
    case 3: return "Зелёный";
    case 4: return "Белый";
    default: return "Неизвестно";
    }
}

void CellWidget::updateLightIndicator() {
    switch (lightState) {
    case 0: lightIndicator->setStyleSheet("background-color: grey;"); break;
    case 1: lightIndicator->setStyleSheet("background-color: blue;"); break;
    case 2: lightIndicator->setStyleSheet("background-color: red;"); break;
    case 3: lightIndicator->setStyleSheet("background-color: green;"); break;
    case 4: lightIndicator->setStyleSheet("background-color: white;"); break;
    }
}

void CellWidget::onLightTimerTimeout() {
    lightState = (lightState + 1) % 5;
    updateLightIndicator();
}

void CellWidget::onLightClicked() {
    if (lightTimer->isActive()) {
        lightTimer->stop();
        lightButton->setText("Свет: Выкл");
    } else {
        lightTimer->start(500); // Мигать каждые 500 мс
        lightButton->setText("Свет: Мигает");
    }
}


void CellWidget::onOpenClicked() {
    isTegInstalled = !isTegInstalled;
    updateUI();
}

void CellWidget::onChargeClicked() {
    chargeCurrent = (chargeCurrent == 0) ? (rand() % 500 + 100) : 0;
    updateUI();
}

void CellWidget::onLockClicked() {
    QMessageBox::information(this, "Замок", "Замок ячейки переключён.");
}

void CellWidget::onUpdateState() {
    // Кнопка для ручного обновления состояния
    simulateState();
}
