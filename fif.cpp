#include "fif.h"
#include "ui_fif.h"
#include <QPainter>
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <qDebug>
#include <QPainter>
#include <QPixmap>
#include <qDebug>
#include <iostream>
#include <QDialog>
#include <QMessageBox>
#include <QApplication>

FIF::FIF(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FIF)
{
    ui->setupUi(this);

    ui->label->setText("轮到：蓝色");
    ui->widget->setStyleSheet("background-color: blue;");

    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();

    connect(ui->pushButton, &QPushButton::clicked, [=](){
        if(shapes.size() == 0) return;

        auto t = shapes.back();
        int xx = t.y / 40 - 1, yy =t.x / 40 - 1;
        st[xx][yy] = false;
        g[xx][yy] = 0;

        color = changeColor(t.color);
        qDebug() << color;

        // 需要注意connect这里应该是执行完全部才更新外部的，因为这个问题，我们可以反正写，这或许和第一个问题一样
        if(color == 2)
        {
            ui->label->setText("轮到：蓝色");
            ui->widget->setStyleSheet("background-color: blue;");
        }
        else if(color == 1)
        {
            ui->label->setText("轮到：红色");
            ui->widget->setStyleSheet("background-color: red;");
        }
        shapes.pop_back();
        update();
    });

}

void FIF::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    // 设置背景色

    painter.translate(40, 40);

    painter.fillRect(rect(), Qt::white);

    // 绘制网格
    painter.setPen(Qt::black);
    int gridSize = 40; // 网格大小
    for (int row = 0; row < 15; ++row) {
        for (int col = 0; col < 15; ++col) {
            int x = col * gridSize;
            int y = row * gridSize;
            painter.drawRect(x, y, gridSize, gridSize);
        }
    }

    QPainter painter2(this);
    for(auto &x: shapes)
    {
        if(x.color == 1) // 蓝色棋子
        {
            painter2.setPen(Qt::NoPen);
            painter2.setBrush(Qt::blue);
        }
        else if(x.color == 2) // 红色棋子
        {
            painter2.setPen(Qt::NoPen);
            painter2.setBrush(Qt::red);
        }
        painter2.drawEllipse(x.x, x.y, x.radius, x.radius);
    }

}

void FIF::reset()
{
    this->winner = "";
    memset(g, 0, sizeof(g));
    memset(st, 0, sizeof(st));
    color = 1;
    ui->label->setText("轮到：蓝色");
    ui->widget->setStyleSheet("background-color: blue;");
    shapes.clear();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
}

bool FIF::event(QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {

        QMouseEvent *ev = static_cast<QMouseEvent*> (e);
        int y = ev->x(), x = ev->y(); // ev中的是数学坐标轴，要转换为数组坐标轴

        x-=40, y-=40; // 对应点0，0
        x /= 40, y /=40; // 计算相应坐标
        dx = y, dy = x;

        QString str2 = QString("Even函数中：鼠标按下 x = %1, y = %2").arg(x).arg(y);

        if(!st[x][y] && x >= 0 && x < 15 && y >= 0 && y < 15)
        {
            st[x][y] = true;
            g[x][y] = color;

            shapes.append({(dx + 1) * 40, (dy + 1) * 40, 40, color});

            color = changeColor(color);

            if(color == 1)
            {
                ui->label->setText("轮到：蓝色");
                ui->widget->setStyleSheet("background-color: blue;");
            }
            else if(color == 2)
            {
                ui->label->setText("轮到：红色");
                ui->widget->setStyleSheet("background-color: red;");
            }

            update();
        }
        qDebug() << str2;


        if(winner == "") match();
        if(winner != "")
        {
            QString str = "";
            if(winner == "Blue") str = "蓝色胜利了！！";
            else if(winner == "Red") str = "红色胜利了！！";
            ui->label_2->show();
            ui->label_3->show();
            ui->label_4->show();
            ui->label_5->show();
            ui->label_6->show();
            ui->label_7->show();

            int choose = QMessageBox::information(this, "MVP结算界面", str, "不服!再战!!", "不玩了");
            if(choose == 0) reset();
            else if(choose == 1) QApplication::quit();
        }

        return true; // 事件处理完, 不向下分发
    }

    return QWidget::event(e); // 交给父类去分发
}


void FIF::match()
{
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if(st[i][j] && dfs(i, j))
            {
                if(g[i][j] == 1) winner = "Blue";
                else if(g[i][j] == 2) winner = "Red";
                return;
            }
        }
    }
}

bool FIF::dfs(int x, int y)
{
    const int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1}, dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

    for(int i = 0; i < 8; i ++)
    {
        int cnt = 1;
        int a = x, b = y;
        for(int j = 1; j <= 4; j++) // 延展长度
        {
            a += dx[i], b += dy[i];
            if(a >= 0 && a < N && b >= 0 && b < N && g[x][y] == g[a][b]) cnt++;
            else break;
        }
        if(cnt >= 5) return true;
    }
    return false;
}

int FIF::changeColor(int color)
{
    return 3 - color;
}

FIF::~FIF()
{
    delete ui;
}

