#include "crabs_editor.h"
#include "ui_crabs_editor.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#define ALL_FORMAT "All Files (*.*)"
#define TXT_FORMAT "Text File (*.txt)"

CrabsEditor::CrabsEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CrabsEditor)
{
    ui->setupUi(this);
     this->setCentralWidget(ui->Editor);
}

CrabsEditor::~CrabsEditor()
{
    delete ui;
}

//New
void CrabsEditor::on_actionNew_triggered()
{
    file_name_ = ALL_FORMAT ";;" TXT_FORMAT;
    file_name_ = "";
    ui->Editor->setText("");
}

//Open
void CrabsEditor::on_actionOpen_triggered()
{
    QString filter = ALL_FORMAT ";;" TXT_FORMAT;

    QString openFilePath = QFileDialog::getOpenFileName(this, "Open", QDir::homePath(), filter, 0, QFileDialog::DontUseNativeDialog);
    if(openFilePath == "") {
        return ;
    }
    file_name_ = openFilePath;

    QFile file(openFilePath);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "...", "File not open...");
        return ;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->Editor->setText(text);
    file.close();
}

//Save
void CrabsEditor::on_actionSave_triggered()
{
    QString filter = ALL_FORMAT ";;" TXT_FORMAT;

    if(file_name_ == "") {
        QString openFilePath = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), filter, 0, QFileDialog::DontUseNativeDialog);

        if(openFilePath == "") {
            return ;
        }
        file_name_ = openFilePath;
    }

    QFile file(file_name_);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "...", "File not open...");
        return ;
    }
    QTextStream out(&file);
    QString text = ui->Editor->toPlainText();
    out << text;
    file.flush();
    file.close();
}

//Save as
void CrabsEditor::on_actionSave_as_triggered()
{
    QString filter = ALL_FORMAT ";;" TXT_FORMAT;

    QString openFilePath = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), filter, 0, QFileDialog::DontUseNativeDialog);

    if(openFilePath == "") {
        return ;
    }

    QFile file(openFilePath);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "...", "File not open...");
        return ;
    }
    QTextStream out(&file);
    QString text = ui->Editor->toPlainText();
    out << text;
    file.flush();
    file.close();
}

//Exit
void CrabsEditor::on_actionExit_triggered()
{
    QMainWindow::close();
}

//Cut
void CrabsEditor::on_actionCut_triggered()
{
    ui->Editor->cut();
}

//Copy
void CrabsEditor::on_actionCopy_triggered()
{
    ui->Editor->copy();
}

//Paste
void CrabsEditor::on_actionPaste_triggered()
{
    ui->Editor->paste();
}

//Undo
void CrabsEditor::on_actionUndo_triggered()
{
    ui->Editor->undo();
}

//Redo
void CrabsEditor::on_actionRedo_triggered()
{
    ui->Editor->redo();
}

//About
void CrabsEditor::on_actionAbout_CrabsEditor_triggered()
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

    QMessageBox::about(this, "About CrabsEditor", text);
}
