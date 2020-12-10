#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <config.h>

#include <QDir>
#include <QFile>
#include <QDebug>

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"



QSqlDatabase db;
QString activeTab[2];
QPalette normalPaleteApplication;
QString Background;
/*
 * connection with SQlite
 */
void connectDB(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QApplication::applicationDirPath()+"/"+"EnglishWords.db3");
    qDebug()<<QApplication::applicationDirPath()+"/"+"EnglishWords.db3";
}

/*
 * shows words in accordance with the supplied letter
*/
void MainWindow::setValueInWord_textEdit(QString little_letter,QString big_letter){

    ui->status->setText(big_letter);
    activeTab[0] = little_letter;
    activeTab[1] = big_letter;

    ui->words_textEdit->clear();
    db.open();
    QSqlQuery query;
    query.exec("SELECT id, English_word, Russian_word, example, second_form_verb, third_form_verb FROM words");
    QString words;
    while (query.next()) {
        QString englishWord = query.value(1).toString();
        if(englishWord[0] == little_letter || englishWord[0] == big_letter){
            //QString id = query.value(0).toString();
            QString russianWord = query.value(2).toString();
            QString example =   query.value(3).toString() != nullptr ? "( "+ query.value(3).toString()+" )" : "";
            QString secondFormVerb = query.value(4).toString() != nullptr ? " | "+query.value(4).toString() : "";
            QString thirdFormVerb =query.value(5).toString() != nullptr ? " | "+query.value(5).toString() : "";
            words = englishWord+" "+secondFormVerb+" "+thirdFormVerb+"       "+russianWord+" "+example+ "\n";
            ui->words_textEdit->insertPlainText(words);


        }
    }
    db.close();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectDB();

    /*work with config*/
    config conf;
    conf.createConfig();
    //select size font
    int fontSize = conf.readInConfig("sizeFont","20","sizeFont").toInt();
    ui->Slider_text_size->setValue(fontSize);

    QFont font = ui->words_textEdit->font();
    font.setPixelSize(ui->Slider_text_size->value());
    ui->words_textEdit->setFont(font);

    //remember the default palette
    normalPaleteApplication = this->palette();

    QFile configBaground(QApplication::applicationDirPath()+"/BackgroundMainWindow.json");
    if(!configBaground.exists()){
        config *conf = new config;
        conf->writeInConfig("Background","normal_background","BackgroundMainWindow");
    }
    PutBackgroundOnMainWindow();

    /*end work with config*/

    //set the defalt letter
    setValueInWord_textEdit("a","A");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * add new word in database
 */
void MainWindow::on_addNewWord_clicked()
{
    /*
     * get data from input fields
     */
    QString englishWord = ui->english_new_word->text();
    QString russianWord = ui->russian_new_word->text();
    QString example = ui->example->text();
    QString secondFormVerb =  ui->second_form_verb_textedit->text();
    QString thirdFormVerb =  ui->third_form_verb_textedit->text();

    /*
     * work with database
     */
    db.open();
    QString newWord;

    if (secondFormVerb == nullptr && thirdFormVerb == nullptr)
        newWord = "INSERT INTO words (English_word, Russian_word, example) VALUES ( \'" +englishWord+"\' , \'"+russianWord+"\' , \'"+example+ "\' )";
    else
        newWord = "INSERT INTO words (English_word, Russian_word, example, second_form_verb, third_form_verb) "
                  "VALUES ( \'" +englishWord+"\' , \'"+russianWord+"\' , \'"+example+ "\', \'"+secondFormVerb+"\', \'"+thirdFormVerb+"\')";
    QSqlQuery query;
    query.exec(newWord);
    db.close();
    //end work wirh database

    //clear input fields
    ui->english_new_word->clear();
    ui->russian_new_word->clear();
    ui->second_form_verb_textedit->clear();
    ui->third_form_verb_textedit->clear();
    ui->example->clear();
    //

    /*
     * update data
     */
    setValueInWord_textEdit(activeTab[0],activeTab[1]);
}

/*
    * delete don't need word
    */
void MainWindow::on_delete_word_clicked()
{
    /*
    * get the delete word
    */
    QString deleteWord = ui->delete_word_textedit->text();

    db.open();
    QString commandDeletedWord = "DELETE FROM words WHERE English_word = \'"+deleteWord+"\'";
    QSqlQuery query;
    query.exec(commandDeletedWord);
    db.close();

    ui->delete_word_textedit->clear();

    /*
     * update data
     */
    setValueInWord_textEdit(activeTab[0],activeTab[1]);
}


void MainWindow::on_search_textChanged()
{
    /*
     * word to find
     */
    QString word = ui->search->text();
    db.open();
    QSqlQuery query;
    query.exec("SELECT id, English_word, Russian_word, example, second_form_verb, third_form_verb FROM words");
    while (query.next()) {
        QString englishWord = query.value(1).toString();
        if(word == englishWord){
            QString englishWord = query.value(1).toString();
            QString russianWord = query.value(2).toString();
            QString example =   query.value(3).toString() != nullptr ? "( "+ query.value(3).toString()+" )" : "";
            QString secondFormVerb = query.value(4).toString() != nullptr ? " | "+query.value(4).toString() : "";
            QString thirdFormVerb =query.value(5).toString() != nullptr ? " | "+query.value(5).toString() : "";
            ui->words_textEdit->setText(englishWord+" "+secondFormVerb+" "+thirdFormVerb+"       "+russianWord+" "+example+ "\n");
            /*
             * if word is find than search stop
             */
            break;
        }else if(word == " " || word == "" || word == nullptr){
            setValueInWord_textEdit(activeTab[0],activeTab[1]);
        }else{
            ui->words_textEdit->setText("");
        }
    }
    db.close();
}



void MainWindow::on_english_edit_word_textChanged()
{
    QString English_Word = ui->english_edit_word->text();
    db.open();
    QSqlQuery query;
    query.exec("SELECT English_word, Russian_word, example, second_form_verb, third_form_verb FROM words");
    while (query.next()) {
        if(query.value(0).toString() == English_Word){
            ui->russian_edit_word->setText(query.value(1).toString());
            ui->example_edit->setText(query.value(2).toString());
            ui->second_form_verb_edit_textedit->setText(query.value(3).toString());
            ui->third_form_verb_edit_textedit->setText(query.value(4).toString());
            break;
        }else{
            ui->russian_edit_word->clear();
            ui->example_edit->clear();
            ui->second_form_verb_edit_textedit->clear();
            ui->third_form_verb_edit_textedit->clear();
        }
    }
    db.close();
}


void MainWindow::on_save_edited_clicked()
{
    QString English_Word = ui->english_edit_word->text();
    if(English_Word != nullptr && English_Word != "" && English_Word != " "){
        db.open();

        QString editedRussian_word = ui->russian_edit_word->text();
        QString editedExample = ui->example_edit->text();
        QString second_form = ui->second_form_verb_edit_textedit->text();
        QString third_form = ui->third_form_verb_edit_textedit->text();

        QString command = "UPDATE words SET Russian_word = \'"+editedRussian_word+"\', example = \'"+editedExample+
                "\', second_form_verb =  \'"+second_form+"\', third_form_verb =  \'"+third_form+"\' "
                                                                                                "WHERE English_word = \'"+English_Word+"\'";
        QSqlQuery query;
        query.exec(command);
        db.close();

        ui->english_edit_word->clear();
        ui->russian_edit_word->clear();
        ui->example_edit->clear();
        ui->second_form_verb_edit_textedit->clear();
        ui->third_form_verb_edit_textedit->clear();

        setValueInWord_textEdit(activeTab[0],activeTab[1]);
    }
}


void MainWindow::on_words_a_clicked()
{

    setValueInWord_textEdit("a","A");
}

void MainWindow::on_words_b_clicked()
{

    setValueInWord_textEdit("b","B");
}

void MainWindow::on_words_c_clicked()
{

    setValueInWord_textEdit("c","C");
}

void MainWindow::on_words_d_clicked()
{
    setValueInWord_textEdit("d","D");
}

void MainWindow::on_words_e_clicked()
{
    setValueInWord_textEdit("e","E");
}

void MainWindow::on_words_f_clicked()
{
    setValueInWord_textEdit("f","F");
}

void MainWindow::on_words_g_clicked()
{
    setValueInWord_textEdit("g","G");
}

void MainWindow::on_words_h_clicked()
{
    setValueInWord_textEdit("h","H");
}

void MainWindow::on_words_i_clicked()
{
    setValueInWord_textEdit("i","I");
}

void MainWindow::on_words_j_clicked()
{
    setValueInWord_textEdit("j","J");
}

void MainWindow::on_words_k_clicked()
{
    setValueInWord_textEdit("k","K");
}

void MainWindow::on_words_l_clicked()
{
    setValueInWord_textEdit("l","L");
}

void MainWindow::on_words_m_clicked()
{
    setValueInWord_textEdit("m","M");
}

void MainWindow::on_words_n_clicked()
{
    setValueInWord_textEdit("n","N");
}

void MainWindow::on_words_o_clicked()
{
    setValueInWord_textEdit("o","O");
}

void MainWindow::on_words_p_clicked()
{
    setValueInWord_textEdit("p","P");
}

void MainWindow::on_words_q_clicked()
{
    setValueInWord_textEdit("q","Q");
}

void MainWindow::on_words_r_clicked()
{
    setValueInWord_textEdit("r","R");
}

void MainWindow::on_words_s_clicked()
{
    setValueInWord_textEdit("s","S");
}

void MainWindow::on_words_t_clicked()
{
    setValueInWord_textEdit("t","T");
}

void MainWindow::on_words_u_clicked()
{
    setValueInWord_textEdit("u","U");
}

void MainWindow::on_words_v_clicked()
{
    setValueInWord_textEdit("v","V");
}

void MainWindow::on_words_w_clicked()
{
    setValueInWord_textEdit("w","W");
}

void MainWindow::on_words_x_clicked()
{
    setValueInWord_textEdit("x","X");
}

void MainWindow::on_words_y_clicked()
{
    setValueInWord_textEdit("y","Y");
}

void MainWindow::on_words_z_clicked()
{
    setValueInWord_textEdit("z","Z");
}

void MainWindow::on_horizontalSlider_actionTriggered(int action)
{
}

void MainWindow::on_Slider_text_size_actionTriggered(int action)
{
    QFont font = ui->words_textEdit->font();
    font.setPixelSize(ui->Slider_text_size->value());
    ui->words_textEdit->setFont(font);

    /*
     * save size text in config
     */
    config *conf = new config;
    QString value = QString::number(ui->Slider_text_size->value());
    conf->writeInConfig("sizeFont",value,"sizeFont");
}


void MainWindow::SetBackground(QString _nameBackground){
    QPixmap bkgnd(QApplication::applicationDirPath()+"/img/"+_nameBackground);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    EditColorApplication("0","0","0","200");

}

void MainWindow::SetNormalBackground(){
    this->setPalette(normalPaleteApplication);
    EditColorApplication("30","30","30","255");
}

void MainWindow::EditColorApplication(QString first_color,QString second_color,QString third_color,QString fourth_color){
    ui->search->setStyleSheet("QLineEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->english_new_word->setStyleSheet("QLineEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->russian_new_word->setStyleSheet("QLineEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->example->setStyleSheet("QLineEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->second_form_verb_textedit->setStyleSheet("QLineEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->third_form_verb_textedit->setStyleSheet("QLineEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->english_edit_word->setStyleSheet("QLineEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->russian_edit_word->setStyleSheet("QLineEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->example_edit->setStyleSheet("QLineEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->second_form_verb_edit_textedit->setStyleSheet("QLineEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->third_form_verb_edit_textedit->setStyleSheet("QLineEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->delete_word_textedit->setStyleSheet("QLineEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");

    ui->words_textEdit->setStyleSheet("QTextEdit {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");

    ui->words_a->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_b->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_c->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_d->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_e->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_f->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_g->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_h->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_i->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_j->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_k->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_l->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_m->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_n->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_o->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_p->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_q->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_r->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_s->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_t->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_u->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_v->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_w->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_x->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_y->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->words_z->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");


    ui->addNewWord->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->save_edited->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
    ui->delete_word->setStyleSheet("QPushButton {background-color: rgba("+first_color+","+second_color+","+third_color+","+fourth_color+");}");
}

void MainWindow::chooseBackground(){
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(!button)
        return;
    config *conf = new config;
    conf->writeInConfig("Background",button->text(),"BackgroundMainWindow");
    if(button->text() != "normal_background")
        SetBackground(button->text());
    else
        SetNormalBackground();
}

void MainWindow::createWindowChooseBackground(){
    QWidget *chooseBackground = new QWidget;
    QDir directoryImg(QApplication::applicationDirPath()+"/img");
    if(!directoryImg.exists()){
        directoryImg.mkdir(QApplication::applicationDirPath()+"/img");
    }
    QFileInfoList dirContent = directoryImg.entryInfoList(QStringList()
                                                          << "*.jpg"<<"*.png",
                                                          QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    qDebug()<<"images : "<<dirContent.count();
    QVBoxLayout *_layout = new QVBoxLayout;
    QPushButton *setBackgroundNormalButton = new QPushButton;
    setBackgroundNormalButton->setText("normal_background");
    _layout->addWidget(setBackgroundNormalButton);
    QObject::connect(setBackgroundNormalButton,&QPushButton::clicked,this,&MainWindow::chooseBackground);
    //create button ,the number of buttons is equal to the number of images + 1
    for(int i = 0;i<dirContent.count();i++){
        QPushButton *button = new QPushButton;
        button->setText(dirContent[i].fileName());
        _layout->addWidget(button);
        QObject::connect(button,&QPushButton::clicked,this,&MainWindow::chooseBackground);
    }
    chooseBackground->setLayout(_layout);
    chooseBackground->show();
}

void MainWindow::on_chooseBackground_clicked()
{
    createWindowChooseBackground();
}

void MainWindow::PutBackgroundOnMainWindow(){
    config *conf = new config;
    Background = conf->readInConfig("Background","normal","BackgroundMainWindow");
    if(Background != "normal_background")
        SetBackground(Background);
    else
        SetNormalBackground();
}
