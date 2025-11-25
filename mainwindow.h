#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>

/*
 * ===============================
 *  STUDENT MANAGEMENT SYSTEM
 *  Developed by: Muhammad Farhan
 *  Roll No: (K25SW01)
 * ===============================
 */

// ------------------------------
// Structure to store one student
// ------------------------------
struct Student {
    QString name;
    QString roll;
    QString father;
    QString caste;
    QString district;
    QString gpa;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddClicked();     // Add new record
    void onNextClicked();    // Show next record
    void onUpdateClicked();  // Update existing record
    void onDeleteClicked();  // Delete record

private:
    Ui::MainWindow *ui;

    QVector<Student> students; // All student records
    int currentIndex;          // Current record index

    // Helper functions
    void clearFields();
    bool validateFields();
    bool rollExists(const QString &roll, int ignoreIndex = -1);  // NEW FUNCTION
};

#endif // MAINWINDOW_H
