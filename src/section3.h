#ifndef SECTION3_H
#define SECTION3_H

#include <QWidget>
#include <QSpinBox>
#include <qlabel.h>
#include <qcombobox.h>

class Section3 : public QWidget {
    Q_OBJECT

public:
    explicit Section3(QWidget *parent = nullptr);
    void handleMethodSelection(int index);

private:
    QSpinBox *infillSpinBox;
    QComboBox *shapeComboBox;
    QLineEdit *strandDistanceLineEdit;
};

#endif // SECTION3_H
