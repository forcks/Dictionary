#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QDebug>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void EditColorApplication(QString first_color,QString second_color,QString third_color,QString fourth_color);

    void createWindowChooseBackground();

    void PutBackgroundOnMainWindow();

public slots:
    void chooseBackground();
private slots:

    void setValueInWord_textEdit(QString little_letter,QString big_letter);

    void on_words_a_clicked();

    void on_words_b_clicked();

    void on_words_c_clicked();

    void on_words_d_clicked();

    void on_words_e_clicked();

    void on_words_f_clicked();

    void on_words_g_clicked();

    void on_words_h_clicked();

    void on_words_i_clicked();

    void on_words_j_clicked();

    void on_words_k_clicked();

    void on_words_l_clicked();

    void on_words_m_clicked();

    void on_words_n_clicked();

    void on_words_o_clicked();

    void on_words_p_clicked();

    void on_words_q_clicked();

    void on_words_r_clicked();

    void on_words_s_clicked();

    void on_words_t_clicked();

    void on_words_u_clicked();

    void on_words_v_clicked();

    void on_words_w_clicked();

    void on_words_x_clicked();

    void on_words_y_clicked();

    void on_words_z_clicked();




    void on_addNewWord_clicked();


    void on_delete_word_clicked();

    void on_search_textChanged();

    void on_english_edit_word_textChanged();

    void on_save_edited_clicked();

    void on_horizontalSlider_actionTriggered(int action);

    void on_Slider_text_size_actionTriggered(int action);

    void SetBackground(QString _nameBackground);

    void SetNormalBackground();

    void on_chooseBackground_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
