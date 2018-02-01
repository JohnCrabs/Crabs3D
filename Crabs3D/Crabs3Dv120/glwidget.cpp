#include "glwidget.h"
#include <math.h>
#include <QDebug>

#define RADPERDEG 0.017453293

/**********/
/* Public */
/**********/

GLWidget::GLWidget(QWidget *parent):
    QGLWidget(parent)
{
    connect(&GLtimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    GLtimer.start(30);

    //Set Viewing Defaults
    SetScreenViewingDefaults();

    //Set C3D Defaults
    SetC3Ddefaults();

    //Set Cloud/Cluster Defaults
    SetCloudClusterDefaults();
}

//Initialize Open GL - Set Some Parameter
void GLWidget::initializeGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); //Set screen color to black
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST); //Enable depth
    //glEnable(GL_LIGHT0); //Enable light and choose GL_LIGHT0
    //glEnable(GL_LIGHTING); //Enable Lighting
    //glEnable(GL_COLOR_MATERIAL); //Enable Color Material
}

//Draw to the OpenGl window
void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear COLOR buffer and DEPTH buffer

    //If ChangeScreen = true (this means resizing window)
    if(ChangeScreen) {
        resizeGL(width, height); //Run once again resizeGL
        ChangeScreen = false; //Set ChangeScreen value to false
    }

    if(axesExist) {
        DrawAxes();
    }

    //DrawGrid
    DrawGrid();

    //If C3D file is open (the only format on version 1.20)
    if(C3D_IsOpen == true) {
        DrawC3D(); //Draw C3D
    }
}

//When window resized
void GLWidget::resizeGL(int w, int h){
    width = w;
    height = h;

    glViewport(0, 0, (GLint)w, (GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(zoom, (float)w/h, 0.001, maxView);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /* gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble centerY, GLdouble centerZ)
     *
     * eyeX, eyeY, eyeZ -> Specifies the position of the eye point.
     * centerX, centerY, centerZ -> Specifies the position of the reference point.
     * upX, upY, upZ -> Specifies the direction of the up vector.
     *
     */
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

    glRotatef(rotateX, rotNumX, 0.0, 0.0);
    glRotatef(rotateY, 0.0, rotNumY, 0.0);
    glRotatef(rotateZ, 0.0, 0.0, rotNumZ);
}

//Grid
void GLWidget::Axes() {
    axesExist = true;
}

//-------------------------------------------------------------------------------//

/*~~~~~~~~~~~~~~~*/
/* C3D Functions */
/*~~~~~~~~~~~~~~~*/

//Read C3D file
bool GLWidget::ReadC3D(std::string fileName, QWidget *widget) {
    //If fileName is not NULL
    if(fileName.c_str() != NULL) {
        //If C3D is open
        if(C3D_IsOpen == true) {
            c3d_f->CleanUp(c3d_f); //clean memory
        }
        C3D_IsOpen = true; //set C3D_IsOpen = true
        c3d_f = (Read_C3D*)malloc(1*sizeof(Read_C3D)); //allocate memory
        c3d_f->Import(fileName, widget, c3d_f); //Import C3D file

        C3DMultiplier = c3d_f->POINT().MultiplierForMeters(c3d_f->POINT()); //Set C3D scaling value (transform units to meters)
        C3DframeNum = 0; //Set frameNum to 0 (frame Start)

        return true; //C3D file has been read
    }
    return false; //fileName is NULL
}

//Export C3D File
bool GLWidget::ExportC3D(std::string fileName, const GLWidget::c3d IsType) {
    //If C3D_IsOpen
    if(C3D_IsOpen) {
        //Check the type
        switch (IsType) {
        case c3d::IsHeader: {
            c3d_f->printHeaderFile(fileName); //export Header
            return true;
            break;
        } case c3d::IsParameter: {
            c3d_f->printParameterFile(fileName); //export Parameter
            return true;
            break;
        } case c3d::Is3DPoints: {
            c3d_f->printPointFile(fileName); //export 3D Point
            return true;
            break;
        } case c3d::IsAnalog: {
            c3d_f->printAnalogFile(fileName); //export Analog
            return true;
            break;
        }
        default:
            return false; //default return false (for safety)
            break;
        }
    } else {
        return false; //else if C3D_IsOpen = false return false
    }
}

//Clean C3D file
void GLWidget::CleanUpC3D() {
    //if C3D is open (the only format in version 1.20)
    if(C3D_IsOpen) {
        C3D_IsOpen = false; //set C3D_IsOpen value to false
        c3d_f->CleanUp(c3d_f); //free memory
    }
}

//-------------------------------------------------------------------------------//

/*~~~~~~~~~~~~~~~~~~~*/
/* Viewing Functions */
/*~~~~~~~~~~~~~~~~~~~*/

//Set EyeX value
void GLWidget::SetEyeX(GLWidget* widget, const float AddValue) {
    widget->eyeX += AddValue;
    ChangeScreen = true;
}

//Set EyeY value
void GLWidget::SetEyeY(GLWidget* widget, const float AddValue) {
    widget->eyeY += AddValue;
    ChangeScreen = true;
}

//Set EyeZ value
void GLWidget::SetEyeZ(GLWidget* widget, const float AddValue) {
    widget->eyeZ += AddValue;
    ChangeScreen = true;
}

//Set CenterX value
void GLWidget::SetCenterX(GLWidget* widget, const float AddValue) {
    widget->centerX += AddValue;
    ChangeScreen = true;
}

//Set CenterY value
void GLWidget::SetCenterY(GLWidget* widget, const float AddValue) {
    widget->centerY += AddValue;
    ChangeScreen = true;
}

//Set CenterZ value
void GLWidget::SetCenterZ(GLWidget* widget, const float AddValue) {
    widget->centerZ += AddValue;
    ChangeScreen = true;
}

//Set UpX value
void GLWidget::SetUpX(GLWidget* widget, const float AddValue) {
    widget->upX += AddValue;
    ChangeScreen = true;
}

//Set UpY value
void GLWidget::SetUpY(GLWidget* widget, const float AddValue) {
    widget->upY += AddValue;
    ChangeScreen = true;
}

//Set UpZ value
void GLWidget::SetUpZ(GLWidget* widget, const float AddValue) {
    widget->upZ += AddValue;
    ChangeScreen = true;
}

//Set Play value
void GLWidget::SetPlay(GLWidget* widget) {
    if(widget->play == true) {
        widget->play = false;
    } else {
        widget->play = true;
    }
}

//Set Velocity value
void GLWidget::SetVelocity(GLWidget* widget, const int vel) {
    widget->velocity = vel;
}

//Rotation X
void GLWidget::RotateX(const float rot) {
    rotateX += rot;
    if(rotateX >= 360) {
        rotateX -= 360;
    } else if(rotateX <= 0) {
        rotateX += 360;
    }

    ChangeScreen = true;
}

//Rotation Y
void GLWidget::RotateY(const float rot) {
    rotateY += rot;
    if(rotateY >= 360) {
        rotateY -= 360;
    } else if(rotateY <= 0) {
        rotateY += 360;
    }
    ChangeScreen = true;
}

//Rotation Z
void GLWidget::RotateZ(const float rot) {
    rotateZ += rot;
    if(rotateZ >= 360) {
        rotateZ -= 360;
    } else if(rotateZ <= 0) {
        rotateZ += 360;
    }

    ChangeScreen = true;
}

//Set Zoom
void GLWidget::SetZoom(GLWidget* widget, const float addValue) {
    widget->zoom += addValue;

    if(widget->zoom < 15) {
        widget->zoom = 15;
    } else if(widget->zoom > 150) {
        widget->zoom = 150;
    }

    ChangeScreen = true;
}

//Set Axis X
void GLWidget::SetAxisX(GLWidget* widget, const float value) {
    widget->axis_X += value;
}

//Set Axis Y
void GLWidget::SetAxisY(GLWidget* widget, const float value) {
    widget->axis_Y += value;
}

//Set Axis Z
void GLWidget::SetAxisZ(GLWidget* widget, const float value) {
    widget->axis_Z += value;
}

//-------------------------------------------------------------------------------//

/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Cloud/Cluster Functions */
/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

//Set Point Cloud and Create a Cluster (from the first Point Cloud Frame)
void GLWidget::SetCloud(GLWidget* widget) {

    //If cloudExists
    if(widget->cloudExists) {
        widget->CleanUpClouds(); //Clean memory
    }

    //If C3D is open (in version 1.20 is the only format)
    if(widget->C3D_IsOpen) {
        //Set frameNum and pointNum
        widget->cloudSize = widget->c3d_f->Header().LastFrame() - widget->c3d_f->Header().FirstFrame() + 1;
        widget->pointPerCloudFrame = widget->c3d_f->Header().NumberOfPoints();
    }
    widget->cloudExists = true; //Set cloudExists value to true


    widget->cloud = (Cloud*)malloc(cloudSize*sizeof(Cloud)); //allocate memory for the new cloud
    //Loop through cloudSize and allocate memory for cloud variables
    for(int i = 0; i < cloudSize; i++) {
        widget->cloud[i].id = (int*)malloc(pointPerCloudFrame*sizeof(int));
        widget->cloud[i].x = (float*)malloc(pointPerCloudFrame*sizeof(float));
        widget->cloud[i].y = (float*)malloc(pointPerCloudFrame*sizeof(float));
        widget->cloud[i].z = (float*)malloc(pointPerCloudFrame*sizeof(float));

        widget->cloud[i].name = new std::string[pointPerCloudFrame];
    }

    //Loop through cloudSize
    for(int i = 0; i < widget->cloudSize; i++) {
        //Loop through pointNum
        for (int j = 0; j < widget->pointPerCloudFrame; j++) {
            widget->cloud[i].id[j] = j+1; //Set cloud id (this is useful for tracking points - id starts from 1, not from 0)
            //If C3D is Open (in version 1.20 is the only format)
            if(widget->C3D_IsOpen) {
                GeoPoint geo; //Create a GeoPoint to store correct (x,y,z) values
                //Set GeoPoint values
                geo = c3d_f->POINT().CheckScreens(c3d_f->POINT(), c3d_f->Data().Frame(i).Point(j).X(), c3d_f->Data().Frame(i).Point(j).Y(), c3d_f->Data().Frame(i).Point(j).Z(), c3d_f->Data().Relocation(0).DR(j));
                widget->cloud[i].x[j] = geo.x*C3DMultiplier; //scale x point
                widget->cloud[i].y[j] = geo.y*C3DMultiplier; //scale y point
                widget->cloud[i].z[j] = geo.z*C3DMultiplier; //scale z point

                //widget->cloud[i].name[j] = "";
                widget->cloud[i].name[j] = widget->c3d_f->POINT().Labels(j); //Set label for each point (in every frame)
            }
        }
    }

    //if clusterExists
    if(widget->clusterExists) {
        widget->CleanUpClusters(); //clean up memory
    }

    widget->clusterExists = true; //set clusterExists value to true
    widget->cluster.SetCluster(cloud[0], clusterSize, pointPerCloudFrame); //create clusters from the first cloud frame (this frame always exist)

    widget->cloudCentroids = (GeoCentroid*)malloc(widget->cloudSize*sizeof(GeoCentroid));

    for(int i = 0; i < widget->cloudSize; i++) {
         widget->cloudCentroids[i].x = 0;
         widget->cloudCentroids[i].y = 0;
         widget->cloudCentroids[i].z = 0;

        //Loop through pointNum
        for (int j = 0; j < widget->pointPerCloudFrame; j++) {
            if(widget->C3D_IsOpen) {
                widget->cloudCentroids[i].x += widget->cloud[i].x[j];
                widget->cloudCentroids[i].y += widget->cloud[i].y[j];
                widget->cloudCentroids[i].z += widget->cloud[i].z[j];
            }
        }
        widget->cloudCentroids[i].x /= widget->pointPerCloudFrame;
        widget->cloudCentroids[i].y /= widget->pointPerCloudFrame;
        widget->cloudCentroids[i].z /= widget->pointPerCloudFrame;
    }


}

//Clean Memory (delete point cloud)
void GLWidget::CleanUpClouds() {
    //if cloudExist
    if(cloudExists == true) {
        //Loop through cloud and free memory
        for(int i = 0; i < cloudSize; i++) {
            free(cloud[i].id);
            free(cloud[i].x);
            free(cloud[i].y);
            free(cloud[i].z);
            delete[] cloud[i].name;
        }
        free(cloud); //then free memory from cloud
        cloudExists = false; //Set cloudExists value to false (there are no more point clouds) :-(
    }
}

//Clean Memory (delete point cloud)
void GLWidget::CleanUpClusters() {
    //If clusterExists
    if(clusterExists == true) {
        cluster.CleanUp(); //Clean memory from clusters
        clusterExists = false; //Set clusterExists value to false (there are no more clusters) :-(
    }
}

//Recreate Clusters
void GLWidget::SetClusterNumber(GLWidget* widget, const int clus) {
    widget->clusterSize = clus; //Set the size of the clusters

    //If a point cloud exists
    if(widget->cloudExists) {
        widget->CleanUpClusters(); //Clean memory from previous clusters (to create new clusters)

        widget->clusterExists = true; //Set clusterExists value to true
        //At last create the clusters (with kmeans method)
        widget->cluster.SetCluster(widget->cloud[0], widget->clusterSize, widget->c3d_f->Header().NumberOfPoints());
    }
}

//Set view state (if this cluster must be drawn to screen or not)
void GLWidget::ChangeView(std::string name, bool state) {
    //Loop throught clusters
    for (int i = 0; i < clusterSize; i++) {
        //Check if the ClusterName is the one we want
        if(name == cluster.GetCluster(i).name[0]) {
            cluster.SetClusterView(i, state); //If it is set view state
            break; //break the loop (there is no point to loop till the end)
        }
    }
}

//Open Dialog window and change cluster viewing options (color, point size etc)
void GLWidget::ChangeClusterFromDialog(GLWidget* widget, ClusterOptions* dialog, std::string name) {
    //User input new cluster values (name, color, point size)
    dialog->ClusterTypeOptions(&widget->cluster, widget->clusterSize, clusterExists, name);

}

//Create Model
void GLWidget::CreateModel(GLWidget* widget, ModelCreateDialog* dialog, bool* state) {
    dialog->ModelCreate(&widget->model, state);
}

//Set Bones
void GLWidget::setBones(GLWidget* widget, SetBones* dialog, bool *state) {

    if(widget->clusterExists) {
        dialog->Bones(&widget->bufClusterName, &widget->bufBoneState, widget->cluster, widget->clusterSize, state);

        //qDebug() << QString::fromUtf8(widget->bufClusterName.c_str());
    }
}

//Create Bones
bool GLWidget::CreateBones(GLWidget* widget) {
    int index = 0;
    for(int i = 0; i < clusterSize; i++) {
        if(widget->cluster.GetCluster(i).name[0] == bufClusterName) {
            index = i;
            break;
        }
    }
    widget->model.CreateModelFromCluster(widget->cluster.GetCluster(index), widget->cloud, widget->cloudSize, widget->pointPerCloudFrame, &widget->model);

    return true;
}

//Set Bone View State
void GLWidget::SetBoneViewState(GLWidget *widget, const bool state) {
    widget->model.SetBoneViewState(state);
    widget->boneViewState = state;
}

/*******************************************************************************************************************/
/*******************************************************************************************************************/

/***********/
/* Private */
/***********/

//Set Viewing defaults
void GLWidget::SetScreenViewingDefaults() {

    //A good looking camera possition is as (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ) are set
    axesExist = false;
    axis_X = 0;
    axis_Y = 0;
    axis_Z = 0;

    gridXY_is_on = false;
    gridXZ_is_on = true;
    gridYZ_is_on = false;

    unitDistance = 1.0; //meters

    eyeX = 0.0;
    eyeY = 4.0;
    eyeZ = 6.0; //6

    centerX = 0.0;
    centerY = 0.0;
    centerZ = 0.0;

    upX = 0.0;
    upY = 4.0; //1
    upZ = 0.0;

    rotateX = 0.0;
    rotateY = 0.0;
    rotateZ = 0.0;
    rotNumX = 0.5;
    rotNumY = 0.5;
    rotNumZ = 0.5;

    zoom = 40.0;
    maxView = 50.0;

    //-------------------//

    ChangeScreen = false; //Screen hasn't been resized yet ;)
    boneViewState = false;

    play = false; //When data has been load we want to start from frame 0, so play must be set to fault
    velocity = 1; //Starting velocity is 1 (velocity is frame step and must never be set to 0 - max is 10 in version 1.20)

    minThreshold = 0.0; //Starting histogram min Threshold is 0 (all points are visible)
    maxThreshold = 10.0; //Starting histogram max Threshold is 10 (a good looking value - it works fine in version 1.20)
}

//Set C3D defaults
void GLWidget::SetC3Ddefaults() {
    C3D_IsOpen = false; //C3D is not open yet (and may not be opened - this is not true in version 1.20)
    C3DframeNum = 0; //default value to frameNum is 0
    C3DMultiplier = 1.0; //We assume that the C3D data is in meters (till we learn the true scale of measurements - usually is mm)
}

//Set Cloud/Cluster defaults
void GLWidget::SetCloudClusterDefaults() {
    cloudExists = false; //Cloud doesn't exist when software starts
    clusterExists = false; //Also cluster doesn't exist when software starts
    clusterSize = 1; //However cluster size must be set to 1 (there is no point for 0 clusters)

    modelSize = 0; //Set Model Size
}

//Draw C3D
void GLWidget::DrawC3D() {
    GeoPoint geo; //Create a GeoPoint (this was useful on version 1.10, in version 1.20 only dr value is useful)

    //Loop through c3d Points
    int pointSizeMax = c3d_f->Header().NumberOfPoints();
    for(register int i = 0; i < pointSizeMax; i++) {

        //Set GeoPoin Values (only dr is useful in version 1.20)
        geo = c3d_f->POINT().CheckScreens(c3d_f->POINT(), c3d_f->Data().Frame(C3DframeNum).Point(i).X(), c3d_f->Data().Frame(C3DframeNum).Point(i).Y(), c3d_f->Data().Frame(C3DframeNum).Point(i).Z(), c3d_f->Data().Relocation(0).DR(i));
        geo.dr*= C3DMultiplier; //Scale geo.dr to meters

        //If geo.dr is between histogram min and max values then draw the point (geo.dr = | PotisionPoint(x,y,z)_lastFrame - PotisionPoint(x,y,z)_firstFrame |)
        if(geo.dr >= minThreshold && geo.dr <= maxThreshold) {
            //Loop through clusters
            for(register int k = 0; k < clusterSize; k++) {
                //Set Point Size to draw
                glPointSize(cluster.GetCluster(k).pointSize);
                //Loop through points inside the cluster
                int clusterSizeMax = cluster.GetCluster(k).size;
                bool clusterView = cluster.GetCluster(k).view;
                for(register int j = 0; j < clusterSizeMax; j++) {
                    //If the cloud point belongs to the cluster
                    if(cloud[C3DframeNum].id[i] == cluster.GetCluster(k).cloud.id[j]) {
                        //AND if ClusterView is true (cluster box is checked)
                        if(clusterView) {
                            glBegin(GL_POINTS);
                            //Set the color to draw
                            glColor3f(cluster.GetCluster(k).color.red, cluster.GetCluster(k).color.green, cluster.GetCluster(k).color.blue);
                            //Draw the point
                            glVertex3f(cloud[C3DframeNum].x[i]*unitDistance, cloud[C3DframeNum].y[i]*unitDistance, cloud[C3DframeNum].z[i]*unitDistance);
                            glEnd();
                        }
                    }
                }
            }

            if(boneViewState) {
                glLineWidth(1);
                glColor3f(model.GetColorRed()/255.0, model.GetColorGreen()/255.0, model.GetColorBlue()/255.0);

                int boneIndexMax = model.GetBones().boneSize;
                //int clusterIndexMax = model.GetBones().boneSize

                for(register int boneIndex = 0; boneIndex < boneIndexMax; boneIndex++) {
                    int newConnectionsMax = model.GetBones().bones[boneIndex].newConnectionsSize;
                    for(register int clusterIndex = 0; clusterIndex < boneIndexMax; clusterIndex++) {
                        for(register int newConnections = 0; newConnections < newConnectionsMax; newConnections++) {
                            if(model.GetBones().bones[boneIndex].idNewConnections[newConnections] == model.GetBones().points[C3DframeNum].id[clusterIndex]) {
                                glBegin(GL_LINES);
                                //Draw the point
                                glVertex3f(model.GetBones().points[C3DframeNum].x[boneIndex]*unitDistance, model.GetBones().points[C3DframeNum].y[boneIndex]*unitDistance, model.GetBones().points[C3DframeNum].z[boneIndex]*unitDistance);
                                glVertex3f(model.GetBones().points[C3DframeNum].x[clusterIndex]*unitDistance, model.GetBones().points[C3DframeNum].y[clusterIndex]*unitDistance, model.GetBones().points[C3DframeNum].z[clusterIndex]*unitDistance);
                                glEnd();
                            }
                        }
                    }
                }
            }
        }
        glFlush();
    }

    //If frameNumber (index) is negative (this must never happens - this check is for safety only)
    if(C3DframeNum < 0) {
        C3DframeNum = 0; //Set frameNumber (index) to 0
    } else { //Else
        //If play is true
        if(play)
            C3DframeNum += velocity; //Add velocity value to frameNum (index)

        //If frameNum is higher than the FrameSize (this means segmentation fault)
        if(C3DframeNum > (c3d_f->Header().LastFrame() - c3d_f->Header().FirstFrame() - 1))
            C3DframeNum = 0; //Reset the frameNum (index) to 0 (start again)

    }
}

//Draw Grid
void GLWidget::DrawGrid() {
    float xsize = 0.0;
    float ysize = 0.0;

    glLineWidth(1);

    for(int i = 0; i < 100; i++) {
        xsize = 0;
        for(int j = 0; j < 100; j++) {

            if(gridXY_is_on) {
                glColor3f(0.2, 0.2, 0.1);
                glBegin(GL_POLYGON);
                    glVertex3f(-50.0+xsize, -50.0+ysize, 0.0);
                    glVertex3f(-40.0+xsize, -50.0+ysize, 0.0);
                    glVertex3f(-40.0+xsize, -40.0+ysize, 0.0);
                    glVertex3f(-50.0+xsize, -40.0+ysize, 0.0);
                glEnd();
            }
            if(gridXZ_is_on) {
                glColor3f(0.2, 0.0, 0.2);
                glBegin(GL_POLYGON);
                    glVertex3f(-50.0+xsize, 0.0, -50.0+ysize);
                    glVertex3f(-40.0+xsize, 0.0, -50.0+ysize);
                    glVertex3f(-40.0+xsize, 0.0, -40.0+ysize);
                    glVertex3f(-50.0+xsize, 0.0, -40.0+ysize);
                glEnd();
            }
            if(gridYZ_is_on) {
                glColor3f(0.0, 0.2, 0.2);
                glBegin(GL_POLYGON);
                    glVertex3f(0.0, -50.0+xsize, -50.0+ysize);
                    glVertex3f(0.0, -40.0+xsize, -50.0+ysize);
                    glVertex3f(0.0, -40.0+xsize, -40.0+ysize);
                    glVertex3f(0.0, -50.0+xsize, -40.0+ysize);
                glEnd();
            }


            xsize += 2.0;
        }
        ysize += 2.0;
    }
}

//Draw Axes
void GLWidget::DrawAxes()
{

    float dx =   axis_X * cos(rotateY*RADPERDEG) * cos(rotateZ*RADPERDEG) + axis_Y * sin(rotateZ*RADPERDEG) - axis_Z * sin(rotateY*RADPERDEG);  //  drXZ * cos(rotY) * cos(rotZ) + drXY * sin(rotZ) - drYZ * sin(rotY)
    float dy = - axis_X * sin(rotateZ*RADPERDEG) + axis_Y * cos(rotateZ*RADPERDEG) * cos(rotateX*RADPERDEG) + axis_Z * sin(rotateX*RADPERDEG);  //- drXZ * sin(rotZ) + drXY * cos(rotZ) * cos(rotX) + drYZ * sin(rotY)
    float dz =   axis_X * sin(rotateY*RADPERDEG) - axis_Y * sin(rotateX*RADPERDEG) + axis_Z * cos(rotateY*RADPERDEG) * cos(rotateX*RADPERDEG);  //  drXZ * sin(rotY) - drXY * sin(rotX) - drYZ * cos(rotY) * cos(rotX)

    glLineWidth(3);
    glColor3f(1.0,0.0,0.0); // red x
        glBegin(GL_LINES);
        // x aix

        glVertex3f(-2.0f + dx, 0.0f + dy, 0.0f + dz);
        glVertex3f(2.0f + dx, 0.0f + dy, 0.0f + dz);

        // arrow
        glVertex3f(2.0f + dx, 0.0f + dy, 0.0f + dz);
        glVertex3f(1.0f + dx, 0.5f + dy, 0.0f + dz);

        glVertex3f(2.0f + dx, 0.0f + dy, 0.0f + dz);
        glVertex3f(1.0f + dx, -0.5f + dy, 0.0f + dz);
        glEnd();
        glFlush();

        // y
        glColor3f(0.0,1.0,0.0); // green y
        glBegin(GL_LINES);
        glVertex3f(0.0 + dx, -2.0f + dy, 0.0f + dz);
        glVertex3f(0.0 + dx, 2.0f + dy, 0.0f + dz);

        // arrow
        glVertex3f(0.0 + dx, 2.0f + dy, 0.0f + dz);
        glVertex3f(0.5 + dx, 1.0f + dy, 0.0f + dz);

        glVertex3f(0.0 + dx, 2.0f + dy, 0.0f + dz);
        glVertex3f(-0.5 + dx, 1.0f + dy, 0.0f + dz);
        glEnd();
        glFlush();

        // z
        glColor3f(0.0,0.0,1.0); // blue z
        glBegin(GL_LINES);
        glVertex3f(0.0 + dx, 0.0f + dy, -2.0f + dz);
        glVertex3f(0.0 + dx, 0.0f + dy, 2.0f + dz);

        // arrow
        glVertex3f(0.0 + dx, 0.0f + dy, 2.0f + dz);
        glVertex3f(0.0 + dx, 0.5f + dy, 1.0f + dz);

        glVertex3f(0.0 + dx, 0.0f + dy, 2.0f + dz);
        glVertex3f(0.0 + dx, -0.5f + dy, 1.0f + dz);
        glEnd();
        glFlush();
}
