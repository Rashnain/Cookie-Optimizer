#ifndef COOKIE_OPTIMIZER_H
#define COOKIE_OPTIMIZER_H

#include <QGuiApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

#include "QDoubleSpinBoxCustom.h"

class CookieOptimizer : public QWidget {
public:
    explicit CookieOptimizer(QWidget *parent = nullptr);
    ~CookieOptimizer() override;

private slots:
    void addRow();
    void updateResults() const;
    void buy(int index) const;
    void removeRow(int row);

    static double sumPrice(double price, int amount);

private:
    QGridLayout *gridLayout;

    QDoubleSpinBoxCustom *valueSpinBox;
    QComboBox *valueComboBox;
    QDoubleSpinBoxCustom *productionSpinBox;
    QComboBox *productionComboBox;
    QLabel *secondesLabel;
    QLabel *minutesLabel;

    std::vector<QDoubleSpinBoxCustom *> priceSpinBox;
    std::vector<QComboBox *> priceComboBox;
    std::vector<QDoubleSpinBoxCustom *> cpsSpinBox;
    std::vector<QComboBox *> cpsComboBox;
    std::vector<QSpinBox *> amountSpinBox;
    std::vector<QLabel *> priceLabel;
    std::vector<QLabel *> cpsLabel;
    std::vector<QLabel *> ratioLabel;
    std::vector<QPushButton *> buyButton;
    std::vector<QPushButton *> removeButton;
};

#endif
