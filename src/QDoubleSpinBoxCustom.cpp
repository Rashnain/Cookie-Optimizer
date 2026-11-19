#include "QDoubleSpinBoxCustom.h"

QValidator::State QDoubleSpinBoxCustom::validate(QString &text, int &pos) const {
    text.replace(QStringLiteral("."), QStringLiteral(","));
    return QDoubleSpinBox::validate(text, pos);
}
