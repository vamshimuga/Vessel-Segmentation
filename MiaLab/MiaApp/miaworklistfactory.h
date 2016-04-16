#ifndef MIAWORKLISTFACTORY_H
#define MIAWORKLISTFACTORY_H

#include <QObject>

class MiaWorkListFactory : public QObject
{
    Q_OBJECT
public:
    explicit MiaWorkListFactory(QObject *parent = 0);
    ~MiaWorkListFactory();

signals:

public slots:
    void stepThroughWorkList();
    void workStepFinished(bool sucess);
};

#endif // MIAWORKLISTFACTORY_H
