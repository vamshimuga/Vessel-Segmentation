#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include "MiaCoreModules/miamodule.h"
#include "miamodules_global.h"

enum FileFormat { MIA_DICOM,MIA_CHALLENGE,MIA_MHD,MIA_2D,MIA_NII,MIA_VTK};
class MIAVIEWMODULESSHARED_EXPORT MiaImageReader : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaImageReader(MiaNetwork *parent = 0);
    virtual ~MiaImageReader();
    void setFileName(QString name,FileFormat format);
    QString getFileName() {return fileName;}
    bool LoadImageInSharedMemory(QString memID,int size);
    void freeSharedMemory();
signals:

public slots:
protected:
    QString fileName;
    FileFormat currentFormat;
    virtual bool reAllocOutputImage();
    virtual bool checkIfNeedReAllocOutputImage() { return aNewFileSet;}
    bool loadMHDImage();
    bool loadDICOMImage();
    bool loadChallengeFolder();
    bool load2DImage();
    bool load2DDicomSlice(QString filename);
    bool loadDICOMImageWithDCMTK();
    bool loadNIFTIImage();
    bool loadVTKImage();
    void* sharedMemory;
    int sharedMemorySize;
    bool aNewFileSet;

};

#endif // IMAGEREADER_H
