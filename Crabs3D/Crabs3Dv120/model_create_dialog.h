#ifndef MODEL_CREATE_DIALOG_H
#define MODEL_CREATE_DIALOG_H

#include <QDialog>
#include "model.h"

namespace Ui {
class ModelCreateDialog;
}

class ModelCreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModelCreateDialog(QWidget *parent = 0);
    ~ModelCreateDialog();

    void ModelCreate(Model* model, bool* state);

private slots:
    void on_Name_Edit_textEdited(const QString &arg1);

    void on_RedSpin_valueChanged(int arg1);

    void on_GreenSpin_valueChanged(int arg1);

    void on_BlueSpin_valueChanged(int arg1);

    void on_Create_Button_clicked();

private:
    Ui::ModelCreateDialog *ui;
    Model *modelDialog;
    bool *okState;
};

#endif // MODEL_CREATE_DIALOG_H
