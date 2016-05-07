#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QTextCodec>
#include <stdio.h>

#define Max 111111
struct dict{
    char *key;                                               //words  it can be used in malloc
    char *content;                                           //words in translation
};

/*Open the dict and read the content in it*/
int open_dict(struct dict **p, const char *dict_filename){
    FILE *istream;
    if ( (istream = fopen ( "dict.txt", "r" ) ) == NULL )
    {
    printf ( "file non-existant!\n" );
    }

    FILE *pfile = fopen(dict_filename, "r");                 //open it by READ only
    if (pfile == NULL)
        return 0;                                            //If open failed return 0

    *p = (struct dict *)malloc(sizeof(struct dict) * Max);   //Distribute The Memory
    memset(*p, 0, sizeof(struct dict)*Max);                  //Initial it to 0
    struct dict *pD = *p;                                    //pD point to the front address of p .It's an array

    char buf[1024] = { 0 };                                  //Cache
    size_t len = 0;                                          //mean long unsigned int in 64bit sys
    int i = 0;                                               //counting method
    while (!feof(pfile)){                                    //Read the file recursively
        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf), pfile);                      //read one line
        len = strlen(buf);
        if (len > 0){
            pD[i].key = (char *)malloc(len);                 //distribute the memory according to the length
            memset(pD[i].key, 0, len);
            strcpy(pD[i].key, &buf[1]);                      //copy it into the key Notice it begins from 1  [#a]
        }

        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf), pfile);
        len = strlen(buf);
        if (len > 0){
            pD[i].content = (char *)malloc(len);
            memset(pD[i].content, 0, len);
            strcpy(pD[i].content, &buf[6]);
        }
        i++;
    }
    fclose(pfile);                                           //close the file

    return i;                                                //return the number of key
}

/*Search for the content according to the key*/
int search_dict(const struct dict *p, int size, const char *key, char *content){
    int i = 0;
    for (i = 0; i < size; i++){
        if ((p[i].key == NULL) || (p[i].content == NULL))
            continue;
        if (strncmp(p[i].key, key, strlen(key)) == 0)
        {
            strcpy(content, p[i].content);
            return 1;                                        //find the word ,return 1
        }
    }
    return 0;                                                //if not ,return 0
}


void free_dict(struct dict *p, int size)
{
    int i = 0;
    for (i = 0; i < size; i++)                               //Free the memory recursively
    {
        if (p[i].key)
            free(p[i].key);
        if (p[i].content)
            free(p[i].content);
    }
    free(p);                                                 //free p
}

struct dict * p = NULL;
int dict_size = 0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    dict_size = open_dict(&p, "dict.txt");                   //open the file
    if (dict_size == 0)
        exit(0);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_2_clicked()
{
    this->close();
}

void Widget::on_pushButton_clicked()
{
    char key[1024];
    char content[1024];
    memset(key, 0, sizeof(key));
    memset(content, 0, sizeof(content));

    //QString text1;
    //text1 = ui->lineEdit->text();                          //get the user's input from the widge
    QTextCodec *codec = QTextCodec::codecForName("GBK");     //要实现GBK和utf8之间编码的转化

    strcpy(key,codec->fromUnicode(ui->lineEdit->text()));
    if (search_dict(p, dict_size, key, content)){
        ui->label->setText(codec->toUnicode(content));       //codec->toUnicode(content);//将GBK编码格式转化为utf8格式
    }
    else{
        ui->label->setText("Not Found");
    }
}
