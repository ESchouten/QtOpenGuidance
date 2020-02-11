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

#ifndef VALUEDOCKBLOCK_H
#define VALUEDOCKBLOCK_H

#include <QObject>
#include <QDockWidget>
#include <QSizePolicy>
#include <QMenu>

#include "../gui/MyMainWindow.h"
#include "../gui/ValueDock.h"

#include "BlockBase.h"
#include "ValueDockBlockBase.h"

class ValueDockBlock : public ValueDockBlockBase {
    Q_OBJECT

  public:
    explicit ValueDockBlock( MyMainWindow* mainWindow )
      : ValueDockBlockBase( mainWindow ) {
      widget = new ValueDock( mainWindow );
    }

    ~ValueDockBlock() {
      widget->deleteLater();
    }

    virtual const QFont& getFont() {
      return widget->fontOfLabel();
    }
    virtual int getPrecision() override {
      return widget->precision;
    }
    virtual int getFieldWidth() override {
      return widget->fieldWidth;
    }
    virtual double getScale() override {
      return widget->scale;
    }
    virtual bool captionEnabled() {
      return widget->captionEnabled();
    }

    virtual void setFont( const QFont& font ) {
      widget->setFontOfLabel( font );
    }
    virtual void setPrecision( int precision ) override {
      widget->precision = precision;
    }
    virtual void setFieldWidth( int fieldWidth ) override {
      widget->fieldWidth = fieldWidth;
    }
    virtual void setScale( double scale ) override {
      widget->scale = scale;
    }
    virtual void setCaptionEnabled( bool enabled ) override {
      widget->setCaptionEnabled( enabled );
    }

  public slots:
    void setName( const QString& name ) override {
      dock->setWindowTitle( name );
      action->setText( QStringLiteral( "Value: " ) + name );
      widget->setName( name );
    }

    void setValue( float value ) {
      widget->setMeter( value );
    }

  public:

    void toJSON( QJsonObject& json ) override {
      QJsonObject valuesObject;

      valuesObject[QStringLiteral( "Font" )] = QJsonValue::fromVariant( QVariant( widget->fontOfLabel() ) );
      valuesObject[QStringLiteral( "Precision" )] = widget->precision;
      valuesObject[QStringLiteral( "Scale" )] = widget->scale;
      valuesObject[QStringLiteral( "FieldWitdh" )] = widget->fieldWidth;

      json[QStringLiteral( "values" )] = valuesObject;
    }

    void fromJSON( QJsonObject& json ) override {
      if( json[QStringLiteral( "values" )].isObject() ) {
        QJsonObject valuesObject = json[QStringLiteral( "values" )].toObject();

        widget->setFontOfLabel( valuesObject[QStringLiteral( "Font" )].toVariant().value<QFont>() );
        widget->precision = valuesObject[QStringLiteral( "Precision" )].toInt();
        widget->scale = valuesObject[QStringLiteral( "Scale" )].toDouble();
        widget->fieldWidth = valuesObject[QStringLiteral( "FieldWitdh" )].toInt();
      }
    }

    ValueDock* widget = nullptr;
};

class ValueDockBlockFactory : public BlockFactory {
    Q_OBJECT

  public:
    ValueDockBlockFactory( MyMainWindow* mainWindow,
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
      return QStringLiteral( "ValueDockBlock" );
    }

    virtual void addToCombobox( QComboBox* combobox ) override {
      combobox->addItem( getNameOfFactory(), QVariant::fromValue( this ) );
    }

    virtual BlockBase* createNewObject() override {
      return new ValueDockBlock( mainWindow );
    }

    virtual QNEBlock* createBlock( QGraphicsScene* scene, QObject* obj ) override {
      auto* b = createBaseBlock( scene, obj );

      ValueDockBlock* object = qobject_cast<ValueDockBlock*>( obj );

      object->dock->setWidget( object->widget );
      object->dock->setFeatures( features );
      object->dock->setAllowedAreas( allowedAreas );
      object->dock->setObjectName( getNameOfFactory() + QString::number( b->id ) );

      object->action = object->dock->toggleViewAction();
      menu->addAction( object->action );

      mainWindow->addDockWidget( area, object->dock );

      b->addInputPort( QStringLiteral( "Number" ), QLatin1String( SLOT( setValue( float ) ) ) );

      return b;
    }

  private:
    MyMainWindow* mainWindow = nullptr;
    Qt::DockWidgetArea area;
    Qt::DockWidgetAreas allowedAreas;
    QDockWidget::DockWidgetFeatures features;
    QMenu* menu = nullptr;
};

#endif // VALUEDOCKBLOCK_H