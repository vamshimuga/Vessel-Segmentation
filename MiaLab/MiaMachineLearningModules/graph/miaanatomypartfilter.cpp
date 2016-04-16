#include "miaanatomypartfilter.h"
#include<iostream>
#include "transform/MiaTransform.h"

MiaAnatomyPartFilter::MiaAnatomyPartFilter()
{

}

void MiaAnatomyPartFilter::addStandardMarker(std::string label, const MiaPoint4D &loc)
{
    AnatomyNode node;
    node.label = label;
    node.location = loc;
    node.certainty = 1.0f;
    standardMarkers[label] = node;
}

void MiaAnatomyPartFilter::addAnatomyNode(const std::string label, const MiaPoint4D &loc, const float cert)
{
    AnatomyNode node;
    node.label = label;
    node.location = loc;
    node.certainty = cert;
    anatomyGraph.addAnatomyNode(node);
    int newAddedNodeIndex = anatomyGraph.countNodes()-1;

    std::cout << "Add node" << label << "certainty " << cert << std::endl;

    //add eddges
    for (int i = 0; i < anatomyGraph.countNodes(); i++)
    {
        float weight = getConfidenceLevel(anatomyGraph.getNodeAt(i), node);
        if(weight > 0)
            anatomyGraph.addEdge(i, newAddedNodeIndex, weight);
    }



}

void MiaAnatomyPartFilter::addStandardVariances(std::string label1, std::string label2, const Eigen::Vector3f &mean, const Eigen::Matrix3f &covariance)
{
    AnatomyRelation relation;
    relation.label1 = label1;
    relation.label2 = label2;
    relation.mean = mean;
    relation.covariance = covariance;
    standardRelativeRelations.push_back(relation);
}

bool MiaAnatomyPartFilter::getBestTransformation(const MiaTransform &imagetransform, Eigen::VectorXf &parameters)
{
    std::vector<MiaAnatomyGraph> graphs = anatomyGraph.findConnectedSubGraphs(0.25);
    std::cout << "Total nodes: " << anatomyGraph.countNodes() << std::endl;
    std::cout << "Coonnected Graphs: " << graphs.size() << std::endl;
    int bestGraphIndex = -1;
    float maxCertainty = -1.0;
    for(int i = 0; i < graphs.size(); i++)
    {
        if(graphs[i].getSumCertainty() > maxCertainty)
        {
            maxCertainty = graphs[i].getSumCertainty();
            bestGraphIndex = i;
        }
    }
    if(bestGraphIndex < 0 || maxCertainty < 0)
        return false;
    std::cout <<"best fit " << bestGraphIndex << " with nodes " << graphs[bestGraphIndex].countNodes() << std::endl;
    Eigen::MatrixXf A = Eigen::MatrixXf::Zero(3*graphs[bestGraphIndex].countNodes(), 12);
    Eigen::VectorXf b;
    b.resize(3*graphs[bestGraphIndex].countNodes());

    for(int i = 0; i < graphs[bestGraphIndex].countNodes(); i++)
    {
        const AnatomyNode& node = graphs[bestGraphIndex].getNodeAt(i);
        MiaPoint4D ptInVoxel = imagetransform.fromPatientCoordinate(node.location);
        std::string label = node.label;
        MiaPoint4D modelPtInVoxel = standardMarkers[label].location;
        A(3*i, 0) = ptInVoxel.pos[0];
        A(3*i, 1) = ptInVoxel.pos[1];
        A(3*i, 2) = ptInVoxel.pos[2];
        A(3*i, 3) = 1.0f;
        A(3*i+1, 4) = ptInVoxel.pos[0];
        A(3*i+1, 5) = ptInVoxel.pos[1];
        A(3*i+1, 6) = ptInVoxel.pos[2];
        A(3*i+1, 7) = 1.0f;
        A(3*i+2, 8) = ptInVoxel.pos[0];
        A(3*i+2, 9) = ptInVoxel.pos[1];
        A(3*i+2, 10) = ptInVoxel.pos[2];
        A(3*i+2, 11) = 1.0f;


        b[3*i+0] = modelPtInVoxel.pos[0];
        b[3*i+1] = modelPtInVoxel.pos[1];
        b[3*i+2] = modelPtInVoxel.pos[2];

    }

    std::cout << A << std::endl;
    std::cout << b << std::endl;

    parameters = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);

}

void MiaAnatomyPartFilter::clear()
{
    standardRelativeRelations.clear();
    standardMarkers.clear();
    anatomyGraph.clear();
}

float MiaAnatomyPartFilter::getConfidenceLevel(const AnatomyNode &node1, const AnatomyNode &node2)
{
    //compute Mahalanobis distance
    float distance = 0;
    for(int i = 0; i <standardRelativeRelations.size(); i++)
    {
        MiaPoint4D pt1, pt2;
        bool foundAMatch = false;
        AnatomyRelation relation = standardRelativeRelations[i];
        if(relation.label1 == node1.label && relation.label2 == node2.label)
        {
            foundAMatch = true;
            pt1 = node1.location;
            pt2 = node2.location;
        }
        else if(relation.label1 == node2.label && relation.label2 == node1.label)
        {
            foundAMatch = true;
            pt2 = node1.location;
            pt1 = node2.location;
        }
        if(foundAMatch)
        {
            Eigen::Vector3f relativePos(pt1.pos[0]-pt2.pos[0], pt1.pos[1]-pt2.pos[1], pt1.pos[2]-pt2.pos[2]);
            Eigen::Vector3f delta = relativePos - standardRelativeRelations[i].mean;
            Eigen::Matrix3f incov = standardRelativeRelations[i].covariance.inverse();
            distance = delta.transpose() * incov * delta;
            distance = sqrt(distance);
            std::cout << standardRelativeRelations[i].covariance << std::endl;
            std::cout << delta << std::endl;
            std::cout << incov << std::endl;
            std::cout << node1.label << ":" << node2.label << " " << distance << std::endl;
        }

    }
    if(distance > 0)
        return 1.0/distance;

    return 0;

}

