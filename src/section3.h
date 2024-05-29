#ifndef SECTION3_H
#define SECTION3_H

#include <QWidget>
#include <QSpinBox>
#include <qlabel.h>
#include <qcombobox.h>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>

class Section3 : public QWidget {
    Q_OBJECT

public:
    explicit Section3(QWidget *parent = nullptr);
    void handleMethodSelection(int index);

    int getInfillValue() const;
    int getShapeIndex() const;
    double getStrandDistanceValue() const;

signals:
    void valueChanged();

private:
    QSpinBox *infillSpinBox;
    QComboBox *shapeComboBox;
    QLineEdit *strandDistanceLineEdit;

    QLabel *porcentaje;
    QLabel *milimetros;
};

#endif // SECTION3_H
