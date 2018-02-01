#include "main_window.h"
#include "ui_main_window.h"
#include "crabs_editor.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QTime>
#include <QDir>
#include <QBrush>

#define ALL_FORMAT "All Files (*.*)"
#define TXT_FORMAT "Text File (*.txt)"
#define CSV_FORMAT "Comma Separated Values File (*.csv)"
#define C3D_FORMAT "Coordinate 3D File (*.c3d)"


/**********/
/* Public */
/**********/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Create Clock in Status Bar
    ClockTimer = new QTimer(this);
    connect(ClockTimer, SIGNAL(timeout()), this, SLOT(Clock()));
    ClockTimer->start(1000);
    ui->statusBar->addPermanentWidget(ui->clockLabel);
    ui->statusBar->addPermanentWidget(ui->spaceLabel);

    //Create list
    listExists = false;
    connect(&ColorTimer, SIGNAL(timeout()), this, SLOT(RefreshList()));
    ColorTimer.start(60);

    //Model
    modelExists = false;
    boneExists = false;
    checkModelExistence = true;

    //Dialogs
    dialog = new ClusterOptions; //Create Cluster Options Dialog
    modelCreateDialog = new ModelCreateDialog; //Create Model Create Dialog
    unitDialog = new UnitDialog; //Create Unit Dialog
    setBones = new SetBones; //Create Set Bones Dialog

    //Axis
    ui->AxisWidget->Axes();
    ui->AxisWidget->SetGridXYView(ui->AxisWidget, true);
    ui->AxisWidget->SetGridXZView(ui->AxisWidget, true);
    ui->AxisWidget->SetGridYZView(ui->AxisWidget, true);

}

MainWindow::~MainWindow()
{ 
    ui->ViewWidget->CleanUpC3D();
    ui->ViewWidget->CleanUpClusters();
    ui->ViewWidget->CleanUpClouds();
    ui->ViewWidget->CleanUpModelBones();

    delete modelCreateDialog;
    delete unitDialog;
    delete setBones;

    delete ui;
}

/****************/
/* Public Slots */
/****************/

//Set the clock
void MainWindow::Clock()
{
    QTime time = QTime::currentTime();
    QString time_next = time.toString("hh : mm : ss");
    ui->clockLabel->setText(time_next);
    ui->clockLabel->show();
}

//Refresh the item list when exists
void MainWindow::RefreshList() {
    if(listExists) {
        ClearList(ui->ClusterList);
        for(int i=0; i < ui->ViewWidget->GetClusterSize(); i++) {
            QString name = QString::fromUtf8(ui->ViewWidget->GetCluster().GetCluster(i).name[0].c_str());
            AddToList(name, ui->ViewWidget->GetCluster().GetCluster(i).color, ui->ClusterList, ui->ViewWidget->GetView(i));
        }
        listSize = ui->ViewWidget->GetClusterSize();
    }

    ClearList(ui->ModelList);
    if(ui->ViewWidget->ModelReturn().GetState()) {
        AddToModelList(ui->ViewWidget->ModelReturn(), ui->ModelList, ui->ViewWidget->BoneView());
    }

    if(checkModelExistence) {
        if(modelExists) {
            ui->actionSetBones->setEnabled(true);
            modelExists = false;
        }
        if(boneExists) {
            ui->actionCreate_Bones->setEnabled(true);
            boneExists = false;
        }
    }

}

/***********/
/* Private */
/***********/

//Add Items to list
void MainWindow::AddToList(QString itemName, Color color,  QListWidget* widget, bool viewState) {
    QListWidgetItem *item = new QListWidgetItem(itemName, widget);

    int red = color.red * 255; //Calculate red color
    int green = color.green * 255; //Calculate green color
    int blue = color.blue * 255; //Calculate blue color

    QBrush brushText(QColor(red, green, blue)); //Set forground color to clusters color
    QBrush brushBackground(QColor(0, 0, 0)); //Set background color to black
    item->setForeground(brushText); //Set foreground brush
    item->setBackground(brushBackground); //Set background brush
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
    if(viewState) {
        item->setCheckState(Qt::Checked); //Initialize check state
    } else {
        item->setCheckState(Qt::Unchecked); //Initialize uncheck state
    }

}

//Add Item to Model list
void MainWindow::AddToModelList(Model model, QListWidget* widget, bool viewState) {
    QString name = QString::fromUtf8(model.GetModelName().c_str());

    QListWidgetItem *item = new QListWidgetItem(name, widget);

    QBrush brushText(QColor(model.GetColorRed(), model.GetColorGreen(), model.GetColorBlue())); //Set forground color to clusters color
    QBrush brushBackground(QColor(0, 0, 0)); //Set background color to black
    item->setForeground(brushText); //Set foreground brush
    item->setBackground(brushBackground); //Set background brush

    if(model.GetBones().state) {
        if(viewState) {
            item->setCheckState(Qt::Checked); //Initialize check state
        } else {
            item->setCheckState(Qt::Unchecked); //Initialize uncheck state
        }
    }
}

//Clear the List
void MainWindow::ClearList(QListWidget* widget) {
   widget->clear();
}

/*****************/
/* Private Slots */
/*****************/

//Exit Program -> When triggered
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit(); //Quit the application
}

//Open C3D file -> When triggered
void MainWindow::on_actionC3D_triggered()
{
    QString filter = C3D_FORMAT;

    //set the file path
    QString openFilePath = QFileDialog::getOpenFileName(this, "Open", QDir::homePath(), filter, 0, QFileDialog::DontUseNativeDialog);

    //Check if the file path isn't null
    if(openFilePath != nullptr) {
        //Check if the file has been read correctly
       if(ui->ViewWidget->ReadC3D(openFilePath.toUtf8().constData(), this)) {
           ui->ViewWidget->SetCloud(ui->ViewWidget); //Create the point cloud and clusters

           //Add items to the list for the fisrt time
           for(int i=0; i < ui->ViewWidget->GetClusterSize(); i++) {
               QString name = QString::fromUtf8(ui->ViewWidget->GetCluster().GetCluster(i).name[0].c_str());
               AddToList(name, ui->ViewWidget->GetCluster().GetCluster(i).color, ui->ClusterList, ui->ViewWidget->GetView(i));
           }
           listSize = ui->ViewWidget->GetClusterSize(); //save the size of the list
           listExists = true; //set the flag

           //QMessageBox::information(this, "Open Successful", "File opening was successful!"); //this option is from a previous version
       }
    }
}

//Velocity Change -> When valueChanged
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->ViewWidget->SetVelocity(ui->ViewWidget, value); //Change velocity movement (step between frames)
}

//Minimum Histogram Threshold -> When valueChanged
void MainWindow::on_MinThreshold_valueChanged(double arg1)
{
    ui->ViewWidget->SetMinThreshold(ui->ViewWidget, arg1); //Set minimum histogram threshold
}

//Maximum Histogram Threshold -> When valueChanged
void MainWindow::on_MaxThreshold_valueChanged(double arg1)
{
    ui->ViewWidget->SetMaxThreshold(ui->ViewWidget, arg1); //Set maximum histogram threshold
}

//Save C3D Header section to TXT -> When triggered
void MainWindow::on_actionHeader_TXT_triggered()
{
    QString filter = ALL_FORMAT;

    QString openFilePath = QFileDialog::getSaveFileName(this, "Open", QDir::homePath(), filter, 0, QFileDialog::DontUseNativeDialog);

    if(openFilePath != nullptr) {
        if(ui->ViewWidget->ExportC3D(openFilePath.toUtf8().constData(), ui->ViewWidget->c3d::IsHeader))
            QMessageBox::information(this, "Export Successful", "File exporting was successful!");
    }
}

//Save C3D Parameter section to TXT -> When triggered
void MainWindow::on_actionParameter_TXT_triggered()
{
    QString filter = ALL_FORMAT;

    QString openFilePath = QFileDialog::getSaveFileName(this, "Open", QDir::homePath(), filter, 0, QFileDialog::DontUseNativeDialog);

    if(openFilePath != nullptr) {
        if(ui->ViewWidget->ExportC3D(openFilePath.toUtf8().constData(), ui->ViewWidget->c3d::IsParameter))
            QMessageBox::information(this, "Export Successful", "File exporting was successful!");
    }
}

//Save C3D Header section to CSV -> When triggered
void MainWindow::on_action3D_Points_TXT_triggered()
{
    QString filter = CSV_FORMAT;

    QString openFilePath = QFileDialog::getSaveFileName(this, "Open", QDir::homePath(), filter, 0, QFileDialog::DontUseNativeDialog);

    if(openFilePath != nullptr) {
        if(ui->ViewWidget->ExportC3D(openFilePath.toUtf8().constData(), ui->ViewWidget->c3d::Is3DPoints))
            QMessageBox::information(this, "Export Successful", "File exporting was successful!");
    }
}

//Save C3D Header section to CSV -> When triggered
void MainWindow::on_actionAnalog_data_TXT_triggered()
{
    QString filter = CSV_FORMAT;

    QString openFilePath = QFileDialog::getSaveFileName(this, "Open", QDir::homePath(), filter, 0, QFileDialog::DontUseNativeDialog);

    if(openFilePath != nullptr) {
        if(ui->ViewWidget->ExportC3D(openFilePath.toUtf8().constData(), ui->ViewWidget->c3d::IsAnalog))
            QMessageBox::information(this, "Export Successful", "File exporting was successful!");
    }
}

//About Info -> When triggered
void MainWindow::on_actionAbout_Crabs3D_triggered()
{
    QString text;

    text = "<p>Copyright (C) Feb 2018 <a href=\"https://github.com/JohnCrabs\">John Crabs</a> <br><b>kavouras.ioannis@gmail.com</b></p>";
    text += "<p>This program is free software: you can redistribute it and/or modify ";
    text += "it under the terms of the GNU General Public License as published by ";
    text += "the Free Software Foundation, either version 3 of the License, or ";
    text += "(at your option) any later version.</p>";
    text += "<p>This program is distributed in the hope that it will be useful, ";
    text += "but WITHOUT ANY WARRANTY; without even the implied warranty of ";
    text += "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the ";
    text += "GNU General Public License for more details.</p>";
    text += "<p>You should have received a copy of the GNU General Public License ";
    text += "along with this program.  If not, see <br><a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>.";

    QMessageBox::about(this, "About Crabs3D", text);
}

//Check the Number of Clusters -> When valueChanged
void MainWindow::on_ClusterNumber_valueChanged(int arg1)
{
    ui->ViewWidget->SetClusterNumber(ui->ViewWidget, arg1); //Set the number of clusters

    if(dialog->isVisible()) {
        dialog->setVisible(false);
    }
    if(setBones->isVisible()) {
        setBones->setVisible(false);
    }
}

//Check the list of the clusters -> itemChanged
void MainWindow::on_ClusterList_itemChanged(QListWidgetItem *item)
{
    std::string name = item->text().toUtf8().constData(); //Set item name
    bool state;

    //Chech if item->checkState is Unchecked or Not
    if(item->checkState() == Qt::Unchecked) {
        state = false; //set state to false
    } else {
        state = true; //set state to true
    }

    ui->ViewWidget->ChangeView(name, state);
}

//Open Cluster Options dialog -> When triggered
void MainWindow::on_actionCluster_Options_triggered()
{
    ui->ViewWidget->ChangeClusterFromDialog(ui->ViewWidget, dialog); //Connect dialog with ViewWidget
    dialog->show(); //Show Cluster Options
}

//Open CrabsEditor -> When triggered
void MainWindow::on_actionCrabsEditor_triggered()
{
    CrabsEditor* editor = new CrabsEditor(this); //Create editor

    editor->show(); //Show editor
}

//Open Cluster Options dialog -> When itemPressed
void MainWindow::on_ClusterList_itemPressed(QListWidgetItem *item)
{
    std::string name = item->text().toUtf8().constData();

    ui->ViewWidget->ChangeClusterFromDialog(ui->ViewWidget, dialog, name); //Connect dialog with ViewWidget
    dialog->show(); //Show Cluster Options
}

//Open Model Create Dialog -> When triggered
void MainWindow::on_actionModelCreate_triggered()
{
    ui->ViewWidget->CreateModel(ui->ViewWidget, modelCreateDialog, &modelExists);
    modelCreateDialog->show(); //Show Model Create Dialog
}

//Move UP -> When triggered
void MainWindow::on_actionUp_triggered()
{
    ui->ViewWidget->SetEyeY(ui->ViewWidget, 0.5); //Move up
    ui->ViewWidget->SetCenterY(ui->ViewWidget, 0.5); //Move up

    ui->AxisWidget->SetEyeY(ui->AxisWidget, 0.5); //Move up
    ui->AxisWidget->SetCenterY(ui->AxisWidget, 0.5); //Move up
    ui->AxisWidget->SetAxisY(ui->AxisWidget, 0.5);
}

//Move DOWN -> When triggered
void MainWindow::on_actionDown_triggered()
{
    ui->ViewWidget->SetEyeY(ui->ViewWidget, -0.5); //Move down
    ui->ViewWidget->SetCenterY(ui->ViewWidget, -0.5); //Move down

    ui->AxisWidget->SetEyeY(ui->AxisWidget, -0.5); //Move up
    ui->AxisWidget->SetCenterY(ui->AxisWidget, -0.5); //Move up
    ui->AxisWidget->SetAxisY(ui->AxisWidget, -0.5);
}

//Move LEFT -> When triggered
void MainWindow::on_actionLeft_triggered()
{
    ui->ViewWidget->SetEyeX(ui->ViewWidget, -0.5); //Move left
    ui->ViewWidget->SetCenterX(ui->ViewWidget, -0.5); //Move left

    ui->AxisWidget->SetEyeX(ui->AxisWidget, -0.5); //Move up
    ui->AxisWidget->SetCenterX(ui->AxisWidget, -0.5); //Move up
    ui->AxisWidget->SetAxisX(ui->AxisWidget, -0.5);
}

//Move RIGHT -> When clicked
void MainWindow::on_actionRight_triggered()
{
    ui->ViewWidget->SetEyeX(ui->ViewWidget, 0.5); //Move left
    ui->ViewWidget->SetCenterX(ui->ViewWidget, 0.5); //Move right

    ui->AxisWidget->SetEyeX(ui->AxisWidget, 0.5); //Move up
    ui->AxisWidget->SetCenterX(ui->AxisWidget, 0.5); //Move up
    ui->AxisWidget->SetAxisX(ui->AxisWidget, 0.5);
}

//X Rotation + -> When triggered
void MainWindow::on_actionX_Rotation_triggered()
{
    ui->ViewWidget->RotateX(10);
    ui->AxisWidget->RotateX(10);
}

//X Rotation - -> When triggered
void MainWindow::on_actionX_Rotation_2_triggered()
{
    ui->ViewWidget->RotateX(-10);
    ui->AxisWidget->RotateX(-10);
}

//Y Rotation + -> When triggered
void MainWindow::on_actionY_Rotation_triggered()
{
    ui->ViewWidget->RotateY(10);
    ui->AxisWidget->RotateY(10);
}

//Y Rotation - -> When triggered
void MainWindow::on_actionY_Rotation_2_triggered()
{
    ui->ViewWidget->RotateY(-10);
    ui->AxisWidget->RotateY(-10);
}

//Z Rotation + -> When triggered
void MainWindow::on_actionZ_Rotation_triggered()
{
    ui->ViewWidget->RotateZ(-10);
    ui->AxisWidget->RotateZ(-10);
}

//Z Rotation - -> When triggered
void MainWindow::on_actionZ_Rotation_2_triggered()
{
    ui->ViewWidget->RotateZ(10);
    ui->AxisWidget->RotateZ(10);
}

//Play or Pause Motion -> When triggered
void MainWindow::on_actionPlay_Pause_triggered()
{
    ui->ViewWidget->SetPlay(ui->ViewWidget); //Play or Pause the motion
}

//Zoom In -> When triggered
void MainWindow::on_actionZoom_In_triggered()
{
    ui->ViewWidget->SetZoom(ui->ViewWidget, -2.5);
    ui->AxisWidget->SetZoom(ui->AxisWidget, -2.5);
}

//Zoom Out -> When triggered
void MainWindow::on_actionZoom_Out_triggered()
{
    ui->ViewWidget->SetZoom(ui->ViewWidget, 2.5);
    ui->AxisWidget->SetZoom(ui->AxisWidget, 2.5);
}

//Move Front -> When triggered
void MainWindow::on_actionFront_triggered()
{
    ui->ViewWidget->SetEyeZ(ui->ViewWidget, -0.5);
    ui->ViewWidget->SetCenterZ(ui->ViewWidget, -0.5);

    ui->AxisWidget->SetEyeZ(ui->AxisWidget, -0.5); //Move up
    ui->AxisWidget->SetCenterZ(ui->AxisWidget, -0.5); //Move up
    ui->AxisWidget->SetAxisZ(ui->AxisWidget, -0.5);
}

//Move Back -> When triggered
void MainWindow::on_actionBack_triggered()
{
    ui->ViewWidget->SetEyeZ(ui->ViewWidget, 0.5);
    ui->ViewWidget->SetCenterZ(ui->ViewWidget, 0.5);

    ui->AxisWidget->SetEyeZ(ui->AxisWidget, 0.5); //Move up
    ui->AxisWidget->SetCenterZ(ui->AxisWidget, 0.5); //Move up
    ui->AxisWidget->SetAxisZ(ui->AxisWidget, 0.5);
}

//Show XY grid
void MainWindow::on_actionXY_grid_triggered()
{
    if(ui->actionXY_grid->isChecked()) {
        ui->ViewWidget->SetGridXYView(ui->ViewWidget, true);
    } else {
        ui->ViewWidget->SetGridXYView(ui->ViewWidget, false);
    }
}

//Show XZ grid
void MainWindow::on_actionXZ_grid_triggered()
{
    if(ui->actionXZ_grid->isChecked()) {
        ui->ViewWidget->SetGridXZView(ui->ViewWidget, true);
    } else {
        ui->ViewWidget->SetGridXZView(ui->ViewWidget, false);
    }
}

//Show YZ grid
void MainWindow::on_actionYZ_grid_triggered()
{
    if(ui->actionYZ_grid->isChecked()) {
        ui->ViewWidget->SetGridYZView(ui->ViewWidget, true);
    } else {
        ui->ViewWidget->SetGridYZView(ui->ViewWidget, false);
    }
}

//Show XY grid
void MainWindow::on_XY_CheckBox_stateChanged(int arg1)
{
    if(arg1 != ui->XY_CheckBox->isChecked()) {
        ui->AxisWidget->SetGridXYView(ui->AxisWidget, true);
    } else {
        ui->AxisWidget->SetGridXYView(ui->AxisWidget, false);
    }
}

//Show XZ grid
void MainWindow::on_XZ_CheckBox_stateChanged(int arg1)
{
    if(arg1 != ui->XZ_CheckBox->isChecked()) {
        ui->AxisWidget->SetGridXZView(ui->AxisWidget, true);
    } else {
        ui->AxisWidget->SetGridXZView(ui->AxisWidget, false);
    }
}

//Show YZ grid
void MainWindow::on_YZ_CheckBox_stateChanged(int arg1)
{
    if(arg1 != ui->YZ_CheckBox->isChecked()) {
        ui->AxisWidget->SetGridYZView(ui->AxisWidget, true);
    } else {
        ui->AxisWidget->SetGridYZView(ui->AxisWidget, false);
    }
}

//Set Unit -> When triggered
void MainWindow::on_actionUnits_triggered()
{
    ui->ViewWidget->SetUnit(unitDialog);
    unitDialog->show();
}

//Set Bones -> When triggered
void MainWindow::on_actionSetBones_triggered()
{
    ui->ViewWidget->setBones(ui->ViewWidget, setBones, &boneExists);
    setBones->show();
}

//Create Bones -> When triggered
void MainWindow::on_actionCreate_Bones_triggered()
{
    ui->ViewWidget->CreateBones(ui->ViewWidget);
}

void MainWindow::on_ModelList_itemChanged(QListWidgetItem *item)
{
    bool state;

    //Chech if item->checkState is Unchecked or Not
    if(item->checkState() == Qt::Unchecked) {
        state = false; //set state to false
    } else {
        state = true; //set state to true
    }

    ui->ViewWidget->SetBoneViewState(ui->ViewWidget, state);
}
