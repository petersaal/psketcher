#include "QtPrimitiveBase.h"

QtPrimitiveBase::QtPrimitiveBase(QGraphicsItem * parent ) : 
QGraphicsItem(parent)
{

}

void QtPrimitiveBase::Display()
{

}

void QtPrimitiveBase::UpdateDisplay()
{

}

bool QtPrimitiveBase::IsSelected()
{
	return isSelected();
}

void QtPrimitiveBase::SetSelectable(bool selectable)
{
	return setFlag(QGraphicsItem::ItemIsSelectable, selectable);
}

// Erase current ais_object from the ais_context
void QtPrimitiveBase::Erase()
{

}
