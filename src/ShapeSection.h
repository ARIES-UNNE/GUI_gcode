#ifndef SHAPESECTION_H
#define SHAPESECTION_H

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleValidator>

// ShapeSection class definition
class ShapeSection : public QWidget {
    Q_OBJECT

public:
    // Constructor
    explicit ShapeSection(QWidget *parent = nullptr);

    // funtion to handle shape and size selection changes
    void handleShapeSelection(int index);

    // Getter functions for size and shape values
    int getSize1() const;
    int getSize2() const;
    int getShapeIndex1() const;
    int getShapeIndex2() const;

signals:
    // Signal emitted when any value changes
    void valueChanged();

private:
    // Line edit widgets for size inputs and combo box widgets for shape selection
    QLineEdit *sizeLineEdit;
    QLineEdit *sizeLineEdit2;
    QComboBox *shapeComboBox;
    QComboBox *shapeComboBox2;

    QLabel *sectionTitle;
    QLabel *shapeLabel;
    QLabel *sizeLabel;

    // styles


public slots:
     void retranslateUi();
       void applyStyles(bool darkMode);

};

#endif // SHAPESECTION_H
