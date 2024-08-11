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
#include <QVector2D>

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
    void plateSizeChanged(const QVector2D &size);

private slots:
    // Slot to show/hide manual center input fields
    void showManualCenterInput(QLabel *labelX, QLabel *labelY);


private:
    // Spin boxes for plate dimensions and center coordinates
    QSpinBox *plateXSpinBox;
    QSpinBox *plateYSpinBox;
    QSpinBox *centerXSpinBox;
    QSpinBox *centerYSpinBox;

    // declaration for translating
    QLabel *sectionTitle;
    QLabel *labelTitle;
    QPushButton *manualCenterButton;

    void onPlateXChanged(int value) {
        emit plateSizeChanged(QVector2D(value, plateYSpinBox->value()));
    }

    void onPlateYChanged(int value) {
        emit plateSizeChanged(QVector2D(plateXSpinBox->value(), value));
    }

    // styles
    void applyStyles();

public slots:

    void updateValues(int plateX, int plateY, int centerX, int centerY);


    // Trasnlation
    void retranslateUi();
    void applyStyles(bool darkMode);

};

#endif // DIMENSIONSECTION_H
