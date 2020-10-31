#include "matrixcalc.h"

matrixCalc::matrixCalc()
{
    numDecompositionArr[0] = QVector<unsigned int>();
    numDecompositionArr[0].append(0);
    numDecompositionArr[1] = QVector<unsigned int>();
    numDecompositionArr[1].append(1);
    numDecompositionArr[2] = QVector<unsigned int>();
    numDecompositionArr[2].append(2);
    numDecompositionArr[3] = QVector<unsigned int>();
    numDecompositionArr[3].append(2);
    numDecompositionArr[3].append(1);
    numDecompositionArr[4] = QVector<unsigned int>();
    numDecompositionArr[4].append(4);
    numDecompositionArr[5] = QVector<unsigned int>();
    numDecompositionArr[5].append(4);
    numDecompositionArr[5].append(1);
    numDecompositionArr[6] = QVector<unsigned int>();
    numDecompositionArr[6].append(4);
    numDecompositionArr[6].append(2);
    numDecompositionArr[7] = QVector<unsigned int>();
    numDecompositionArr[7].append(7);
    numDecompositionArr[8] = QVector<unsigned int>();
    numDecompositionArr[8].append(7);
    numDecompositionArr[8].append(1);
    numDecompositionArr[9] = QVector<unsigned int>();
    numDecompositionArr[9].append(7);
    numDecompositionArr[9].append(2);


    // Це так, не нужное
    pseudoPrimeNum = QVector<unsigned int>();
    pseudoPrimeNum.append(1);
    pseudoPrimeNum.append(2);
    pseudoPrimeNum.append(4);
    pseudoPrimeNum.append(7);

    state = 0;
}

void matrixCalc::changeInitialData(int newRank, int newDimension)
{
    rank =newRank;
    dimension = newDimension;
}

unsigned int matrixCalc::getDimension()
{
    return dimension;
}

unsigned int matrixCalc::getRank()
{
    return rank;
}

unsigned int matrixCalc::getMaxElementValue()
{
    if(state == 0){
        return qPow(2, rank) - 1;
    }else{
        return 1;
    }

}

unsigned int matrixCalc::getInputLength()
{
    switch (state) {
    case 0:{
        return qPow(rank, dimension);
    }break;
    case 1:{
        return qPow(rank, (dimension+1));
    }break;
    case 2:{
        return qPow(rank, (dimension+1));
    }break;
    case 3:{
        return qPow(rank, (dimension+1));
    }break;
    default:{
        return 0;
    }
    }
}

bool matrixCalc::isValidInput(QVector<unsigned int> input)
{
    for(int i=0; i<input.size();i++){
        if(input[i] > this->getMaxElementValue()){
            return false;
        }
    }
    return true;
}

void matrixCalc::setState(int newState)
{
    state = newState;
}

int matrixCalc::getState()
{
    return state;
}

unsigned int matrixCalc::magic(QVector<unsigned int> externalVector, QVector<QVector<unsigned int>> addedVector, QString* str)
{
    for(int i=0; i < addedVector.size();i++){
        qDebug() << "_" <<externalVector;


        *str += getStrFromVectorUInt(externalVector);

        for(int j=i;j < addedVector.size();j++){
            *str += " x ";
            *str += getStrFromVectorUInt(addedVector[j]);
        }
        *str += " = ";

        externalVector=decompose(externalVector, addedVector[i]);

    }
    *str += QString::number(externalVector[0]);
    return externalVector[0];
}

QVector<unsigned int> matrixCalc::decompose(QVector<unsigned int> externalVector, QVector<unsigned int> addedVector)
{
    int size = addedVector.size();
    int k = externalVector.size()/size;
    QVector<unsigned int> outVector;

    for(int i=0; i<k; i++){
        QVector<unsigned int> tmp = externalVector.mid(i*size, size);
        outVector.append(adding(addedVector, tmp));
    }
    return outVector;
}

unsigned int matrixCalc::adding(QVector<unsigned int> vector1, QVector<unsigned int> vector2)
{
    int size = vector1.size();
    unsigned int out;
    for(int i=0; i < size; i++){
        out = out | (vector1[i]*vector2[i]);
    }
    return out;
}

QString matrixCalc::calcFirst(QVector<unsigned int> input, QVector<QVector<unsigned int>>* outputData)
{
    QString outStr = "";
    unsigned int count = 1;
    QVector<int> primeNumSet = QVector<int>();

    for(unsigned int i = 1; i <= rank; i++){

        while(numDecompositionArr[count].size() != 1){
            count ++;
            if(count > pseudoMathNumRange){
                return "Ограничение верхним порядком набора чисел";
            }
        }
        primeNumSet.append(count);

        outputData->append(QVector<unsigned int>());
        for(int j=0; j < input.size(); j++){
            if( numDecompositionArr[input[j]].contains(count)){
                outputData->last().append(1);
            }else{
                outputData->last().append(0);
            }
        }
        count ++;
    }

    for(int i = 0; i < primeNumSet.size(); i++){
        outStr += "{"+QString::number(primeNumSet[i])+"} :";
        for(int j=0;j < (*outputData)[i].size();j++){
            outStr += QString::number((*outputData)[i][j]);
        }
        outStr += "\n";
    }
    outStr += "Итог: ";

    for(int i = 0; i < (*outputData)[0].size(); i++){
        for(int j=0;j < primeNumSet.size();j++){
            outStr += QString::number((*outputData)[j][i]);
        }
    }

    return outStr;
}

QString matrixCalc::calcSecond(QVector<unsigned int> input, QVector<QVector<unsigned int> > *outputData)
{
    QString outStr = "";
    unsigned int count = 1;
    int localLen =this->getInputLength()/rank;

    QVector<int> primeNumSet = QVector<int>();

    for(unsigned int i = 0; i < rank; i++){

        while(numDecompositionArr[count].size() != 1){
            count ++;
            if(count > pseudoMathNumRange){
                return "Ограничение верхним порядком набора чисел";
            }
        }

        outputData->append(QVector<unsigned int>());
        primeNumSet.append(count);

        for(int j=0; j < localLen; j++){ // ((i)*localLen)  ;(localLen*(i+1))

            qDebug() << j << i << localLen << j*localLen << i+(j*localLen);
            if(input[i+j*rank] == 1){
                outputData->last().append(count);
            }else{
                outputData->last().append(0);
            }
        }
        count ++;
    }


    for(int i = 0; i < primeNumSet.size(); i++){
        outStr += "{"+QString::number(primeNumSet[i])+"}: ";
        for(int j=0;j < (*outputData)[i].size();j++){
            outStr += QString::number((*outputData)[i][j]);
        }
        outStr += "\n";
    }
    outStr += "Итог: ";
    QVector<unsigned int> tmp(localLen);
    for(int i = 0; i < outputData->size(); i++){
        for(int j = 0; j < localLen; j++){
            tmp[j] += (*outputData)[i][j];
        }
    }
    for(int i=0;i < (*outputData)[0].size();i++){
        outStr += QString::number(tmp[i]);
    }

    outputData->append(tmp);

    return outStr;
}

QString matrixCalc::calcThird(QVector<QVector<unsigned int> > input, QVector<QVector<unsigned int> > *outputData)
{
    QString outStr = "";
    int len = this->getInputLength();
    int localLen = len/rank;

    //outNotCompositeNum.append("=");
    outputData->append(QVector<unsigned int>());

    for(unsigned int i = 0; i < rank; i++){
        QVector<unsigned int> externalVector = input[0].mid((i)*localLen, localLen);
        for(int j=0; j < localLen;j++){
            QVector<QVector<unsigned int>> add;

            for(int n=this->getDimension(); n > 0; n--){
                add.append(QVector<unsigned int>());

                for(unsigned int m=0; m< this->rank;m++){
                    add.last().append(input[n][m*localLen+j]);
                    //qDebug() << n << m << j;
                }
            }
            //qDebug()<< add;
            QString tmpStr = "";
            outputData->last().append(magic(externalVector, add, &tmpStr));
            outStr += tmpStr + "\n";
        }
        outStr += "--------------------\n";
    }
    outStr += "Итог: "+ getStrFromVectorUInt(outputData->last());
    return outStr;
}

QString matrixCalc::calcFourth(QVector<unsigned int> input, QVector<QVector<unsigned int> > *outputData)
{
    QString outStr;

    int len = this->getInputLength();
    int n = this->getDimension();
    int k = this->getRank();
    int range;

    for(int i=1; i <= n; i++){
        range = len/qPow(k, i);
        QVector<unsigned int> tmp;// = QVector<unsigned int>(len);
        for(int j=0;j < qPow(k, i-1);j++){ // количество частей, которые мы обкашлеваем

            int indent = (range*k)*j;
            QVector<unsigned int> tmpNum = QVector<unsigned int>();

            for(int m=0;m < (range/k);m++){ // тут мы говорим, что для всех штук в ренже
                for(int h=0;h < k;h++){// тут определяем делимость на что мы смотрим
                   qDebug() << "___" << (range/k);


                    for(int l=0;l < k;l++){ // а тут мы просто берем штуку со всех ренжей
                        qDebug() << indent + l*range + m*k +h;
                        tmpNum.append(input[indent + l*range + m*k +h]);
                    }
//                    qDebug() << "_";
//                    for(int l=0;l<k;l++){
//                        qDebug() << indent + m*k + l +h*range << tmp.size();
//                        tmp[indent + m*k + l +h*range] = tmpNum[l];
//                    }
//                    qDebug() << "_____________________";
                }
            }
            tmp.append(tmpNum);
        }
        qDebug() << tmp;
        outStr += "T"+QString::number(i)+": ";
        outStr += getStrFromVectorUInt(tmp)+"\n";
        outputData->append(tmp);
    }

    return outStr;
}

QString matrixCalc::getStrFromVectorUInt(QVector<unsigned int> vec)
{
    QString out ="";
    for(int i=0; i<vec.size();i++){
        out+=QString::number(vec[i]);
    }
    return out;
}


