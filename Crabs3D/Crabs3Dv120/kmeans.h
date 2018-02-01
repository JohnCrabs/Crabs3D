#ifndef KMEANS_H
#define KMEANS_H

#include <iostream>

struct Cloud
{
    int *id;
    std::string *name;

    float *x;
    float *y;
    float *z;
};

struct Centroid
{
    float x;
    float y;
    float z;
};

struct Color
{
    float red;
    float green;
    float blue;
};

struct Cluster
{
    Cloud cloud; //Point Cloud inside the cluster
    Centroid centroid; //The centroid of point cloud (center of mass)
    Color color; //The color of the cluster (when viewing)
    std::string *name; //The name of the cluster
    float pointSize; //The size of points (when viewing)
    bool view; //Set it to true for view or set it to false to hide these points
    bool changeColor; //Set it to true when the color
    int size; //The size of point cloud

};

class KMeans
{
public:
    KMeans() {}
    void SetCluster(Cloud cloud, int clusterNumber, const int pointSize);
    void CleanUp();

    Cluster GetCluster(const int index) {return cluster[index];}
    void SetClusterView(const int index, bool state) {cluster[index].view = state;}
    void SetColorValueRed(const int index, const float red) {cluster[index].color.red = red;}
    void SetColorValueGreen(const int index, const float green) {cluster[index].color.green = green;}
    void SetColorValueBlue(const int index, const float blue) {cluster[index].color.blue = blue;}
    void SetColorState(const int index, bool state) {cluster[index].changeColor = state;}
    void SetPointSize(const int index, const float size) {cluster[index].pointSize = size;}
    void SetName(const int index, const std::string name) {cluster[index].name[0] = name;}

    bool GetColorState(const int index) {return cluster[index].changeColor;}


private:

    Cluster *cluster;
    int clusterSize;
};

#endif // KMEANS_H
