#ifndef SECTION4_H
#define SECTION4_H

#include <QWidget>
#include <QSpinBox>
#include <qlabel.h>
#include <qcombobox.h>

class Section4 : public QWidget {
    Q_OBJECT

public:
    explicit Section4(QWidget *parent = nullptr);


private:
    QSpinBox *numMaterialsSpinBox;
};

#endif // SECTION4_H
