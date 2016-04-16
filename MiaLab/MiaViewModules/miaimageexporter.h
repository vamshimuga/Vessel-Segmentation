#ifndef MIAIMAGEEXPORTER_H
#define MIAIMAGEEXPORTER_H

#include "MiaCoreModules/miamodule.h"
#include "miaimagereader.h"

class vtkMatrix4x4;
class MIAVIEWMODULESSHARED_EXPORT MiaImageExporter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaImageExporter(MiaNetwork *parent = 0);
    void setFileName(QString name,FileFormat format);
    QString getFileName() {return fileName;}
signals:

public slots:
protected:
    QString fileName;
    FileFormat currentFormat;
    virtual bool runOperation();
    bool saveMHDImage();
    bool saveNIFTIImage();
    virtual bool checkIfNeedReAllocOutputImage(){return false;}
    vtkMatrix4x4 *getTranformMatrix();
};

#endif // MIAIMAGEEXPORTER_H
