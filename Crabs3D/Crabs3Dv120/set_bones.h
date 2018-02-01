#ifndef SET_BONES_H
#define SET_BONES_H

#include <QDialog>
#include <kmeans.h>
#include <model.h>

namespace Ui {
class SetBones;
}

class SetBones : public QDialog
{
    Q_OBJECT

public:
    explicit SetBones(QWidget *parent = 0);
    ~SetBones();

    void Bones(std::string *clusterName, bool *boneState, KMeans clusters, const int clusterSize, bool *state);

private slots:
    void on_ClusterBox_currentTextChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::SetBones *ui;

    std::string* clusName;
    bool dialogState;
    bool* okState;
    bool* okEnableState;

};

#endif // SET_BONES_H
