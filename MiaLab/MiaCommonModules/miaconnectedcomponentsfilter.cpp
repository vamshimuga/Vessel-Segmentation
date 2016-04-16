#include "miaconnectedcomponentsfilter.h"

MiaConnectedComponentsFilter::MiaConnectedComponentsFilter(MiaNetwork *parent) :
    MiaModule(parent)
{

}

bool MiaConnectedComponentsFilter::runOperation()
{
    return false;
}

bool MiaConnectedComponentsFilter::reAllocOutputImage()
{
    return false;
}
