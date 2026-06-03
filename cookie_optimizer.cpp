#include "cookie_optimizer.h"

CookieOptimizer::CookieOptimizer(QWidget *parent) : QWidget(parent)
{
    gridLayout = new QGridLayout(this);

    gridLayout->setColumnMinimumWidth(5, 75);
    gridLayout->setColumnMinimumWidth(6, 75);
    gridLayout->setColumnMinimumWidth(7, 75);

    gridLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::Expanding), 100, 0);

    if (!load()) {
        addRow();
        addRow();
    }

    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

CookieOptimizer::~CookieOptimizer() {
    save();

    delete gridLayout;

    delete valueSpinBox;
    delete valueComboBox;
    delete productionSpinBox;
    delete productionComboBox;
    delete minuteLabel;

    for (int i = 0; i < priceSpinBox.size(); i++) {
        delete priceSpinBox[i];
        delete priceComboBox[i];
        delete cpsSpinBox[i];
        delete cpsComboBox[i];
        delete amountSpinBox[i];
        delete priceLabel[i];
        delete cpsLabel[i];
        delete ratioLabel[i];
        delete buyButton[i];
        delete removeButton[i];
    }
}

void CookieOptimizer::addRow()
{
    const int index = int(priceSpinBox.size());

    constexpr int frameStyle = QFrame::Sunken | QFrame::Panel;

    const QStringList units{tr("K"), tr("M"), tr("B"), tr("T"), tr("Qa"), tr("Qi"),
            tr("Sx"), tr("Sp"), tr("O"), tr("N"), tr("D")};

    if (index == 0) {
        valueSpinBox = new QDoubleSpinBoxCustom();
        valueSpinBox->setMaximum(9999);
        valueSpinBox->setDecimals(3);

        valueComboBox = new QComboBox();
        valueComboBox->addItems(units);
        valueComboBox->setCurrentIndex(0);

        productionSpinBox = new QDoubleSpinBoxCustom();
        productionSpinBox->setMaximum(9999);
        productionSpinBox->setDecimals(3);

        productionComboBox = new QComboBox();
        productionComboBox->addItems(units);
        productionComboBox->setCurrentIndex(0);

        minuteLabel = new QLabel();
        minuteLabel->setFrameStyle(frameStyle);

        percentLabel = new QLabel();
        percentLabel->setFrameStyle(frameStyle);

        connect(valueSpinBox, &QDoubleSpinBox::valueChanged, this, &CookieOptimizer::updateResults);
        connect(valueComboBox, &QComboBox::currentIndexChanged, this, &CookieOptimizer::updateResults);
        connect(productionSpinBox, &QDoubleSpinBox::valueChanged, this, &CookieOptimizer::updateResults);
        connect(productionComboBox, &QComboBox::currentIndexChanged, this, &CookieOptimizer::updateResults);

        gridLayout->addWidget(valueSpinBox, 0, 0);
        gridLayout->addWidget(valueComboBox, 0, 1);
        gridLayout->addWidget(productionSpinBox, 0, 2);
        gridLayout->addWidget(productionComboBox, 0, 3);
        gridLayout->addWidget(minuteLabel, 0, 6);
        gridLayout->addWidget(percentLabel, 0, 7);
    }

    priceSpinBox.push_back(new QDoubleSpinBoxCustom());
    priceSpinBox[index]->setMaximum(9999);
    priceSpinBox[index]->setDecimals(3);

    priceComboBox.push_back(new QComboBox());
    priceComboBox[index]->addItems(units);
    priceComboBox[index]->setCurrentIndex(valueComboBox->currentIndex());

    cpsSpinBox.push_back(new QDoubleSpinBoxCustom());
    cpsSpinBox[index]->setMaximum(9999);
    cpsSpinBox[index]->setDecimals(3);

    cpsComboBox.push_back(new QComboBox());
    cpsComboBox[index]->addItems(units);
    cpsComboBox[index]->setCurrentIndex(productionComboBox->currentIndex());

    amountSpinBox.push_back(new QSpinBox());
    amountSpinBox[index]->setMinimum(1);

    priceLabel.push_back(new QLabel());
    priceLabel[index]->setFrameStyle(frameStyle);

    cpsLabel.push_back(new QLabel());
    cpsLabel[index]->setFrameStyle(frameStyle);

    ratioLabel.push_back(new QLabel());
    ratioLabel[index]->setFrameStyle(frameStyle);

    buyButton.push_back(new QPushButton(tr("Buy")));
    buyButton[index]->setMaximumWidth(75);

    if (index == 0)
        removeButton.push_back(new QPushButton(tr("+")));
    else
        removeButton.push_back(new QPushButton(tr("x")));
    removeButton[index]->setMaximumWidth(50);

    connect(priceSpinBox[index], &QDoubleSpinBox::valueChanged, this, &CookieOptimizer::updateResults);
    connect(priceComboBox[index], &QComboBox::currentIndexChanged, this, &CookieOptimizer::updateResults);
    connect(cpsSpinBox[index], &QDoubleSpinBox::valueChanged, this, &CookieOptimizer::updateResults);
    connect(cpsComboBox[index], &QComboBox::currentIndexChanged, this, &CookieOptimizer::updateResults);
    connect(amountSpinBox[index], &QSpinBox::valueChanged, this, &CookieOptimizer::updateResults);
    connect(buyButton[index], &QAbstractButton::clicked, this, [this, index]{ buy(index); });

    if (index == 0)
        connect(removeButton[index], &QAbstractButton::clicked, this, &CookieOptimizer::addRow);
    else
        connect(removeButton[index], &QAbstractButton::clicked, this, [this, index]{ removeRow(index); });

    gridLayout->addWidget(priceSpinBox[index], index + 1, 0);
    gridLayout->addWidget(priceComboBox[index], index + 1, 1);
    gridLayout->addWidget(cpsSpinBox[index], index + 1, 2);
    gridLayout->addWidget(cpsComboBox[index], index + 1, 3);
    gridLayout->addWidget(amountSpinBox[index], index + 1, 4);
    gridLayout->addWidget(priceLabel[index], index + 1, 5);
    gridLayout->addWidget(cpsLabel[index], index + 1, 6);
    gridLayout->addWidget(ratioLabel[index], index + 1, 7);
    gridLayout->addWidget(buyButton[index], index + 1, 8);
    gridLayout->addWidget(removeButton[index], index + 1, 9);
}

void CookieOptimizer::updateResults() const
{
    char *buffer = new char[100];

    const double value = valueSpinBox->value();
    const int valueIndex = valueComboBox->currentIndex();
    const double valueMult = std::pow(10, 3 * (valueIndex + 1));
    const double prod = productionSpinBox->value();
    const int prodIndex = productionComboBox->currentIndex();
    const double prodMult = std::pow(10, 3 * (prodIndex + 1));

    if (value != 0 && prod != 0) {
        const double time = value * valueMult / (prod * prodMult);
        sprintf(buffer, "%.1f m", time / 60);
        minuteLabel->setText(buffer);
        sprintf(buffer, "%.3f", prod / 100);
        percentLabel->setText(buffer);
    }

    double min = 1e9, max = 0;
    double ratios[priceSpinBox.size()];

    for (int i = 0; i < priceSpinBox.size(); i++) {
        if (priceSpinBox[i] == nullptr) continue;
        double price = priceSpinBox[i]->value();
        const int priceIndex = priceComboBox[i]->currentIndex();
        const double priceMult = std::pow(10, 3 * (priceIndex + 1));
        double cps = cpsSpinBox[i]->value();
        const int cpsIndex = cpsComboBox[i]->currentIndex();
        const double cpsMult = std::pow(10, 3 * (cpsIndex + 1));
        const int amount = amountSpinBox[i]->value();
        if (price != 0 && cps != 0) {
            price = sumPrice(price, amount);
            cps *= amount;
            sprintf(buffer, "%.3f", price);
            priceLabel[i]->setText(buffer);
            sprintf(buffer, "%.3f", cps);
            cpsLabel[i]->setText(buffer);
            const double ratio = cps * cpsMult / (price * priceMult);
            ratios[i] = ratio;
            if (ratio < min)
                min = ratio;
            if (ratio > max)
                max = ratio;
        } else
            ratios[i] = -1;
    }

    constexpr int color_min[] = {170, 0};
    constexpr int color_max[] = {0, 170};
    const double diff_min_max = max - min != 0.0 ? max - min : 1.0;
    for (int i = 0; i < priceSpinBox.size(); i++) {
        if (priceSpinBox[i] == nullptr) continue;
        if (ratios[i] == -1) continue;
        double ratio_diff = (ratios[i] - min) / diff_min_max;
        double x = color_min[0] * (1 - ratio_diff) + color_max[0] * ratio_diff;
        double y = color_min[1] * (1 - ratio_diff) + color_max[1] * ratio_diff;
        double text = ratio_diff > 0.5 ? 0 : 255;
        sprintf(buffer, "%.1f %%", ratio_diff * 100);
        ratioLabel[i]->setText(buffer);
        ratioLabel[i]->setStyleSheet("QLabel { background-color : rgb("
            + tr(std::to_string(int(x)).c_str())
            + ", "
            + tr(std::to_string(int(y)).c_str())
            + ", 0); "
            + "color : rgb("
            + tr(std::to_string(int(text)).c_str())
            + ", "
            + tr(std::to_string(int(text)).c_str())
            + ", "
            + tr(std::to_string(int(text)).c_str())
            + "); }"
        );
    }
    delete[] buffer;
}

void CookieOptimizer::buy(const int index) const
{
    if (priceSpinBox[index]->value() != 0) {
        priceSpinBox[index]->setValue(priceSpinBox[index]->value() * 1.15);
        amountSpinBox[index]->setValue(amountSpinBox[index]->value() - 1);
    }
}

template<typename T>
void CookieOptimizer::removeItem(std::vector<T>& vec, int row)
{
    gridLayout->removeWidget(vec[row]);
    delete vec[row];
    vec[row] = nullptr;
}

void CookieOptimizer::removeRow(const int row)
{
    removeItem(priceSpinBox, row);
    removeItem(priceComboBox, row);
    removeItem(cpsSpinBox, row);
    removeItem(cpsComboBox, row);
    removeItem(amountSpinBox, row);
    removeItem(priceLabel, row);
    removeItem(cpsLabel, row);
    removeItem(ratioLabel, row);
    removeItem(buyButton, row);
    removeItem(removeButton, row);
    updateResults();
}

double CookieOptimizer::sumPrice(const double price, const int amount)
{
    if (amount == 1)
        return price;
    return price + sumPrice(price * 1.15, amount-1);
}

void CookieOptimizer::save() const {
    const double value = valueSpinBox->value();
    const int valueIndex = valueComboBox->currentIndex();
    const double prod = productionSpinBox->value();
    const int prodIndex = productionComboBox->currentIndex();

    std::ofstream file("cookie_optimizer.csv");
    int row = 0;
    for (auto price_spin_box : priceSpinBox)
        if (price_spin_box != nullptr)
            row++;
    file << value << " " << valueIndex << " " << prod << " " << prodIndex << " " << row << std::endl;

    for (int i = 0; i < priceSpinBox.size(); i++) {
        if (priceSpinBox[i] == nullptr) continue;
        const double price = priceSpinBox[i]->value();
        const int priceIndex = priceComboBox[i]->currentIndex();
        const double cps = cpsSpinBox[i]->value();
        const int cpsIndex = cpsComboBox[i]->currentIndex();
        const int amount = amountSpinBox[i]->value();
        file << price << " " << priceIndex << " " << cps << " " << cpsIndex << " " << amount << std::endl;
    }

    file.close();
}

bool CookieOptimizer::load() {
    std::ifstream file("cookie_optimizer.csv");
    if (!file.good()) return false;

    double price;
    int priceMult;
    double cps;
    int cpsMult;
    int row;
    int amount;

    addRow();

    file >> price >> priceMult >> cps >> cpsMult >> row;
    valueSpinBox->setValue(price);
    valueComboBox->setCurrentIndex(priceMult);
    productionSpinBox->setValue(cps);
    productionComboBox->setCurrentIndex(cpsMult);

    for (int i = 0; i < row ; i++) {
        file >> price >> priceMult >> cps >> cpsMult >> amount;
        priceSpinBox[i]->setValue(price);
        priceComboBox[i]->setCurrentIndex(priceMult);
        cpsSpinBox[i]->setValue(cps);
        cpsComboBox[i]->setCurrentIndex(cpsMult);
        amountSpinBox[i]->setValue(amount);

        if (i + 1 < row)
            addRow();
    }

    file.close();
    return true;
}
