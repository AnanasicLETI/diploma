#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

#include "matrixcalc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_RankSpinBox_valueChanged(int arg1);

    void on_dimensionSpinBox_valueChanged(int arg1);

    void on_caclFirstBtn_clicked();

    void on_input1_textChanged();

    void on_tabWidget_tabBarClicked(int index);

    void on_input2_textChanged();

    void on_input3_External_textChanged(const QString &arg1);

    void on_input3_Internal_textChanged();

    void on_input4_textChanged();

    void on_saveBtn_clicked();

private:
    Ui::MainWindow *ui;

    matrixCalc matrixCalcEntity;

    bool isValidInput;

    QVector<unsigned int> inputSequence;
    QVector<QVector<unsigned int>>* outputData;

    void updateInformation();
    QString inputValidation(int state);

    bool isDigitalString(QString str);
    QVector<unsigned int> getVectorIntFromStr(QString str);

    bool firstValidation(QString* errorStr);
    bool secondValidation(QString* errorStr);
    bool thirdValidation(QString* errorStr);
    bool fouthValidation(QString* errorStr);
};
#endif // MAINWINDOW_H
