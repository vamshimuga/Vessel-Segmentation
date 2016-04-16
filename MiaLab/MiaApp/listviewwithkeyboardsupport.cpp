#include "listviewwithkeyboardsupport.h"
#include <QtAlgorithms>

listViewWithKeyBoardSupport::listViewWithKeyBoardSupport(QWidget *parent) :
    QListView(parent)
{
}
void listViewWithKeyBoardSupport::keyPressEvent ( QKeyEvent * event )
{
    if(event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
    {
        QItemSelectionModel* smodel = selectionModel () ;
        if(smodel->hasSelection())
        {
            QModelIndexList indexes = smodel->selection().indexes();
            QModelIndex index;
            QList<int> indexlist;
            foreach(index, indexes) {
                indexlist.append(index.row());
            }
            int i;
            qSort(indexlist.begin(), indexlist.end(), qGreater<int>());
            foreach(i, indexlist)
            {
                model()->removeRow(i);
            }
        }
    }
    QListView::keyPressEvent(event);
}
