#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QString>
#include <QTimer>
#include <kmeans.h>

#include "model_create_dialog.h"
#include "cluster_options.h"
#include "unit_dialog.h"
#include "set_bones.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    //Set the clock
    void Clock();

    //Refresh the item list when exists
    void RefreshList();

    //Refresh model item list
    //void RefreshModelList();

private:
    //Add Items to list
    void AddToList(QString itemName, Color color, QListWidget* widget, bool viewState);
    //Clear the List
    void ClearList(QListWidget* widget);
    //Add Items to Model list
    void AddToModelList(Model model, QListWidget* widget, bool viewState);

    Ui::MainWindow *ui;

    //Timer
    QTimer *ClockTimer;
    QTimer ColorTimer;
    QTimer ModelTimer;

    //Dialogs
    ClusterOptions *dialog;
    ModelCreateDialog *modelCreateDialog;
    UnitDialog* unitDialog;
    SetBones* setBones;

    bool listExists;
    bool modelExists;
    bool boneExists;
    bool checkModelExistence;
    int listSize;

private slots:

    //Exit Program -> When triggered
    void on_actionExit_triggered();

    //Open C3D file -> When triggered
    void on_actionC3D_triggered();

    //Velocity Change -> When valueChanged
    void on_horizontalSlider_valueChanged(int value);

    //Minimum Histogram Threshold -> When valueChanged
    void on_MinThreshold_valueChanged(double arg1);

    //Maximum Histogram Threshold -> When valueChanged
    void on_MaxThreshold_valueChanged(double arg1);

    //Save C3D Header section to TXT -> When triggered
    void on_actionHeader_TXT_triggered();

    //Save C3D Parameter section to TXT -> When triggered
    void on_actionParameter_TXT_triggered();

    //Save C3D Header section to CSV -> When triggered
    void on_action3D_Points_TXT_triggered();

    //Save C3D Header section to CSV -> When triggered
    void on_actionAnalog_data_TXT_triggered();

    //About Info -> When triggered
    void on_actionAbout_Crabs3D_triggered();

    //Check the Number of Clusters -> When valueChanged
    void on_ClusterNumber_valueChanged(int arg1);

    //Check the list of the clusters -> itemChanged
    void on_ClusterList_itemChanged(QListWidgetItem *item);

    //Open Cluster Options dialog -> When triggered
    void on_actionCluster_Options_triggered();

    //Open CrabsEditor -> When triggered
    void on_actionCrabsEditor_triggered();

    //Open Cluster Options dialog -> When itemPressed
    void on_ClusterList_itemPressed(QListWidgetItem *item);

    //Open Model Create Dialog -> When triggered
    void on_actionModelCreate_triggered();

    //Move UP -> When triggered
    void on_actionUp_triggered();

    //Move DOWN -> When triggered
    void on_actionDown_triggered();

    //Move LEFT -> When triggered
    void on_actionLeft_triggered();

    //Move RIGHT -> When clicked
    void on_actionRight_triggered();

    //X Rotation + -> When triggered
    void on_actionX_Rotation_triggered();

    //X Rotation - -> When triggered
    void on_actionX_Rotation_2_triggered();

    //Y Rotation + -> When triggered
    void on_actionY_Rotation_triggered();

    //Y Rotation - -> When triggered
    void on_actionY_Rotation_2_triggered();

    //Z Rotation + -> When triggered
    void on_actionZ_Rotation_triggered();

    //Z Rotation - -> When triggered
    void on_actionZ_Rotation_2_triggered();

    //Play or Pause Motion -> When triggered
    void on_actionPlay_Pause_triggered();

    //Zoom In -> When triggered
    void on_actionZoom_In_triggered();

    //Zoom Out -> When triggered
    void on_actionZoom_Out_triggered();

    //Move Front -> When triggered
    void on_actionFront_triggered();

    //Move Back -> When triggered
    void on_actionBack_triggered();

    //Show XY grid
    void on_actionXY_grid_triggered();

    //Show XZ grid
    void on_actionXZ_grid_triggered();

    //Show YZ grid
    void on_actionYZ_grid_triggered();

    //Show XY grid
    void on_XY_CheckBox_stateChanged(int arg1);

    //Show XZ grid
    void on_XZ_CheckBox_stateChanged(int arg1);

    //Show YZ grid
    void on_YZ_CheckBox_stateChanged(int arg1);

    //Set Unit -> When triggered
    void on_actionUnits_triggered();

    //Set Bones -> When triggered
    void on_actionSetBones_triggered();

    //Create Bones -> When triggered
    void on_actionCreate_Bones_triggered();
    void on_ModelList_itemChanged(QListWidgetItem *item);
};

#endif // MAIN_WINDOW_H
