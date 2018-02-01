#ifndef CRABS_EDITOR_H
#define CRABS_EDITOR_H

#include <QMainWindow>

namespace Ui {
class CrabsEditor;
}

class CrabsEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit CrabsEditor(QWidget *parent = 0);
    ~CrabsEditor();

private slots:
    //New
    void on_actionNew_triggered();

    //Open
    void on_actionOpen_triggered();

    //Save
    void on_actionSave_triggered();

    //Save as
    void on_actionSave_as_triggered();

    //Exit
    void on_actionExit_triggered();

    //Cut
    void on_actionCut_triggered();

    //Copy
    void on_actionCopy_triggered();

    //Paste
    void on_actionPaste_triggered();

    //Undo
    void on_actionUndo_triggered();

    //Redo
    void on_actionRedo_triggered();

    //About
    void on_actionAbout_CrabsEditor_triggered();

private:
    Ui::CrabsEditor *ui;
    QString file_name_;
};

#endif // CRABS_EDITOR_H
