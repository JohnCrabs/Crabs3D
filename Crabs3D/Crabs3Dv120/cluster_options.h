#ifndef CLUSTER_OPTIONS_H
#define CLUSTER_OPTIONS_H

#include <QDialog>
#include <QTimer>
#include "kmeans.h"

namespace Ui {
class ClusterOptions;
}

class ClusterOptions : public QDialog
{
    Q_OBJECT

public:
    explicit ClusterOptions(QWidget *parent = 0);
    ~ClusterOptions();

    void ClusterTypeOptions(KMeans *cluster, const int clusterSize, bool state, std::string ItemName);
    void CleanUpCluster();
    void ChangeCluster(KMeans *cluster);

public slots:

private:
    Ui::ClusterOptions *ui;

    //Variables

    //Cluster
    bool clusterExists;
    bool saveState;
    int clusterDialogSize;
    int clusterIndex;
    KMeans *clusterDialog;

    //ClusterBox Timer
    QTimer clusterBoxTimer;

    //Clean Cluster Box
    void CleanBox();

private slots:
    //Check if cluster Box changed value
    void on_clusterBox_currentTextChanged(const QString &arg1);

    //Change red color value of a cluster
    void on_redSpin_valueChanged(int arg1);

    //Change green color value of a cluster
    void on_greenSpin_valueChanged(int arg1);

    //Change blue color value of a cluster
    void on_blueSpin_valueChanged(int arg1);

    //Close dialog
    void on_Ok_clicked();

    //Change the point size of a cluster
    void on_pointSizeSpinBox_valueChanged(double arg1);

    //Change the name of a cluster
    void on_NewNameEdit_textEdited(const QString &arg1);

};

#endif // CLUSTER_OPTIONS_H
