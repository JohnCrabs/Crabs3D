#include "model.h"
#include "math.h"

Model::Model()
{
    modelInfo.state = false;
    modelInfo.name = new std::string;
    modelInfo.name[0] = "Model";
    modelInfo.color.red = 0;
    modelInfo.color.green = 0;
    modelInfo.color.blue = 0;

    bones.state = false;
}

Model::~Model()
{

}

void Model::CreateModelFromCluster(const Cluster cluster, const Cloud cloud[], const int pointCloudFrameSize, const int pointCloudPointSize, Model *model) {

    //Allocate memory for Bones
    model->bones.boneSize = cluster.size;
    model->bones.pointSize = cluster.size;
    model->bones.bones = (Connections*) malloc(model->bones.boneSize*sizeof(Connections));
    model->bones.points = (BonePoints*) malloc(pointCloudFrameSize*sizeof(BonePoints));

    for(register int i = 0; i <  model->bones.boneSize; i++) {
        model->bones.bones[i].idNewConnections = (int*)malloc((model->bones.boneSize - 1)*sizeof(int));
        model->bones.bones[i].idPrevConnections = (int*)malloc((model->bones.boneSize - 1)*sizeof(int));

        model->bones.bones[i].newConnectionsSize = 0;
        model->bones.bones[i].prevConnectionsSize = 0;
    }

    for(register int i = 0; i < pointCloudFrameSize; i++) {
        model->bones.points[i].id = (int*)malloc(model->bones.boneSize*sizeof(int));
        model->bones.points[i].x = (float*)malloc(model->bones.boneSize*sizeof(float));
        model->bones.points[i].y = (float*)malloc(model->bones.boneSize*sizeof(float));
        model->bones.points[i].z = (float*)malloc(model->bones.boneSize*sizeof(float));
    }

    //Create Bones
    int accIndex = 0;
    int accuracy = (85*((pointCloudFrameSize/4))) / 100;
    float dist1 = 0;
    float dist2;

    for(register int i = 0; i < model->bones.boneSize; i++) {
        for(register int j = 0; j < pointCloudPointSize; j++) {
            if(cluster.cloud.id[i] == cloud[0].id[j]) {
                for(register int k = 0; k < pointCloudFrameSize; k++) {
                    model->bones.points[k].id[i] = cloud[k].id[j];
                    model->bones.points[k].x[i] = cloud[k].x[j];
                    model->bones.points[k].y[i] = cloud[k].y[j];
                    model->bones.points[k].z[i] = cloud[k].z[j];
                }
            }
        }
    }

    //Loop through model
    for(register int modelIndex = 0; modelIndex < model->bones.boneSize; modelIndex++) {
        model->bones.bones[modelIndex].idBase = cluster.cloud.id[modelIndex];

        for(register int clusterIndex = modelIndex + 1; clusterIndex < cluster.size; clusterIndex++) {
            //Loop through PointCloud points
            for(register int pointIndex = 0; pointIndex < pointCloudPointSize; pointIndex++) {
                if(cluster.cloud.id[clusterIndex] == cloud[0].id[pointIndex]) {

                    //Loop through the next points
                    for(register int pointIndex2 = pointIndex+1; pointIndex2 < pointCloudPointSize; pointIndex2++) {
                        float dx1 = cloud[0].x[pointIndex2] - cloud[0].x[modelIndex];
                        float dy1 = cloud[0].y[pointIndex2] - cloud[0].y[modelIndex];
                        float dz1 = cloud[0].z[pointIndex2] - cloud[0].z[modelIndex];

                        dist1 = dx1*dx1 + dy1*dy1 + dz1*dz1;

                        //Loop through PointCloud frames
                        accIndex = 1;
                        int frameMax = pointCloudFrameSize;
                        for(register int cloudFrame = 1; cloudFrame < frameMax; cloudFrame+=2) {
                            float dx2 = cloud[cloudFrame].x[pointIndex2] - cloud[cloudFrame].x[pointIndex];
                            float dy2 = cloud[cloudFrame].y[pointIndex2] - cloud[cloudFrame].y[pointIndex];
                            float dz2 = cloud[cloudFrame].z[pointIndex2] - cloud[cloudFrame].z[pointIndex];

                            dist2 = dx2*dx2 + dy2*dy2 + dz2*dz2;
                            float dist = dist2 - dist1;
                            if(dist < 0) {
                                dist *= -1;
                             }
                            if(dist < 0.003) {
                                accIndex++;
                            }
                        }
                        if(accIndex > accuracy) {
                            model->bones.bones[modelIndex].idNewConnections[model->bones.bones[modelIndex].newConnectionsSize++] = cloud[0].id[pointIndex2];
                            model->bones.bones[modelIndex].idPrevConnections[model->bones.bones[modelIndex].prevConnectionsSize++] = cloud[0].id[pointIndex];
                        }
                    }
                }
             }
        }
     }

    model->bones.state = true;

}

void Model::CleanUpBones(Model* model) {
    if(model->bones.state) {
        for(int i = 0; i < model->bones.boneSize; i++) {
            free(model->bones.bones[i].idNewConnections);
            free(model->bones.bones[i].idPrevConnections);
        }
        for(int i = 0; i < model->bones.pointSize; i++) {
            free(model->bones.points[i].id);
            free(model->bones.points[i].x);
            free(model->bones.points[i].y);
            free(model->bones.points[i].z);
        }

        free(model->bones.bones);
        free(model->bones.points);
    }
}



