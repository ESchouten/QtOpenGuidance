// Copyright( C ) 2020 Christian Riggenbach
//
// This program is free software:
// you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// ( at your option ) any later version.
//
// This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY;
// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see < https : //www.gnu.org/licenses/>.

#ifndef XTEDOCKBLOCK_H
#define XTEDOCKBLOCK_H

#include <QObject>
#include <QDockWidget>
#include <QMenu>

#include "../gui/MyMainWindow.h"
#include "../gui/XteDock.h"

#include "BlockBase.h"

class XteDockBlock : public BlockBase {
    Q_OBJECT

  public:
    explicit XteDockBlock( MyMainWindow* mainWindow )
      : BlockBase() {
      widget = new XteDock( mainWindow );
      dock = new QDockWidget( mainWindow );
      dock->setWidget( widget );
    }

    ~XteDockBlock() {
      widget->deleteLater();
      dock->deleteLater();
      action->deleteLater();
    }


  public slots:
    void setName( const QString& name ) override {
      dock->setWindowTitle( name );
      action->setText( QStringLiteral( "XTE: " ) + name );
      widget->setName( name );
    }

    void setXte( float xte ) {
      widget->setXte( xte );
    }

  public:
    QDockWidget* dock = nullptr;
    QAction* action = nullptr;
    XteDock* widget = nullptr;
};

class XteDockBlockFactory : public BlockFactory {
    Q_OBJECT

  public:
    XteDockBlockFactory( MyMainWindow* mainWindow,
                         Qt::DockWidgetArea area,
                         Qt::DockWidgetAreas allowedAreas,
                         QDockWidget::DockWidgetFeatures features,
                         QMenu* menu )
      : BlockFactory(),
        mainWindow( mainWindow ),
        area( area ),
        allowedAreas( allowedAreas ),
        features( features ),
        menu( menu ) {}

    QString getNameOfFactory() override {
      return QStringLiteral( "XteDockBlock" );
    }

    virtual void addToCombobox( QComboBox* combobox ) override {
      combobox->addItem( getNameOfFactory(), QVariant::fromValue( this ) );
    }

    virtual BlockBase* createNewObject() override {
      return new XteDockBlock( mainWindow );
    }

    virtual QNEBlock* createBlock( QGraphicsScene* scene, QObject* obj ) override {
      auto* b = createBaseBlock( scene, obj );

      XteDockBlock* object = qobject_cast<XteDockBlock*>( obj );

      object->dock->setWidget( object->widget );
      object->dock->setFeatures( features );
      object->dock->setAllowedAreas( allowedAreas );
      object->dock->setObjectName( getNameOfFactory() + QString::number( b->id ) );

      object->action = object->dock->toggleViewAction();
      menu->addAction( object->action );

      mainWindow->addDockWidget( area, object->dock );

      b->addInputPort( QStringLiteral( "XTE" ), QLatin1String( SLOT( setXte( float ) ) ) );

      return b;
    }

  private:
    MyMainWindow* mainWindow = nullptr;
    Qt::DockWidgetArea area;
    Qt::DockWidgetAreas allowedAreas;
    QDockWidget::DockWidgetFeatures features;
    QMenu* menu = nullptr;
};

#endif // XTEDOCKBLOCK_H