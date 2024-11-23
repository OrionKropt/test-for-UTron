#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

class CellWidget : public QWidget {
    Q_OBJECT

public:
    explicit CellWidget(int cellId, QWidget *parent = nullptr);
    void simulateState();                    // Обновление состояния в эмуляции
    void updateStateFromModbus(int data);    // Обновление состояния на основе данных Modbus

private:
    int id;                                  // ID ячейки
    QLabel *statusLabel;                     // Отображение статуса
    QLabel *tegLabel;                        // Установлен ли ТЭГ
    QLabel *currentLabel;                    // Ток заряда
    QLabel *faultLabel;                      // Неисправности
    QLabel *shortCircuitLabel;               // Короткое замыкание
    QLabel *lightStatusLabel;                // Статус световой индикации
    QLabel *lightIndicator;                  // Индикатор цвета света
    QPushButton *openButton;                 // Открыть ячейку
    QPushButton *chargeButton;               // Включить/выключить заряд
    QPushButton *lockButton;                 // Заблокировать/разблокировать ячейку
    QPushButton *lightButton;                // Управление световой индикацией
    QString getLightStatus(int lightState);  // Текстовое описание состояния света

    int chargeCurrent;                       // Текущий ток заряда
    bool isTegInstalled;                     // Установлен ли ТЭГ
    bool isFaulty;                           // Наличие неисправностей
    bool isShortCircuit;                     // Наличие КЗ
    int lightState;                          // Состояние световой индикации (0 = выкл, 1 = синий, 2 = красный и т.д.)
    QTimer *lightTimer;                      // Таймер для мигания

    void updateUI();                         // Обновление интерфейса
    void updateLightIndicator();             // Обновление состояния светового индикатора

private slots:
    void onOpenClicked();
    void onChargeClicked();
    void onLockClicked();
    void onLightClicked();
    void onUpdateState();                    // Обновление состояния ячейки
    void onLightTimerTimeout();              // Обработчик для таймера мигания
};
