#include "model_create_dialog.h"
#include "ui_model_create_dialog.h"

#include <QDebug>

ModelCreateDialog::ModelCreateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModelCreateDialog)
{
    ui->setupUi(this);

}

ModelCreateDialog::~ModelCreateDialog()
{
    delete ui;
}

void ModelCreateDialog::ModelCreate(Model* model, bool *state)
{
    modelDialog = model;
    okState = state;
    *okState = false;
}

void ModelCreateDialog::on_Name_Edit_textEdited(const QString &arg1)
{
    modelDialog->SetModelName(arg1.toUtf8().constData());
}

void ModelCreateDialog::on_RedSpin_valueChanged(int arg1)
{
    modelDialog->SetColorRed(arg1);
}

void ModelCreateDialog::on_GreenSpin_valueChanged(int arg1)
{
    modelDialog->SetColorGreen(arg1);
}

void ModelCreateDialog::on_BlueSpin_valueChanged(int arg1)
{
    modelDialog->SetColorBlue(arg1);
}

void ModelCreateDialog::on_Create_Button_clicked()
{
    modelDialog->SetState(true);
    *okState = true;
    QDialog::close();
}

