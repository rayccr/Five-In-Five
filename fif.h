#ifndef FIF_H
#define FIF_H

#include <QMainWindow>
#include <QList>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class FIF; }
QT_END_NAMESPACE

const int N = 15;

struct Shape {
    int x;
    int y; // 屏幕坐标，并不是矩阵坐标
    int radius;
    int color;
};

class FIF : public QMainWindow
{
    Q_OBJECT

public:
    FIF(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent* event);
    bool event(QEvent *e);
    int color = 1; // 1 bule; 2 red

    int changeColor(int color);
    int dx = 0, dy = 0;
    bool dfs(int x, int y);

    QString winner = "";

    void match();
    void reset();

    ~FIF();

private:
    int g[N][N] = {0};
    bool st[N][N] = {0};
    QList<Shape> shapes;

private:
    Ui::FIF *ui;
};
#endif // FIF_H
