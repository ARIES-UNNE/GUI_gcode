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

private:
    QLineEdit *sizeLineEdit;
    QLineEdit *sizeLineEdit2;
    QComboBox *shapeComboBox;
    QComboBox *shapeComboBox2;
    QLabel *shapeLabel;
};

#endif // SECTION2_H
