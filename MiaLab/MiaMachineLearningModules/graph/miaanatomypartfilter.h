#ifndef MIAANATOMYPARTFILTER_H
#define MIAANATOMYPARTFILTER_H
#include "miaanatomygraph.h"
class MiaTransform;

struct AnatomyRelation
{
    std::string label1;
    std::string label2;
    Eigen::Vector3f mean;
    Eigen::Matrix3f covariance;
};
struct AnatomyDetectorPerformance
{
    std::string label;
    float certainty;
};

class MiaAnatomyPartFilter
{
public:
    MiaAnatomyPartFilter();
    void addStandardMarker(std::string label, const MiaPoint4D& loc);
    void addAnatomyNode(const std::string label, const MiaPoint4D& loc, const float cert ) ;
    void addStandardVariances(std::string label1, std::string label2, const Eigen::Vector3f& mean, const Eigen::Matrix3f& covariance) ;
    bool getBestTransformation(const MiaTransform& imagetransform, Eigen::VectorXf &parameters);
    void clear();
signals:

public slots:
protected:
     std::vector<AnatomyRelation> standardRelativeRelations;
     std::map<std::string, AnatomyNode> standardMarkers;
     MiaAnatomyGraph anatomyGraph;
     float getConfidenceLevel(const AnatomyNode& node1, const AnatomyNode& node2);
};

#endif // MIAANATOMYPARTFILTER_H
