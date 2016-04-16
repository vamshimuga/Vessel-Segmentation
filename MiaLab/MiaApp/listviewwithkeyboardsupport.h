#ifndef LISTVIEWWITHKEYBOARDSUPPORT_H
#define LISTVIEWWITHKEYBOARDSUPPORT_H

#include <QListView>
#include <QtGui>

class listViewWithKeyBoardSupport : public QListView
{
    Q_OBJECT
public:
    explicit listViewWithKeyBoardSupport(QWidget *parent = 0);

signals:

public slots:
protected:
    void keyPressEvent ( QKeyEvent * event );

};

#endif // LISTVIEWWITHKEYBOARDSUPPORT_H
