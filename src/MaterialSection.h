#ifndef MATERIALSECTION_H
#define MATERIALSECTION_H

#include <QWidget>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QList>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleValidator>

// Structure to hold material configuration details
struct MaterialConfig {
    QString name;            // Name of the material
    int nozzleSize;          // Size of the nozzle
    double filamentAmount;   // Amount of filament
    double algo;             // algo
};

// MaterialSection class to manage material configurations
class MaterialSection : public QWidget {
    Q_OBJECT

public:
    // Constructor
    MaterialSection(QWidget *parent = nullptr);

    // Getter to retrieve the list of (materialConfig)
    QList<MaterialConfig> getMaterialConfigs() const;

    // Getter to retrieve the number of materials
    int getNumMaterials() const;

signals:
    // Signal emitted when any value changes
    void valueChanged();

private slots:
    // funtion to update the material names based on the number of materials
    void updateMaterialNames(int numMaterials);

    // funtion to update the material configurations
    void updateMaterialConfigs();

private:
    QSpinBox *numMaterialsSpinBox;        // Spin box to select number of materials
    QWidget *materialNamesContainer;      // Container widget for material names
    QVBoxLayout *materialNamesLayout;     // Layout to arrange material names
    QList<MaterialConfig> materialConfigs; // List to hold material configurations

    // styles
    void applyStyles();

    QLabel *sectionTitle;
    QLabel *headerLabel1;
    QLabel *headerLabel2;
    QLabel *headerLabel3;
    QLabel *headerLabel4;

public slots:
    void retranslateUi();
    void applyStyles(bool darkMode);

};

#endif // MATERIALSECTION_H
