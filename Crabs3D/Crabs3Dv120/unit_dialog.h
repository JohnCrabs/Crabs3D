#ifndef UNIT_DIALOG_H
#define UNIT_DIALOG_H

#include <QDialog>

namespace Ui {
class UnitDialog;
}

class UnitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UnitDialog(QWidget *parent = 0);
    ~UnitDialog();

    void SetUnit(float* unit);

private slots:
    void on_Ok_clicked();

    void on_Cancel_clicked();

private:
    Ui::UnitDialog *ui;
    float bufUnit;
    float* unitToSet;
};

#endif // UNIT_DIALOG_H
