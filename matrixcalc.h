#ifndef MATRIXCALC_H
#define MATRIXCALC_H

#include <QVector>
#include <qmath.h>

#include <QDebug>

class matrixCalc
{
public:
    matrixCalc();
    void changeInitialData(int Rank, int dimension);

    unsigned int getDimension();
    unsigned int getRank();

    unsigned int getMaxElementValue();
    unsigned int getInputLength();

    bool isValidInput(QVector<unsigned int>);

    void setState(int newState);
    int getState();

    unsigned int magic(QVector<unsigned int> externalVector, QVector<QVector<unsigned int>> addedVector, QString* str);
    QVector<unsigned int> decompose(QVector<unsigned int> externalVector, QVector<unsigned int> addedVector);
    unsigned int adding(QVector<unsigned int> vector1, QVector<unsigned int> vector2);


    QString calcFirst(QVector<unsigned int> input, QVector<QVector<unsigned int>>* outputData);
    QString calcSecond(QVector<unsigned int> input, QVector<QVector<unsigned int>>* outputData);
    QString calcThird(QVector<QVector<unsigned int>> input, QVector<QVector<unsigned int> > *outputData);
    QString calcFourth (QVector<unsigned int> input, QVector<QVector<unsigned int>>* outputData);

    QString getStrFromVectorUInt(QVector<unsigned int> vec);

private:
    static const unsigned int pseudoMathNumRange = 10;

    QVector<unsigned int> numDecompositionArr[pseudoMathNumRange];
    QVector<unsigned int> pseudoPrimeNum;

    unsigned int rank;
    unsigned int dimension;

    int state;
};


#endif // MATRIXCALC_H
