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
    display->setMaxLength(14);
    display->setText("0");

    QGridLayout* buttonsLayout = new QGridLayout();
    mainLayout->addLayout(buttonsLayout, 5);

    const QString buttonLabels[5][4] = {
        {"C", "⌫", "%", "/"},
        {"7", "8", "9", "+"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"±", "0", ".", "="}
    };

    for (int row = 0; row < 5; ++row) {
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

                else if (buttonText == "±") {
                    QString text = display->text();
                    if (text == "0") return;
                    double value = text.toDouble();
                    value = -value;
                    display->setText(QString::number(value));
                }

                else if (buttonText == ".") {
                    if (this->isWaitingForOperand) {
                        display->setText("0.");
                        this->isWaitingForOperand = false;
                        return;
                    }

                    QString currentText = display->text();
                    if (currentText.isEmpty()) {
                        display->setText("0.");
                        return;
                    }

                    if (currentText.contains('.')) {
                        return;
                    }

                    display->setText(currentText + ".");
                }

                else if (buttonText == "%") {
                    QString text = display->text();
                    if (text == "0") return;
                    double value = text.toDouble();
                    value = value / 100.0;
                    display->setText(QString::number(value));
                    this->isWaitingForOperand = false;
                }

                else if (buttonText == "⌫") {
                    if (this->isWaitingForOperand) return;
                    if (display->text() == "0") return;
                    QString text = display->text();
                    text.chop(1);
                    if (text.isEmpty() || text == "-") {
                        text = "0";
                    }
                    display->setText(text);
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
                            display->setText("Ошибка");
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
                    if (currentText.length() == display->maxLength()) {
                        return;
                    }
                    else {
                        if (currentText == "0") {
                            display->setText(buttonText);
                        } else {
                            display->setText(currentText + buttonText);
                        }
                    }
                }
            });


        }
    }
}

MainWindow::~MainWindow()
{

}