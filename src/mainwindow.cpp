#include "mainwindow.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QPalette>
#include <QColor>
#include <QFile>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    this->setWindowIcon(QIcon());

    QFile file(":/styles/style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        this->setStyleSheet(stream.readAll());
        file.close();
    }

    resize(400, 500);
    setMinimumSize(400, 500);
    setMaximumSize(400, 500);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QLineEdit* display = new QLineEdit();
    mainLayout->addWidget(display, 1);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    display->setFocusPolicy(Qt::NoFocus);

    QGridLayout* buttonsLayout = new QGridLayout();
    mainLayout->addLayout(buttonsLayout, 5);

    const QString buttonLabels[4][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"0", "C", "=", "+"}
    };

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {

            QString text = buttonLabels[row][col];

            QPushButton* button = new QPushButton(text);
            buttonsLayout->addWidget(button, row, col);

            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


            connect(button, &QPushButton::clicked, [this, button, display]() {
                QString buttonText = button->text();

                if (buttonText == "C") {
                    display->setText("0");
                    this->firstNumber = 0.0;
                    this->operation = "";
                    this->isWaitingForOperand = false;
                }

                else if (buttonText == "+" || buttonText == "-" || buttonText == "*" || buttonText == "/") {
                    this->firstNumber = display->text().toDouble();
                    this->operation = buttonText;
                    this->isWaitingForOperand = true;
                }

                else if (buttonText == "=") {
                    double secondNumber = display->text().toDouble();
                    double result = 0.0;

                    if (this->operation == "+") result = this->firstNumber + secondNumber;
                    else if (this->operation == "-") result = this->firstNumber - secondNumber;
                    else if (this->operation == "*") result = this->firstNumber * secondNumber;
                    else if (this->operation == "/") {
                        if (secondNumber != 0.0) {
                            result = this->firstNumber / secondNumber;
                        } else {
                            display->setText("Ошибка (деление на 0)");
                            this->isWaitingForOperand = true;
                            return;
                        }
                    }


                    display->setText(QString::number(result));
                    this->isWaitingForOperand = true;
                }

                else {

                    if (this->isWaitingForOperand) {
                        display->clear();
                        this->isWaitingForOperand = false;
                    }

                    QString currentText = display->text();
                    if (currentText == "0") {
                        display->setText(buttonText);
                    } else {
                        display->setText(currentText + buttonText);
                    }
                }
            });


        }
    }
}

MainWindow::~MainWindow()
{

}