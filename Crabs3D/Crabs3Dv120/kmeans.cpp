#include "kmeans.h"
#include <random>

void KMeans::SetCluster(Cloud cloud, int clusterNumber, const int pointSize)
{
    clusterSize = clusterNumber;
    cluster = new Cluster[clusterNumber];

    float *dist;
    dist = (float*)malloc(clusterNumber*sizeof(float));
    Centroid *NewCentroids;
    NewCentroids = (Centroid*)malloc(clusterNumber*sizeof(Centroid));

    for(int i = 0; i < clusterNumber; i++) {
        cluster[i].view = true;
        cluster[i].pointSize = 1.0;

        cluster[i].color.red = (rand() % 255) / 255.0;
        cluster[i].color.green = (rand() % 255) / 255.0;
        cluster[i].color.blue = (rand() % 255) / 255.0;
        cluster[i].changeColor = true;

        cluster[i].centroid.x = cloud.x[i];
        cluster[i].centroid.y = cloud.y[i];
        cluster[i].centroid.z = cloud.z[i];
    }

    float mindist;
    int mindist_index;
    int index;
    bool loop = true;
    while (loop) {
        //Allocate starting memory for cluster cloud
        for(int i = 0; i < clusterNumber; i++) {
            cluster[i].size = 0;
            cluster[i].cloud.id = (int*)malloc(pointSize*sizeof(int));
            cluster[i].cloud.x = (float*)malloc(pointSize*sizeof(float));
            cluster[i].cloud.y = (float*)malloc(pointSize*sizeof(float));
            cluster[i].cloud.z = (float*)malloc(pointSize*sizeof(float));
            cluster[i].cloud.name = new std::string[pointSize];
        }

        //Set clouds to clusters
        for(int i = 0; i < pointSize; i++) {
            for(int j = 0; j < clusterNumber; j++) {
                 float dx = (cluster[j].centroid.x  - cloud.x[i]);
                 float dy = (cluster[j].centroid.y - cloud.y[i]);
                 float dz = (cluster[j].centroid.z - cloud.z[i]);

                 if(dx < 0) {
                     dx *= -1;
                 }
                 if(dy < 0) {
                     dy *= -1;
                 }
                 if(dz < 0) {
                     dz *= -1;
                 }

                dist[j] = dx + dy + dz; //Manhantan distance;
            }

            mindist = dist[0];
            mindist_index = 0;

            for(int j = 1; j < clusterNumber; j++) {
                if(dist[j] < mindist) {
                    mindist = dist[j];
                    mindist_index = j;
                }
            }

             index = cluster[mindist_index].size;
             cluster[mindist_index].cloud.id[index] = cloud.id[i];
             cluster[mindist_index].cloud.x[index] = cloud.x[i];
             cluster[mindist_index].cloud.y[index] = cloud.y[i];
             cluster[mindist_index].cloud.z[index] = cloud.z[i];
             //cluster[mindist_index].cloud.name[index] = cloud.name[i];
             cluster[mindist_index].size++;
        }

        //Recalculate Centroids
        for(int i = 0; i < clusterNumber; i++) {
            NewCentroids[i].x = 0.0;
            NewCentroids[i].y = 0.0;
            NewCentroids[i].z = 0.0;

            for(int j = 0; j < cluster[i].size; j++) {
                NewCentroids[i].x += cluster[i].cloud.x[j];
                NewCentroids[i].y += cluster[i].cloud.y[j];
                NewCentroids[i].z += cluster[i].cloud.z[j];
            }

            NewCentroids[i].x /= cluster[i].size;
            NewCentroids[i].y /= cluster[i].size;
            NewCentroids[i].z /= cluster[i].size;
        }

        //Check loop conditions
        loop = false;
        for(int i = 0; i < clusterNumber; i++) {

            float cent_dx = NewCentroids[i].x - cluster[i].centroid.x;
            float cent_dy = NewCentroids[i].y - cluster[i].centroid.y;
            float cent_dz = NewCentroids[i].z - cluster[i].centroid.z;

            if(cent_dx < 0) {
                cent_dx *= -1;
            }
            if(cent_dy < 0) {
                cent_dy *= -1;
            }
            if(cent_dz < 0) {
                cent_dz *= -1;
            }

            if(cent_dx > 0.001 || cent_dy > 0.001 || cent_dz > 0.001) {
                cluster[i].centroid.x = NewCentroids[i].x;
                cluster[i].centroid.y = NewCentroids[i].y;
                cluster[i].centroid.z = NewCentroids[i].z;

                loop = true;
            }
        }

        //If loop == true then free their values so they can be recreated
        if(loop == true) {
            for(int i = 0; i < clusterNumber; i++) {
                free(cluster[i].cloud.id);
                free(cluster[i].cloud.x);
                free(cluster[i].cloud.y);
                free(cluster[i].cloud.z);
                delete[] cluster[i].cloud.name;
            }
        }
    }

    std::string nameIndex;
    for(int i = 0; i < clusterNumber; i++) {
        nameIndex = "Cluster_";
        nameIndex += std::to_string(i);
        nameIndex += " (";
        nameIndex += std::to_string(cluster[i].size);
        nameIndex += ")";

        cluster[i].name = new std::string[1];
        cluster[i].name[0] = nameIndex;
    }

    free(dist);
    free(NewCentroids);

}

void KMeans::CleanUp() {
    for(int i = 0; i < clusterSize; i++) {
        free(cluster[i].cloud.id);
        free(cluster[i].cloud.x);
        free(cluster[i].cloud.y);
        free(cluster[i].cloud.z);

        //delete[] cluster[i].cloud.name; //segmentation fault if exist
    }
    //free(cluster); //segmentation fault if exist
}

