#ifndef DIMENSIONSECTION_H
#define DIMENSIONSECTION_H

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


class DimensionSection : public QWidget {
    Q_OBJECT

public:
    // Constructor for DimensionSection class
    explicit DimensionSection(QWidget *parent = nullptr);

    // Getter methods for spin box values (for printing in the TXT)
    int getPlateXSpinBox() const;
    int getPlateYSpinBox() const;
    int getcenterXSpinBox() const;
    int getcenterYSpinBox() const;

signals:

    // Signal emitted when any value in the section changes
    void valueChanged();

private slots:
    // Slot to show/hide manual center input fields
    void showManualCenterInput(QLabel *labelX, QLabel *labelY);

private:
    // Spin boxes for plate dimensions and center coordinates
    QSpinBox *plateXSpinBox;
    QSpinBox *plateYSpinBox;
    QSpinBox *centerXSpinBox;
    QSpinBox *centerYSpinBox;

    // styles
    void applyStyles();

};

#endif // DIMENSIONSECTION_H
