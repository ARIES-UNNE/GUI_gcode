#ifndef SECTION2_H
#define SECTION2_H

#include <QWidget>
#include <QSpinBox>
#include <qlabel.h>
#include <qcombobox.h>

class Section2 : public QWidget {
    Q_OBJECT

public:
    explicit Section2(QWidget *parent = nullptr);

    void handleShapeSelection(int index);

    int getSize1() const;
    int getSize2() const;
    int getShapeIndex1() const;
    int getShapeIndex2() const;


private:
    QLineEdit *sizeLineEdit;
    QLineEdit *sizeLineEdit2;
    QComboBox *shapeComboBox;
    QComboBox *shapeComboBox2;
    QLabel *shapeLabel;
};

#endif // SECTION2_H
