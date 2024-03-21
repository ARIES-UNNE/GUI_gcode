// section4.h
#ifndef SECTION4_H
#define SECTION4_H

#include <QWidget>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QList>
#include <QLineEdit>

struct MaterialConfig {
    QString name;
    int nozzleSize;
    double filamentAmount;
};

class Section4 : public QWidget {
    Q_OBJECT

public:
    Section4(QWidget *parent = nullptr);

    QList<MaterialConfig> getMaterialConfigs() const;

private slots:
    void updateMaterialNames(int numMaterials);

private:
    QSpinBox *numMaterialsSpinBox;
    QWidget *materialNamesContainer;
    QVBoxLayout *materialNamesLayout;
    QList<MaterialConfig> materialConfigs;

};

#endif // SECTION4_H
