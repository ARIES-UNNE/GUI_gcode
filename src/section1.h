#ifndef SECTION1_H
#define SECTION1_H

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>


class Section1 : public QWidget {
    Q_OBJECT

public:
    explicit Section1(QWidget *parent = nullptr);

    int getPlateXSpinBox() const;
    int getPlateYSpinBox() const;
    int getcenterXSpinBox() const;
    int getcenterYSpinBox() const;

signals:
    void valueChanged();

private slots:
    void showManualCenterInput(QLabel *labelX, QLabel *labelY);

private:
    QSpinBox *plateXSpinBox;
    QSpinBox *plateYSpinBox;
    QSpinBox *centerXSpinBox;
    QSpinBox *centerYSpinBox;
};

#endif // SECTION1_H
