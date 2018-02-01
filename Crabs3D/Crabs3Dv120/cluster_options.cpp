#include "cluster_options.h"
#include "ui_cluster_options.h"
#include <QMessageBox>

/**********/
/* Public */
/**********/

ClusterOptions::ClusterOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClusterOptions)
{
    ui->setupUi(this);
    saveState = false;
}

ClusterOptions::~ClusterOptions()
{
    CleanUpCluster();
    delete ui;
}

void ClusterOptions::ClusterTypeOptions(KMeans *cluster, const int clusterSize, bool state, std::string ItemName) {

    clusterExists = state;
    if(clusterExists) {
        CleanBox();
        clusterDialog = cluster;

        clusterDialogSize = clusterSize;
        for(int i = 0; i < clusterDialogSize; i++) {
            QString name = QString::fromUtf8(clusterDialog[0].GetCluster(i).name[0].c_str());
            ui->clusterBox->addItem(name);
        }

        if(ItemName != "") {
            QString name = QString::fromUtf8(ItemName.c_str());
            ui->clusterBox->setCurrentText(name);
        }
    }

}

void ClusterOptions::CleanUpCluster() {
    if(clusterExists) {
        clusterDialog->CleanUp();
        delete[] clusterDialog;
    }
}

/****************/
/* Public Slots */
/****************/


/***********/
/* Private */
/***********/

void ClusterOptions::CleanBox() {
    ui->clusterBox->clear();
}

/*****************/
/* Private Slots */
/*****************/

void ClusterOptions::on_clusterBox_currentTextChanged(const QString &arg1)
{
    if(clusterExists) {
        std::string name = arg1.toUtf8().constData();
        //QMessageBox::information(this, "Debug", name);

        int index;
        for(index = 0; index < clusterDialogSize; index++) {
            if(name == clusterDialog[0].GetCluster(index).name[0]) {
                break;
            }
        }

        clusterIndex = index;

        ui->NewNameEdit->setText(arg1);

        int red = clusterDialog[0].GetCluster(index).color.red * 255;
        int green = clusterDialog[0].GetCluster(index).color.green * 255;
        int blue = clusterDialog[0].GetCluster(index).color.blue * 255;
        clusterDialog[0].SetColorState(index, true);

        ui->redSpin->setValue(red);
        ui->greenSpin->setValue(green);
        ui->blueSpin->setValue(blue);

        float pointSize = clusterDialog[0].GetCluster(index).pointSize;
        ui->pointSizeSpinBox->setValue(pointSize);

    }
}

void ClusterOptions::on_redSpin_valueChanged(int arg1)
{
    if(clusterExists)
        clusterDialog->SetColorValueRed(clusterIndex, (float)arg1/255);
}

void ClusterOptions::on_greenSpin_valueChanged(int arg1)
{
    if(clusterExists)
        clusterDialog->SetColorValueGreen(clusterIndex, (float)arg1/255);
}

void ClusterOptions::on_blueSpin_valueChanged(int arg1)
{
    if(clusterExists)
        clusterDialog->SetColorValueBlue(clusterIndex, (float)arg1/255);
}

void ClusterOptions::on_Ok_clicked()
{
    QDialog::close();
}

void ClusterOptions::on_pointSizeSpinBox_valueChanged(double arg1)
{
    if(clusterExists)
        clusterDialog->SetPointSize(clusterIndex, arg1);

}

void ClusterOptions::on_NewNameEdit_textEdited(const QString &arg1)
{
    if(clusterExists) {
        int cursorPosition =  ui->NewNameEdit->cursorPosition();

        clusterDialog->SetName(clusterIndex, arg1.toUtf8().constData());

        ui->clusterBox->setItemText(clusterIndex, arg1);
        ui->NewNameEdit->setCursorPosition(cursorPosition);
    }
}
