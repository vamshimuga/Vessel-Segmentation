#include "miaanatomygraph.h"

MiaAnatomyGraph::MiaAnatomyGraph()
{

}

void MiaAnatomyGraph::addAnatomyNode(const AnatomyNode &node)
{
    nodes.push_back(node);
}

void MiaAnatomyGraph::addEdge(int start, int end, float weight)
{
    WeightedEdge edge;
    edge.start = start;
    edge.end = end;
    edge.weight = weight;
    edges.push_back(edge);
}

float MiaAnatomyGraph::getSumCertainty()
{
    float sum = 0;
    for(int i = 0; i < nodes.size(); i++)
    {
        sum += nodes[i].certainty;
    }
    return sum;
}

void MiaAnatomyGraph::deepFirstSearchWithThreshold(int nodeid, MiaAnatomyGraph &graph, std::vector<bool> &checklist, float threshold) const
{
    for(int j = 0; j < nodes.size(); j++)
    {
        if(!checklist[j] && j != nodeid)
        {
            float weight = getEdgeWeight(nodeid,j);
            if(weight >= threshold)
            {
                checklist[j] = true;
                graph.addAnatomyNode(nodes[j]);
                graph.addEdge(nodeid,j,weight);
                deepFirstSearchWithThreshold(j, graph, checklist, threshold);
            }
        }
    }
}

std::vector<MiaAnatomyGraph> MiaAnatomyGraph::findConnectedSubGraphs(float threshold) const
{
    std::vector<MiaAnatomyGraph> graphs;
    std::vector<bool> checklist;
    for(int i = 0; i < nodes.size(); i++)
        checklist.push_back(false);
    for(int i = 0; i < nodes.size(); i++)
    {
        if(!checklist[i])
        {
            checklist[i] = true;
            MiaAnatomyGraph anewgraph;
            anewgraph.addAnatomyNode(nodes[i]);
            deepFirstSearchWithThreshold(i,anewgraph,checklist,threshold);
            graphs.push_back(anewgraph);
        }
    }
    return graphs;
}

float MiaAnatomyGraph::getEdgeWeight(int start, int end) const
{
    for(int i = 0; i < edges.size(); i++)
    {
        if(start == edges[i].start && end == edges[i].end)
        {
            return edges[i].weight;
        }
        if(end == edges[i].start && start == edges[i].end)
        {
            return edges[i].weight;
        }
    }
    return 0;
}

void MiaAnatomyGraph::clear()
{
    edges.clear();
    nodes.clear();
}

