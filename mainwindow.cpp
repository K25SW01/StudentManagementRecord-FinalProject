#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

/*
 * ===========================================
 *  STUDENT MANAGEMENT RECORD SYSTEM (CSV)
 * ===========================================
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButtonAdd->setEnabled(true);
    ui->pushButtonUpdate->setEnabled(false);
    ui->pushButtonDelete->setEnabled(false);

    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->pushButtonNext, &QPushButton::clicked, this, &MainWindow::onNextClicked);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &MainWindow::onUpdateClicked);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);

    currentIndex = -1;

    // ---------- Load existing CSV file ----------
    QFile file("students.csv");
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString header = in.readLine(); // Skip header line

        while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if(fields.size() == 6) {
                Student s = {
                    fields[0].trimmed(),
                    fields[1].trimmed(),
                    fields[2].trimmed(),
                    fields[3].trimmed(),
                    fields[4].trimmed(),
                    fields[5].trimmed()
                };
                students.append(s);
            }
        }
        file.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ------------------------------
// Helper Function: Clear fields
// ------------------------------
void MainWindow::clearFields() {
    ui->lineEditStudentName->clear();
    ui->lineEditRollNumber->clear();
    ui->lineEditFatherName->clear();
    ui->lineEditCaste->clear();
    ui->lineEditDistrict->clear();
    ui->lineEditGPA->clear();
}

// ------------------------------
// Helper Function: Validate inputs
// ------------------------------
bool MainWindow::validateFields() {
    if (ui->lineEditStudentName->text().isEmpty()) {
        QMessageBox::warning(this, "Missing Field", "Name field is required!");
        return false;
    }
    if (ui->lineEditRollNumber->text().isEmpty()) {
        QMessageBox::warning(this, "Missing Field", "Roll Number is required!");
        return false;
    }
    if (ui->lineEditFatherName->text().isEmpty()) {
        QMessageBox::warning(this, "Missing Field", "Father's Name is required!");
        return false;
    }
    if (ui->lineEditCaste->text().isEmpty()) {
        QMessageBox::warning(this, "Missing Field", "Caste field is required!");
        return false;
    }
    if (ui->lineEditDistrict->text().isEmpty()) {
        QMessageBox::warning(this, "Missing Field", "District field is required!");
        return false;
    }
    if (ui->lineEditGPA->text().isEmpty()) {
        QMessageBox::warning(this, "Missing Field", "GPA field is required!");
        return false;
    }
    return true;
}

// ------------------------------
// SAVE ALL STUDENTS TO CSV FILE
// ------------------------------
void saveAllToCSV(const QVector<Student>& students) {
    QFile file("students.csv");
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&file);

        out << "StudentName,RollNumber,FatherName,Caste,District,GPA\n";

        for (const Student &s : students) {
            out << s.name << "," << s.roll << "," << s.father << ","
                << s.caste << "," << s.district << "," << s.gpa << "\n";
        }
        file.close();
    }
}

// ------------------------------
// Helper: Check duplicate roll number
// ------------------------------
bool MainWindow::rollExists(const QString &roll, int ignoreIndex) {
    QString r = roll.trimmed();

    for (int i = 0; i < students.size(); ++i) {
        if (i == ignoreIndex) continue;
        if (students[i].roll.trimmed() == r)
            return true;
    }
    return false;
}

// ------------------------------
// ADD BUTTON FUNCTION
// ------------------------------
void MainWindow::onAddClicked() {
    if (!validateFields()) return;

    Student s;
    s.name = ui->lineEditStudentName->text();
    s.roll = ui->lineEditRollNumber->text().trimmed();
    s.father = ui->lineEditFatherName->text();
    s.caste = ui->lineEditCaste->text();
    s.district = ui->lineEditDistrict->text();
    s.gpa = ui->lineEditGPA->text();

    // Duplicate Roll Check
    if (rollExists(s.roll)) {
        QMessageBox::warning(this, "Duplicate Roll", "A student with this Roll Number already exists!");
        return;
    }

    students.append(s);

    saveAllToCSV(students);

    QMessageBox::information(this, "Success", "Student added successfully!");

    clearFields();
    ui->pushButtonAdd->setEnabled(true);
    ui->pushButtonUpdate->setEnabled(false);
    ui->pushButtonDelete->setEnabled(false);
}

// ------------------------------
// NEXT BUTTON FUNCTION
// ------------------------------
void MainWindow::onNextClicked() {
    if (students.isEmpty()) {
        QMessageBox::information(this, "Info", "No student records available!");
        return;
    }

    currentIndex = (currentIndex + 1) % students.size();

    Student s = students[currentIndex];
    ui->lineEditStudentName->setText(s.name);
    ui->lineEditRollNumber->setText(s.roll);
    ui->lineEditFatherName->setText(s.father);
    ui->lineEditCaste->setText(s.caste);
    ui->lineEditDistrict->setText(s.district);
    ui->lineEditGPA->setText(s.gpa);

    ui->pushButtonAdd->setEnabled(false);
    ui->pushButtonUpdate->setEnabled(true);
    ui->pushButtonDelete->setEnabled(true);
}

// ------------------------------
// UPDATE BUTTON FUNCTION
// ------------------------------
void MainWindow::onUpdateClicked() {
    if (currentIndex < 0 || currentIndex >= students.size()) return;
    if (!validateFields()) return;

    QString newRoll = ui->lineEditRollNumber->text().trimmed();

    // Prevent updating to an existing roll number
    if (rollExists(newRoll, currentIndex)) {
        QMessageBox::warning(this, "Duplicate Roll", "Another student already has this Roll Number!");
        return;
    }

    Student &s = students[currentIndex];
    s.name = ui->lineEditStudentName->text();
    s.roll = newRoll;
    s.father = ui->lineEditFatherName->text();
    s.caste = ui->lineEditCaste->text();
    s.district = ui->lineEditDistrict->text();
    s.gpa = ui->lineEditGPA->text();

    saveAllToCSV(students);

    QMessageBox::information(this, "Updated", "Student record updated successfully!");

    clearFields();
    ui->pushButtonAdd->setEnabled(true);
    ui->pushButtonUpdate->setEnabled(false);
    ui->pushButtonDelete->setEnabled(false);
}

// ------------------------------
// DELETE BUTTON FUNCTION
// ------------------------------
void MainWindow::onDeleteClicked() {
    if (currentIndex < 0 || currentIndex >= students.size()) return;

    students.removeAt(currentIndex);

    if (currentIndex >= students.size())
        currentIndex = students.size() - 1;

    saveAllToCSV(students);

    if (students.isEmpty()) {
        clearFields();
        ui->pushButtonAdd->setEnabled(true);
        ui->pushButtonUpdate->setEnabled(false);
        ui->pushButtonDelete->setEnabled(false);
    } else {
        onNextClicked();
    }

    QMessageBox::information(this, "Deleted", "Student record deleted successfully!");
}
