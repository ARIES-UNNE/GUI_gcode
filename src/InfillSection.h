#ifndef INFILLSECTION_H
#define INFILLSECTION_H

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>

// InfillSection class definition
class InfillSection : public QWidget {
    Q_OBJECT

public:
    // Constructor
    explicit InfillSection(QWidget *parent = nullptr);

    // funtion to handle changes in the selected infill method
    void handleMethodSelection(int index);

    // Getter for values
    int getInfillValue() const;
    int getShapeIndex() const;
    double getStrandDistanceValue() const;

signals:
    // Signal emitted when any value changes
    void valueChanged();

private:

    QSpinBox *infillSpinBox;            // Spin box for infill percentage
    QComboBox *shapeComboBox;           // Combo box for selecting shape
    QLineEdit *strandDistanceLineEdit;  // Line edit for strand distance

    QLabel *percentage;  // Label for percentage
    QLabel *millimeters;  // Label for millimeters

    QLabel *sectionTitle;
    QLabel *title;


    // styles
    void applyStyles();

public slots:
    void retranslateUi();
    void applyStyles(bool darkMode);
    void updateValues(int methodIndex, int infillValue, double strandDistance);

};

#endif // INFILLSECTION_H
