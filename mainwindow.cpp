#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    matrixCalcEntity = matrixCalc();
    matrixCalcEntity.changeInitialData(ui->RankSpinBox->value(), ui->dimensionSpinBox->value());
    matrixCalcEntity.setState(ui->tabWidget->currentIndex());

    isValidInput = false;
    outputData = new QVector<QVector<unsigned int>>();

    updateInformation();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_RankSpinBox_valueChanged(int Rank)
{
    Q_UNUSED(Rank);
    updateInformation();
}

void MainWindow::on_dimensionSpinBox_valueChanged(int dimension)
{
    Q_UNUSED(dimension);
    updateInformation();
}

void MainWindow::on_caclFirstBtn_clicked()
{
    outputData->clear();
    if(!isValidInput){
        QMessageBox msgBox;
        msgBox.setText("Некорректный ввод");
        msgBox.exec();
        return;
    }

    QString outStr;

    switch (matrixCalcEntity.getState()) {
    case 0:{
        outStr = matrixCalcEntity.calcFirst(getVectorIntFromStr(ui->input1->toPlainText()), outputData);
    }break;
    case 1:{
        outStr = matrixCalcEntity.calcSecond(getVectorIntFromStr(ui->input2->toPlainText()), outputData);
    }break;
    case 2:{
        QVector<QVector<unsigned int>> input;
        QString strExternal = ui->input3_External->text();
        QString strInternal = ui->input3_Internal->toPlainText();
        input.append(getVectorIntFromStr(strExternal));
        QList<QString> listStrInternal = strInternal.split('\n');
        for(int i=0; i < listStrInternal.size();i++){
            input.append(getVectorIntFromStr(listStrInternal[i]));
        }
        outStr = matrixCalcEntity.calcThird(input, outputData);
    }break;
    case 3:{
        outStr = matrixCalcEntity.calcFourth(getVectorIntFromStr(ui->input4->toPlainText()), outputData);
    }break;
    }


//    int colCount = outputData->first().size();
//    int rowCount = outputData->size();
//    ui->tableOutWidget->clear();
//    ui->tableOutWidget->setColumnCount(colCount);
//    ui->tableOutWidget->setRowCount(rowCount);

//    for (int i=0; i <  rowCount;i++){
//        for (int j=0; j <  colCount;j++){
//            ui->tableOutWidget->setItem(i, j, new QTableWidgetItem(QString::number(outputData->at(i).at(j))));
//        }
//    }
//    QStringList verticalHeader;
//    for(int i=0; i < outNotCompositeNum.size();i++){
//        verticalHeader.append(outNotCompositeNum[i]);
//    }
//    ui->tableOutWidget->setVerticalHeaderLabels(verticalHeader);

    ui->outputPlainTextEdit->setPlainText(outStr);

}

void MainWindow::updateInformation()
{
    matrixCalcEntity.changeInitialData(ui->RankSpinBox->value(), ui->dimensionSpinBox->value());

//    ui->tableOutWidget->clear();
//    ui->tableOutWidget->setColumnCount(0);
//    ui->tableOutWidget->setRowCount(0);

    unsigned int requiredInputLength = matrixCalcEntity.getInputLength();
    unsigned int maxElementValue = matrixCalcEntity.getMaxElementValue();
    int state = matrixCalcEntity.getState();

    QString errorStr = inputValidation(state);

    switch (state) {
    case 0:{
        ui->input1CountLbl->setText("Вводимая длина: "+QString::number(ui->input1->toPlainText().size()));
    }break;
    case 1:{
        ui->input2CountLbl->setText("Вводимая длина: "+QString::number(ui->input2->toPlainText().size()));
    }break;
    case 2:{
        ui->input3CountLbl->setText("Длина внешней последовательности: "+QString::number(ui->input3_External->text().size()));
    }break;
    case 3:{
        ui->input4CountLbl->setText("Вводимая длина: "+QString::number(ui->input4->toPlainText().size()));
    }break;
    }


    ui->statusbar->showMessage("Необходимая длина: "+ QString::number(requiredInputLength)
            +" Максимальный элемент: "+QString::number(maxElementValue)
            +"| "+errorStr);
}

QString MainWindow::inputValidation(int state)
{
    QString errorStr;
    switch (state) {
    case 0:{
        isValidInput = firstValidation(&errorStr);
    }break;
    case 1:{
        isValidInput = secondValidation(&errorStr);
    }break;
    case 2:{
        isValidInput = thirdValidation(&errorStr);
    }break;
    case 3:{
        isValidInput = fouthValidation(&errorStr);
    }break;
    }

    return errorStr;
}

bool MainWindow::isDigitalString(QString str)
{
    for(int i = 0; i < str.size(); i++){
        if(!str[i].isDigit()){
            return false;
        }
    }
    return true;
}

QVector<unsigned int> MainWindow::getVectorIntFromStr(QString str)
{
    QVector<unsigned int> out;
    for(int i = 0; i < str.size(); i++){
        out.append( ((QString)str[i]).toInt() );
    }
    return out;
}

bool MainWindow::firstValidation(QString* errorStr)
{
    bool lenError = false;
    bool digitalError = false;
    bool maxValueError = false;

    QString str = ui->input1->toPlainText();

    if( ((unsigned int)str.size()) != matrixCalcEntity.getInputLength()){
        lenError = true;
    }
    if(!isDigitalString(str)){
        digitalError = true;
    }

    if(!matrixCalcEntity.isValidInput(getVectorIntFromStr(str))){
        maxValueError = true;
    }

    if(lenError){
        *errorStr += " ошибка длины строки";
    }
    if(digitalError){
        if( !(*errorStr).isEmpty())
            *errorStr +=",";
        *errorStr += " допустим ввод только цифр";
    }
    if(maxValueError){
        if( !(*errorStr).isEmpty())
            *errorStr +=",";
        *errorStr += " превышение значений";
    }

    if(lenError || digitalError || maxValueError){
        ui->input1->setStyleSheet("background-color: #FF6859;");
        return false;
    }else{
        ui->input1->setStyleSheet("");
    }

    *errorStr = "";
    return true;
}

bool MainWindow::secondValidation(QString* errorStr)
{
    bool lenError = false;
    bool digitalError = false;
    bool maxValueError = false;

    QString str = ui->input2->toPlainText();
    if( ((unsigned int)str.size()) != matrixCalcEntity.getInputLength()){
        lenError = true;
    }
    if(!isDigitalString(str)){   
        digitalError = true;
    }

    if(!matrixCalcEntity.isValidInput(getVectorIntFromStr(str))){
        maxValueError = true;
    }

    if(lenError){
        *errorStr += " ошибка длины строки";
    }
    if(digitalError){
        if( !(*errorStr).isEmpty())
            *errorStr +=",";
        *errorStr += " допустим ввод только цифр";
    }
    if(maxValueError){
        if( !(*errorStr).isEmpty())
            *errorStr +=",";
        *errorStr += " превышение значений";
    }

    if(lenError || digitalError || maxValueError){
        ui->input2->setStyleSheet("background-color: #FF6859;");
        return false;
    }else{
        ui->input2->setStyleSheet("");
    }

    *errorStr = "";
    return true;
}

bool MainWindow::thirdValidation(QString *errorStr)
{

    bool lenErrorExt = false;
    bool digitalErrorExt = false;
    bool maxValueErrorExt = false;

    bool lenErrorInter = false;
    bool digitalErrorInter = false;
    bool maxValueErrorInter = false;

    QString strExternal = ui->input3_External->text();

    if( ((unsigned int)strExternal.size()) != matrixCalcEntity.getInputLength()){
        lenErrorExt = true;
    }
    if(!isDigitalString(strExternal)){
        digitalErrorExt = true;
    }
    if(!matrixCalcEntity.isValidInput(getVectorIntFromStr(strExternal))){
        maxValueErrorExt = true;
    }


    QString strInternal = ui->input3_Internal->toPlainText();
    QList<QString> strListInternal = strInternal.split('\n');
    for(int i=0; i < strListInternal.size(); i++){
        if((unsigned int)strListInternal[i].size() != matrixCalcEntity.getInputLength()){
            lenErrorInter = true;
        }
        if(!isDigitalString(strListInternal[i])){
            digitalErrorInter = true;
        }
        if(!matrixCalcEntity.isValidInput(getVectorIntFromStr(strListInternal[i]))){
            maxValueErrorInter = true;
        }
    }
    if(lenErrorExt || lenErrorInter){
        *errorStr += " ошибка длины строки";
    }
    if(digitalErrorExt || digitalErrorInter){
        if( !(*errorStr).isEmpty())
            *errorStr +=",";
        *errorStr += " допустим ввод только цифр";
    }
    if(maxValueErrorExt || maxValueErrorInter){
        if( !(*errorStr).isEmpty())
            *errorStr +=",";
        *errorStr += " превышение значений";
    }

    if(lenErrorExt || digitalErrorExt || maxValueErrorExt){
        ui->input3_External->setStyleSheet("background-color: #FF6859;");
    }else{
        ui->input3_External->setStyleSheet("");
    }

    if(lenErrorInter || digitalErrorInter || maxValueErrorInter){
        ui->input3_Internal->setStyleSheet("background-color: #FF6859;");
    }else{
        ui->input3_Internal->setStyleSheet("");
    }

    if(lenErrorExt || lenErrorInter || digitalErrorExt || digitalErrorInter || maxValueErrorExt || maxValueErrorInter){
        return false;
    }


    if((unsigned int)strListInternal.size() != matrixCalcEntity.getDimension()){
        *errorStr = " неверное количество внутренних последовательностей";
        ui->input3_Internal->setStyleSheet("background-color: #FF6859;");
        return false;
    }
    return true;
}

bool MainWindow::fouthValidation(QString *errorStr)
{
    bool lenError = false;
    bool digitalError = false;
    bool maxValueError = false;

    QString str = ui->input4->toPlainText();
    if( ((unsigned int)str.size()) != matrixCalcEntity.getInputLength()){
        lenError = true;
    }
    if(!isDigitalString(str)){
        digitalError = true;
    }

    if(!matrixCalcEntity.isValidInput(getVectorIntFromStr(str))){
        maxValueError = true;
    }

    if(lenError){
        *errorStr += " ошибка длины строки";
    }
    if(digitalError){
        if( !(*errorStr).isEmpty())
            *errorStr +=",";
        *errorStr += " допустим ввод только цифр";
    }
    if(maxValueError){
        if( !(*errorStr).isEmpty())
            *errorStr +=",";
        *errorStr += " превышение значений";
    }

    if(lenError || digitalError || maxValueError){
        ui->input4->setStyleSheet("background-color: #FF6859;");
        return false;
    }else{
        ui->input4->setStyleSheet("");
    }

    *errorStr = "";
    return true;
}

void MainWindow::on_input1_textChanged()
{
    updateInformation();
}

void MainWindow::on_input2_textChanged()
{
    updateInformation();
}

void MainWindow::on_input3_External_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    updateInformation();
}

void MainWindow::on_input3_Internal_textChanged()
{
    updateInformation();
}

void MainWindow::on_input4_textChanged()
{
    updateInformation();
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    matrixCalcEntity.setState(index);
    updateInformation();
}

void MainWindow::on_saveBtn_clicked()
{
    if(ui->outputPlainTextEdit->toPlainText().isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("Некорректный ввод");
        msgBox.exec();
        return;
    }

    QString outStr;

    switch (matrixCalcEntity.getState()) {
    case 0:{
        outStr = "Ввод:\n" + ui->input1->toPlainText() + "\n";
    }break;
    case 1:{
        outStr = "Ввод:\n" + ui->input2->toPlainText() + "\n";
    }break;
    case 2:{
        outStr = "Ввод:\nВнешняя последовательность:\n" + ui->input3_External->text() + "\n";
        outStr += "Внутренняя последовательность:\n" +ui->input3_Internal->toPlainText() + "\n";
    }break;
    case 3:{
        outStr = "Ввод:\n" + ui->input4->toPlainText() + "\n";
    }break;
    }
    outStr += "Вывод:\n"+ui->outputPlainTextEdit->toPlainText();

    QString fileName = QFileDialog::getSaveFileName(nullptr, "Сохранение файла", "", "*.txt");

    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox msgBox;
            msgBox.setText("Неверное имя файла");
            msgBox.exec();
            return;
        } else {
            QTextStream stream(&file);
            stream << outStr;
            stream.flush();
            file.close();
        }
    }
}
