#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QProcess>
#include <QDebug>
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

private:
    // Pointer to the central widget
    QWidget *centralWidget;
    // Pointer to the QStackedWidget that manages different sections
    QStackedWidget *stackedWidget;
    // Pointer to the OpenGL widget
    OpenGLWidget *openGLWidget;

    // Pointers to the buttons in the main window
    QPushButton *generateButton;
    QPushButton *cancelButton;
    QPushButton *prevButton;
    QPushButton *nextButton;

    // Vertical layout to center the button
    QVBoxLayout *centeredButtonLayout;

    // Method to apply styles to the widgets
    void applyStyles();

    // Method to write configuration to a file
    bool writeConfigurationToFile(const QString &fileName, DimensionSection *DimensionSectionWidget, ShapeSection *ShapeSectionWidget, InfillSection *InfillSectionWidget, MaterialSection *MaterialSectionWidget);

private slots:
    // Funtons to move arround the sections
    void nextSection();
    void previousSection();
    void cancelConfirmation();

    // Method to adjust the section configuration
    void sectionConfiguration(int sectionIndex);

    // Method to save the configuration to a file
    void saveConfigurationToFile(DimensionSection *DimensionSectionWidget, ShapeSection *ShapeSectionWidget, InfillSection *InfillSectionWidget, MaterialSection *MaterialSectionWidget);
    // Method to update the GCODE in real-time
    void RealTimeGCODE(DimensionSection *DimensionSectionWidget, ShapeSection *ShapeSectionWidget, InfillSection *InfillSectionWidget, MaterialSection *MaterialSectionWidget);
    // Method to execute a Python script
    void executePython();
};

#endif // MAINWINDOW_H
