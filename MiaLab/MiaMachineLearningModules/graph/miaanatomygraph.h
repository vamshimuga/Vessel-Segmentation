#ifndef MIAANATOMYGRAPH_H
#define MIAANATOMYGRAPH_H
#include <Eigen/Dense>
#include <string>
#include <vector>
#include "MiaCoreModules/transform/miapoint4d.h"

struct AnatomyNode
{
    std::string label;
    MiaPoint4D location;
    float certainty;
};
struct WeightedEdge
{
    int start;
    int end;
    float weight;
};

class MiaAnatomyGraph
{
public:
    MiaAnatomyGraph();
    void addAnatomyNode(const AnatomyNode& node) ;
    void addEdge(int start, int end, float weight);
    int countNodes(){return nodes.size();}
    float getSumCertainty();
    const AnatomyNode& getNodeAt(int ind) { return nodes[ind];}
    std::vector<MiaAnatomyGraph> findConnectedSubGraphs(float threshold) const;
    float getEdgeWeight(int start, int end) const;
    void clear();
    void deepFirstSearchWithThreshold(int nodeid, MiaAnatomyGraph& graph, std::vector<bool>& checklist, float threshold) const;
signals:

public slots:
protected:
     std::vector<AnatomyNode> nodes;
     std::vector<WeightedEdge> edges;

};

#endif // MIAANATOMYGRAPH_H
