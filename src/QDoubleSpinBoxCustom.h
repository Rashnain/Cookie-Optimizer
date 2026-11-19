#ifndef QDOUBLESPINBOXCUSTOM_H
#define QDOUBLESPINBOXCUSTOM_H

#include <QDoubleSpinBox>

class QDoubleSpinBoxCustom : public QDoubleSpinBox {
public:
    QValidator::State validate(QString & text, int & pos) const override;
};

#endif //QDOUBLESPINBOXCUSTOM_H
