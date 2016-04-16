#ifndef MIAFOCUSVALUEREADOUT_H
#define MIAFOCUSVALUEREADOUT_H

#include "MiaCoreModules/miamodule.h"

class MIACOMMONMODULESSHARED_EXPORT MiaFocusValueReadOut : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaFocusValueReadOut(MiaNetwork *parent = 0);
    float curWinWidth,curWinLevel;
    MiaPoint4D pat_coordinate;
    MiaPoint4D voxel_coordinate;
    float value;
    QRgb rgbValue;
    QString summaryStr;
signals:
    void UpdateSummaryText(QString text);
public slots:
    bool runOperation();
    void dispatchParameters(const QMap <QString, QVariant> &parameters);
};

#endif // MIAFOCUSVALUEREADOUT_H
