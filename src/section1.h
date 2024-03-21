#ifndef SECTION1_H
#define SECTION1_H

#include <QWidget>
#include <QSpinBox>
#include <qlabel.h>

class Section1 : public QWidget {
    Q_OBJECT

public:
    explicit Section1(QWidget *parent = nullptr);

    QSpinBox *getPlateXSpinBox() const;
    QSpinBox *getPlateYSpinBox() const;
    void showManualCenterInput(QLabel *labelX, QLabel *labelY);

private:
    QSpinBox *plateXSpinBox;
    QSpinBox *plateYSpinBox;
    QSpinBox *centerXSpinBox;
    QSpinBox *centerYSpinBox;
};

#endif // SECTION1_H
