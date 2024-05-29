#include "startpage.h"

SectionStart::SectionStart(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *welcomeLabel = new QLabel("Aplicación de impresión 3D", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("font-size: 24px; color: #333;");

    startButton = new QPushButton("Start", this);
    startButton->setStyleSheet("QPushButton {"
                               "background-color: #4CAF50;"
                               "color: white;"
                               "border: none;"
                               "border-radius: 5px;"
                               "padding: 10px 24px;"
                               "font-size: 16px;"
                               "}"
                               "QPushButton:hover { background-color: #45a049; }");

    connect(startButton, &QPushButton::clicked, this, &SectionStart::startButtonClicked);

    layout->addWidget(welcomeLabel);
    layout->addWidget(startButton);
    layout->setAlignment(startButton, Qt::AlignCenter);

    setLayout(layout);
}
