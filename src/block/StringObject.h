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

#include "../gui/StringBlockModel.h"

#pragma once

#include <QObject>

#include "BlockBase.h"

class StringObject : public BlockBase {
    Q_OBJECT

  public:
    explicit StringObject()
      : BlockBase() {}

    void emitConfigSignals() override {
      emit stringChanged( string );
    }

    void toJSON( QJsonObject& json ) override {
      QJsonObject valuesObject;
      valuesObject[QStringLiteral( "String" )] = string;
      json[QStringLiteral( "values" )] = valuesObject;
    }

    void fromJSON( QJsonObject& json ) override {
      if( json[QStringLiteral( "values" )].isObject() ) {
        QJsonObject valuesObject = json[QStringLiteral( "values" )].toObject();

        if( valuesObject[QStringLiteral( "String" )].isString() ) {
          string = valuesObject[QStringLiteral( "String" )].toString();
        }
      }
    }

  signals:
    void stringChanged( QString );

  public:
    QString string;
};

class StringFactory : public BlockFactory {
    Q_OBJECT

  public:
    StringFactory( StringBlockModel* model )
      : BlockFactory(),
        model( model ) {}

    QString getNameOfFactory() override {
      return QStringLiteral( "String" );
    }

    virtual QNEBlock* createBlock( QGraphicsScene* scene, int id ) override {
      auto* obj = new StringObject();
      auto* b = createBaseBlock( scene, obj, id );

      b->addOutputPort( QStringLiteral( "String" ), QLatin1String( SIGNAL( stringChanged( QString ) ) ) );

      b->setBrush( valueColor );

      model->resetModel();

      return b;
    }

  private:
    StringBlockModel* model = nullptr;
};
