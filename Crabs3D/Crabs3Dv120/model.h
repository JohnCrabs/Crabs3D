#ifndef MODEL_H
#define MODEL_H

#include <iostream>

#include "kmeans.h"

struct ModelColor {
    int red;
    int green;
    int blue;
};

struct Connections {
    int idBase;

    int newConnectionsSize;
    int* idNewConnections;

    int prevConnectionsSize;
    int* idPrevConnections;
};

struct BonePoints {
    int* id;
    float* x;
    float* y;
    float* z;
};

struct Bones {
    int boneSize;
    int pointSize;
    bool viewState;
    bool state;
    Connections* bones;
    BonePoints* points;
};

struct ModelInfo {
    std::string* name;
    ModelColor color;

    bool state;
};

struct HumanSizes {
    float height;
};

class Model
{
public:
    Model();
    ~Model();

    //ModelInfo
    void SetModelName(const std::string modelName) {modelInfo.name = new std::string[1];
                                                    modelInfo.name[0] = modelName;}
    std::string GetModelName() {return modelInfo.name[0];}

    void SetColorRed(const int red) {modelInfo.color.red = red;}
    int GetColorRed() {return modelInfo.color.red;}

    void SetColorGreen(const int green) {modelInfo.color.green = green;}
    int GetColorGreen() {return modelInfo.color.green;}

    void SetColorBlue(const int blue) {modelInfo.color.blue = blue;}
    int GetColorBlue() {return modelInfo.color.blue;}

    void SetColor(const ModelColor color) {modelInfo.color = color;}
    ModelColor GetColor() {return modelInfo.color;}

    void SetState(const bool state) {modelInfo.state = state;}
    bool GetState() {return modelInfo.state;}

    void SetBoneViewState(const bool state) {bones.viewState = state;}
    bool SetBoneViewState() {return bones.viewState;}

    Bones GetBones() {return bones;}

    void CreateModelFromCluster(const Cluster cluster, const Cloud cloud[], const int pointCloudFrameSize, const int pointCloudPointSize, Model* model);
    void CleanUpBones(Model* model);

private:
    ModelInfo modelInfo;
    HumanSizes humanSizes;
    Bones bones;
};

#endif // MODEL_H
