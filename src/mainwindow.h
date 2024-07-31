#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QProcess>
#include <QDebug>
#include <QTranslator>
#include <QFileDialog>
#include <QMainWindow>
#include <QFormLayout>
#include <QStackedWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QCoreApplication>
#include <QPushButton>
#include <QMessageBox>
#include <qlabel.h>
#include <QLineEdit>
#include <qsizegrip.h>
#include "DimensionSection.h"
#include "ShapeSection.h"
#include "InfillSection.h"
#include "MaterialSection.h"
#include "opengl.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    // Constructor for the MainWindow class
    MainWindow(QWidget *parent = nullptr);

    void applyStyles(bool darkMode);

private:
    // Pointer to the central widget
    QWidget *centralWidget;
    // Pointer to the QStackedWidget that manages different sections
    QStackedWidget *stackedWidget;
    // Pointer to the OpenGL widget
    OpenGLWidget *openGLWidget;

    // Pointers to the buttons in the main window
    QPushButton *cancelButton;
    QPushButton *prevButton;
    QTranslator translator;
    QPushButton *nextButton;
    QPushButton *toggleDarkModeButton;

    void applyDarkMode();
    void applyLightMode();

    void retranslateUi();
    void retranslateSection(QWidget* section);

    bool darkModeEnabled;




    // Method to write configuration to a file
    bool writeConfigurationToFile(const QString &fileName, DimensionSection *DimensionSectionWidget, ShapeSection *ShapeSectionWidget, InfillSection *InfillSectionWidget, MaterialSection *MaterialSectionWidget);

private slots:
    // Slot to move to the next section
    void nextSection();
    // Slot to move to the previous section
    void previousSection();
    // Slot to show a cancellation confirmation dialog
    void cancelConfirmation();
    void toggleDarkMode();
    // Slot to adjust the section configuration
    void sectionConfiguration(int sectionIndex);

    // Slot to save the configuration to a file
    void saveConfigurationToFile(DimensionSection *DimensionSectionWidget, ShapeSection *ShapeSectionWidget, InfillSection *InfillSectionWidget, MaterialSection *MaterialSectionWidget);
    // Slot to update the GCODE in real-time
    void RealTimeGCODE(DimensionSection *DimensionSectionWidget, ShapeSection *ShapeSectionWidget, InfillSection *InfillSectionWidget, MaterialSection *MaterialSectionWidget);
    // Slot to execute a Python script
    void executePython();

    void changeLanguage(const QString &languageCode);



};

#endif // MAINWINDOW_H
