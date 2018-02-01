#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QMouseEvent>
#include <GL/freeglut.h>

#include "read_c3d.h"
#include "kmeans.h"
#include "cluster_options.h"
#include "model.h"
#include "model_create_dialog.h"

#include "unit_dialog.h"
#include "set_bones.h"

class GLWidget : public QGLWidget
{
public:
    explicit GLWidget(QWidget *parent = 0);

    //Initialize Open GL - Set Some Parameter
    void initializeGL();

    //Draw to the OpenGl window
    void paintGL();

    //When window resized
    void resizeGL(int w, int h);

    //Grid
    void Axes();

    //-------------------------------------------------------------------------------//

    /*~~~~~~~~~~~~~~~*/
    /* C3D Functions */
    /*~~~~~~~~~~~~~~~*/

    //Read C3D file
    bool ReadC3D(std::string fileName, QWidget* widget);

    //Enumeration class for exporting c3d sections
    enum class c3d {IsHeader,
                    IsParameter,
                    Is3DPoints,
                    IsAnalog};

    //Export C3D File
    bool ExportC3D(std::string fileName, c3d IsType);

    //Clean C3D file
    void CleanUpC3D();

    //-------------------------------------------------------------------------------//

    /*~~~~~~~~~~~~~~~~~~~*/
    /* Viewing Functions */
    /*~~~~~~~~~~~~~~~~~~~*/

    //Set Unit
    void SetUnit(UnitDialog *unit) {unit->SetUnit(&unitDistance);}

    //Set EyeX value
    void SetEyeX(GLWidget *widget, const float AddValue);

    //Set EyeY value
    void SetEyeY(GLWidget *widget, const float AddValue);

    //Set EyeZ value
    void SetEyeZ(GLWidget *widget, const float AddValue);

    //Set CenterX value
    void SetCenterX(GLWidget* widget, const float AddValue);

    //Set CenterY value
    void SetCenterY(GLWidget* widget, const float AddValue);

    //Set CenterZ value
    void SetCenterZ(GLWidget* widget, const float AddValue);

    //Set UpX value
    void SetUpX(GLWidget* widget, const float AddValue);

    //Set UpY value
    void SetUpY(GLWidget* widget, const float AddValue);

    //Set UpZ value
    void SetUpZ(GLWidget* widget, const float AddValue);

    //Set Play value
    void SetPlay(GLWidget* widget);

    //Set Velocity value
    void SetVelocity(GLWidget* widget, const int vel);

    //Rotation X
    void RotateX(const float rot);

    //Rotation Y
    void RotateY(const float rot);

    //Rotation Z
    void RotateZ(const float rot);

    //Set MinThreshold value
    void SetMinThreshold(GLWidget* widget, const float value) {widget->minThreshold = value;}

    //Set MaxThreshold value
    void SetMaxThreshold(GLWidget* widget, const float value) {widget->maxThreshold = value;}

    //Set Zoom
    void SetZoom(GLWidget* widget, const float value);

    //Set Axis X
    void SetAxisX(GLWidget* widget, const float value);

    //Set Axis Y
    void SetAxisY(GLWidget* widget, const float value);

    //Set Axis Z
    void SetAxisZ(GLWidget* widget, const float value);

    //Set Grid XY value
    void SetGridXYView(GLWidget* widget, const bool state) {widget->gridXY_is_on = state;}

    //Set Grid XZ value
    void SetGridXZView(GLWidget* widget, const bool state) {widget->gridXZ_is_on = state;}

    //Set Grid YZ value
    void SetGridYZView(GLWidget* widget, const bool state) {widget->gridYZ_is_on = state;}

    //-------------------------------------------------------------------------------//

    /*~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* Cloud/Cluster Functions */
    /*~~~~~~~~~~~~~~~~~~~~~~~~~*/

    //Return cluster
    KMeans GetCluster() {return cluster;}

    //Return cluster size
    int GetClusterSize() {return clusterSize;}

    //Return view state
    bool GetView(const int index) {return cluster.GetCluster(index).view;}

    //Set Point Cloud and Create a Cluster (from the first Point Cloud Frame)
    void SetCloud(GLWidget* widget);

    //Clean Memory (delete point cloud)
    void CleanUpClouds();

    //Clean Memory (delete cluster)
    void CleanUpClusters();

    //Recreate Clusters
    void SetClusterNumber(GLWidget* widget, const int clus);

    //Set view state (if this cluster must be drawn to screen or not)
    void ChangeView(std::string name, bool state);

    //Open Dialog window and change cluster viewing options (color, point size etc)
    void ChangeClusterFromDialog(GLWidget* widget, ClusterOptions* dialog, std::string name = "");

    //-------------------------------------------------------------------------------//

    /*~~~~~~~~~~~~~~~~~*/
    /* Model Functions */
    /*~~~~~~~~~~~~~~~~~*/

    //Create Model
    void CreateModel(GLWidget* widget, ModelCreateDialog* dialog, bool *state);

    //Set Bones
    void setBones(GLWidget* widget, SetBones* dialog, bool* state);

    //Create Bones
    bool CreateBones(GLWidget *widget);

    //Set Bone View State
    void SetBoneViewState(GLWidget *widget, const bool state);

    //Return Model Index
    int ModelSize() {return modelSize;}

    //Return Model
    Model ModelReturn() {return model;}

    //Clean Up Model Bones
    void CleanUpModelBones() {model.CleanUpBones(&model);}

    //Return Bone View
    bool BoneView() {return boneViewState;}

private:
    QTimer GLtimer;
    bool axesExist;
    float axis_X;
    float axis_Y;
    float axis_Z;

    bool gridXY_is_on;
    bool gridXZ_is_on;
    bool gridYZ_is_on;

    float unitDistance;

    //Screen Options
    GLdouble eyeX;
    GLdouble eyeY;
    GLdouble eyeZ;

    GLdouble centerX;
    GLdouble centerY;
    GLdouble centerZ;

    GLdouble upX;
    GLdouble upY;
    GLdouble upZ;

    GLfloat rotateX;
    GLfloat rotateY;
    GLfloat rotateZ;
    GLfloat rotNumX;
    GLfloat rotNumY;
    GLfloat rotNumZ;

    bool ChangeScreen;
    GLdouble zoom;
    GLdouble maxView;

    GLint width;
    GLint height;

    bool play;
    int velocity;
    float minThreshold;
    float maxThreshold;

    //C3D
    bool C3D_IsOpen;
    Read_C3D* c3d_f;
    int C3DframeNum;
    float C3DMultiplier;

    //Cloud
    bool cloudExists;
    int cloudSize;
    int pointPerCloudFrame;
    Cloud* cloud;
    GeoCentroid* cloudCentroids;

    //Cluster
    bool clusterExists;
    int clusterSize;
    KMeans cluster;

    //Model
    bool modelExists;
    int modelSize;
    Model model;

    //ModelBuffers
    std::string bufModelName;
    std::string bufClusterName;
    bool boneViewState;
    bool bufBoneState;

    /*************/
    /* Functions */
    /*************/

    //Set Viewing defaults
    void SetScreenViewingDefaults();

    //Set C3D defaults
    void SetC3Ddefaults();

    //Set Cloud/Cluster defaults
    void SetCloudClusterDefaults();

    //Draw C3D
    void DrawC3D();

    //Draw Grid
    void DrawGrid();

    //Draw Axes
    void DrawAxes();
};

#endif // GLWIDGET_H
