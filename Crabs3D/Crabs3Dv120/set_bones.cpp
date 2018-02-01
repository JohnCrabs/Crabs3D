#include "set_bones.h"
#include "ui_set_bones.h"

SetBones::SetBones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetBones)
{
    ui->setupUi(this);
    dialogState = false;
}

SetBones::~SetBones()
{
    delete ui;
}

void SetBones::Bones(std::string* clusterName, bool* boneState, KMeans clusters, const int clusterSize, bool* state) {

    dialogState = true;
    clusName = clusterName;
    okState = boneState;
    okEnableState = state;

    ui->ClusterBox->clear();
    for(int i = 0; i < clusterSize; i++) {
        std::string name = clusters.GetCluster(i).name[0];
        ui->ClusterBox->addItem(QString::fromUtf8(name.c_str()));
    }

    *clusName = ui->ClusterBox->currentText().toUtf8().constData();
}

void SetBones::on_ClusterBox_currentTextChanged(const QString &arg1)
{
    *clusName = arg1.toUtf8().constData();
}

void SetBones::on_pushButton_2_clicked()
{
    QDialog::close();
}

void SetBones::on_pushButton_clicked()
{
    if(dialogState) {
        *okState = true;
        *okEnableState = true;
    }
    QDialog::close();
}
